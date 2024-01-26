#include "backend.h"
#include "regalloc.h"
#include "string.h"

// TODO: should promote location_t (on every operation, check.) ?

static reg_with_owner_internal_t reg_gp_def  [REG_GP_COUNT] = {
    { .owner = NULL, .reg = { .asname = "eax", .bit_size = 32 } },
    { .owner = NULL, .reg = { .asname = "ebx", .bit_size = 32 } },
    { .owner = NULL, .reg = { .asname = "ecx", .bit_size = 32 } },
    { .owner = NULL, .reg = { .asname = "edx", .bit_size = 32 } },
};
static reg_with_owner_internal_t reg_mmx_def [REG_MMX_COUNT] = {
#define MMX_REG(name) { .owner = NULL, .reg = { .asname = name, .bit_size = 64 } }
    MMX_REG("mm0"),
    MMX_REG("mm1"),
    MMX_REG("mm2"),
    MMX_REG("mm3"),
    MMX_REG("mm4"),
    MMX_REG("mm5"),
    MMX_REG("mm6"),
    MMX_REG("mm7"),
#undef MMX_REG
};
static reg_with_owner_internal_t reg_sse_def [REG_SSE_COUNT] = {
#define SSE_REG(name) { .owner = NULL, .reg = { .asname = name, .bit_size = 128 } }
    SSE_REG("xmm0"),
    SSE_REG("xmm1"),
    SSE_REG("xmm2"),
    SSE_REG("xmm3"),
    SSE_REG("xmm4"),
    SSE_REG("xmm5"),
    SSE_REG("xmm6"),
    SSE_REG("xmm7"),
#undef SSE_REG
};

static char *reg_fpu_names[REG_FPU_COUNT] = {
    "st0", "st1", "st2", "st3", "st4", "st5", "st6", "st7"
};

void regalloc_init(env_t *env) {
    backend_data_t *bd = env->backend_data;

    bd->regalloc.reg_fpu_used = false;
    bd->regalloc.reg_mmx_used = false;

    memcpy(bd->regalloc.reg_gp,
           reg_gp_def,
           sizeof(reg_gp_def));

    if (bd->target.is_mmx) {
        memcpy(bd->regalloc.reg_mmx,
               reg_mmx_def,
               sizeof(reg_mmx_def));
    }

    if (bd->target.is_sse1) {
        memcpy(bd->regalloc.reg_sse,
               reg_sse_def,
               sizeof(reg_sse_def));
    }

    if (bd->target.is_fpu) {
        for (size_t i = 0; i < REG_FPU_COUNT; i ++) {
            reg_with_owner_internal_t *reg = &bd->regalloc.reg_fpu[i];
            reg->reg.asname = reg_fpu_names[i];
            reg->reg.type = DT_FLOAT;
            reg->reg.fpu_stack_level = i;
            reg->owner = NULL;
        }
    }
}

reg_with_owner_internal_t get_best_reg(env_t env,
                                       bool *exists,
                                       /* set to false if all registers that can be considered for the specified ops are in use. (still returns the last register) */
                                       bool *can_be_used,
                                       bool need_arithm,
                                       datatype_t type,
                                       size_t bit_size,
                                       /* if additional == bit_size for vectors, size doesn't matter */
                                       size_t additional) {
    /*
     mm registers (MMX) are on the floating point stack. BE CAREFUL
     SSE added xmm registers (not on the floating point stack), that are unrelated to mm registers. NO INT OP
     SSE2 allowed floating point operations for xmm registers.
     */

    backend_data_t *bd = env.backend_data;

    // on the floating point stack
    bool fpr = (type == DT_FLOAT);

    // general purpose register
    bool gpr = (type == DT_UINT || type == DT_SINT);

    // float vector (requires sse)
    bool fvr = (type == DT_VEC_FLOAT);

    // int vector (requires mmx or sse)
    bool ivr = (type == DT_VEC_UINT || type == DT_VEC_SINT);

    if (gpr && bit_size > 32) {
        gpr = false;

        if (need_arithm) {
            *exists = false;
            return (reg_with_owner_internal_t) {};
        }

        ivr = true;
    }

    if (fpr && bit_size > 64) {
        fpr = false;

        if (need_arithm) {
            *exists = false;
            return (reg_with_owner_internal_t) {};
        }

        ivr = true;
    }

    // check if we can maybe off-load to a different register, so we make space for mmx
    if (fpr &&
        !need_arithm &&
        bd->target.is_mmx &&
        !bd->target.is_sse1 &&
        !bd->regalloc.reg_fpu_used) {
        // TODO: maybe let's not do that and always use mmx registers if possible; **TEST**
        if (bit_size <= 32) {
            // try to use a gp reg

            bool t_exists;
            bool t_usable;
            reg_with_owner_internal_t reg =
                    get_best_reg(env, &t_exists, &t_usable, false, DT_UINT, bit_size, 0);

            if (t_exists && t_usable) {
                return reg;
            }
        }
        if (bit_size <= 64 && bd->regalloc.reg_mmx_used) {
            // try to use a mmx reg

            bool t_exists;
            bool t_usable;
            reg_with_owner_internal_t reg =
                    get_best_reg(env, &t_exists, &t_usable, false, DT_VEC_UINT, bit_size, bit_size);

            if (t_exists && t_usable) {
                return reg;
            }
        }
    }

    if (fpr && !bd->target.is_fpu) {
        *exists = false;
        return (reg_with_owner_internal_t) {};
    }

    if (fvr && !bd->target.is_sse2) {
        *exists = false;
        return (reg_with_owner_internal_t) {};
    }

    if (ivr && !(bd->target.is_mmx || bd->target.is_sse1)) {
        *exists = false;
        return (reg_with_owner_internal_t) {};
    }

    if (gpr) {
        for (size_t i = 0; i < REG_GP_COUNT; i++) {
            reg_with_owner_internal_t reg = bd->regalloc.reg_gp[i];
            if (reg.owner != NULL)
                continue;

            *can_be_used = true;
            *exists = true;

            reg.reg.type = type;
            return reg;
        }

        *can_be_used = false;
        *exists = true;

        reg_with_owner_internal_t reg = bd->regalloc.reg_gp[0];
        reg.reg.type = type;
        return reg;
    }

    if (fpr) {
        if (bd->regalloc.reg_mmx_used) {
            *can_be_used = false;
            *exists = true;
            assert(env, bd->regalloc.reg_fpu[0].owner == NULL,
                   "regalloc.c:get_best_reg:(fpr && mmx_used)");
            return bd->regalloc.reg_fpu[0];
        }

        bd->regalloc.reg_fpu_used = true;

        for (size_t i = 0; i < REG_FPU_COUNT; i++) {
            reg_with_owner_internal_t reg = bd->regalloc.reg_fpu[i];
            if (reg.owner != NULL)
                continue;

            *can_be_used = true;
            *exists = true;
            return reg;
        }

        *exists = true;
        *can_be_used = false;
        return bd->regalloc.reg_fpu[0];
    }

    // TODO: vectors
}

// TODO: float + mmx problem

reg_t alloc_reg_force(env_t env,
                      datatype_t type,
                      size_t bit_size,
                      size_t additional) {

}

void dealloc_reg(reg_t reg) {
    // TODO: check for other locations if they should be promoted
}