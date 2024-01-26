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

#include "regalloc.h"

typedef struct {
    target_t target;
    regalloc_state_t regalloc;
} backend_data_t;

void assert(env_t env, bool test, char *message) {
    if (!test) {
        errbuilder_t e = env.start_error();
        e.text(e.impl_data, message);
        e.end(e.impl_data, true);
    }
}

#endif //VXCC_X86_BACKEND_H
