#ifndef VXCC_X86_BACKEND_H
#define VXCC_X86_BACKEND_H

#define VXCC_BACKEND_IMPL
#include "../../vxcc_backend.h"

#include <stdbool.h>

void emit_str(env_t env, char *str);

typedef struct {
    bool is_fpu;
    bool is_mmx;
    bool is_mmxplus;    // https://refspecs.linuxfoundation.org/AMD-extensions.pdf
    bool is_sse1;
    bool is_sse2;
    // TODO
    // bool is_3dnow;      // https://refspecs.linuxfoundation.org/AMD-3Dnow.pdf
} target_t;

#include "regalloc/regalloc.h"
#include "stackalloc/stackalloc.h"

typedef struct {
    target_t target;
    regalloc_state_t regalloc;
    stackalloc_state_t stackalloc;
} backend_data_t;

static void assert(env_t env, bool test, char *message) {
    if (!test) {
        errbuilder_t e = env.start_error();
        e.text(e.impl_data, message);
        e.end(e.impl_data, true);
    }
}

static void print_target(target_t tg) {
    static char *str_true = "true";
    static char *str_false = "false";

#define ST_BOOL_PT(B) (B ? str_true : str_false)
    printf("fpu   %s\n", ST_BOOL_PT(tg.is_fpu));
    printf("mmx   %s\n", ST_BOOL_PT(tg.is_mmx));
    printf("mmx+  %s\n", ST_BOOL_PT(tg.is_mmxplus));
    printf("sse1  %s\n", ST_BOOL_PT(tg.is_sse1));
    printf("sse2  %s\n", ST_BOOL_PT(tg.is_sse2));
#undef ST_BOOL_PT
}

static backend_data_t *get_bd(env_t env) {
    return (backend_data_t *) env.backend_data;
}

static reg_t get_reg(reg_with_owner_internal_t *reg) {
    return (reg_t) {
        .type = reg->reg.type,
        .bit_size = reg->reg.additional,
        .additional = reg->reg.additional,
        .backend_data = reg,
    };
}

// assert only meant for code readability and debugging
#define ASSERT(v) do {} while (0)

void emit_loc(env_t env, location_t loc);

#endif //VXCC_X86_BACKEND_H
