#ifndef VXCC_X86_REGALLOC_H
#define VXCC_X86_REGALLOC_H

#ifndef VXCC_BACKEND_IMPL
#define VXCC_BACKEND_IMPL
#include "../../vxcc_backend.h"
#endif

typedef struct {
    datatype_t type;
    size_t bit_size;
    size_t additional;
    char *asname;
    unsigned char fpu_stack_level;
} reg_internal_t;

typedef struct {
    reg_internal_t reg;
    location_t *owner;
} reg_with_owner_internal_t;

#define REG_GP_COUNT  4
#define REG_FPU_COUNT 8
#define REG_MMX_COUNT 8
#define REG_SSE_COUNT 8

typedef struct {
    reg_with_owner_internal_t reg_gp  [REG_GP_COUNT];

    reg_with_owner_internal_t reg_fpu [REG_FPU_COUNT];
    bool reg_fpu_used;

    reg_with_owner_internal_t reg_mmx [REG_MMX_COUNT];
    bool reg_mmx_used;

    reg_with_owner_internal_t reg_sse [REG_SSE_COUNT];

    location_t *to_promote_count;
    location_t *to_promote;
} regalloc_state_t;

bool alloc_reg(env_t env,
               location_t *dest,
               bool force,
               bool need_arithm,
               datatype_t type,
               size_t bit_size,
               size_t additional);

void dealloc_reg(location_t *reg);

reg_with_owner_internal_t *get_best_reg(env_t env,
                                       bool *exists,
        /* set to false if all registers that can be considered for the specified ops are in use. (still returns the last register) */
                                       bool *can_be_used,

        /* a list of registers with the same properties (can also include the returned reg) */
                                       size_t *similar_count,
                                       reg_with_owner_internal_t **similar,

                                       bool need_arithm,
                                       datatype_t type,
                                       size_t bit_size,
                                       size_t additional);

void regalloc_init(env_t *env);

#endif //VXCC_X86_REGALLOC_H
