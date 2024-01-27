#ifndef VXCC_X86_STACKALLOC_H
#define VXCC_X86_STACKALLOC_H

#ifndef VXCC_BACKEND_IMPL
#define VXCC_BACKEND_IMPL
#include "../../vxcc_backend.h"
#endif

typedef struct {
    bool deallocatable;
    size_t abs_addr;
} stackalloc_elem_t;

typedef struct {
    size_t count;
    stackalloc_elem_t *elements;

    size_t next_addr;
} stackalloc_frame_t;

typedef struct {
    size_t frames_count;
    size_t frames_alloc;
    stackalloc_frame_t *frames;
    size_t sp;
} stackalloc_state_t;

bool allocate_stack(env_t env,
                    location_t *dest,
                    datatype_t type,
                    size_t bit_size,
                    size_t additional);

void deallocate_stack(location_t *);

void stackalloc_init(env_t *env);

void stackalloc_deinit(env_t env);

#endif //VXCC_X86_STACKALLOC_H
