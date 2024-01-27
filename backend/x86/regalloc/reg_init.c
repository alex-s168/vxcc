#include "../backend.h"
#include "regalloc.h"
#include <string.h>

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