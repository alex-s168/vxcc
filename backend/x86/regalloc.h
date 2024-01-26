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
} regalloc_state_t;

reg_t alloc_reg_force(env_t env,
                      datatype_t type,
                      size_t bit_size,
                      size_t additional);

reg_with_owner_internal_t get_best_reg(env_t env,
                                       bool *exists,
                                       bool *can_be_used,
                                       bool need_arithm,
                                       datatype_t type,
                                       size_t bit_size,
                                       size_t additional);

void dealloc_reg(reg_t reg);

void regalloc_init(env_t *env);

#endif //VXCC_X86_REGALLOC_H
