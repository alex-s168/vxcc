#include "../backend.h"
#include "stackalloc.h"
#include <stdlib.h>

bool allocate_stack(env_t env,
                    location_t *dest,
                    datatype_t type,
                    size_t bit_size,
                    size_t additional) {

    stackalloc_state_t *state =
            &((backend_data_t *) env.backend_data)->stackalloc;

    ASSERT(state->frames_count > 0);

    stackalloc_frame_t *frame = &state->frames[state->frames_count - 1];

    size_t where = frame->count ++;

    size_t bit_size_8 = up8(bit_size);
    size_t byte_size = bit_size_8 / 8;

    size_t address = frame->next_addr;
    frame->next_addr += byte_size;

    frame->elements = realloc(frame->elements,
                              sizeof(stackalloc_elem_t) * frame->count);
    frame->elements[where] = (stackalloc_elem_t) {
        .deallocatable = false,
        .abs_addr = address
    };

    *dest = (location_t) {
        .type = LT_STACK,
        .stack = (stack_t) {
            .type = type,
            .bit_size = bit_size_8,
            .additional = additional,
            .backend_data = (void *) where,
            .abs_addr = address,
        }
    };

    return true;
}

void deallocate_stack(location_t *loc) {
    stackalloc_state_t *state =
            &((backend_data_t *) loc->backend_data)->stackalloc;

    ASSERT(state->frames_count > 0);

    stackalloc_frame_t *frame = &state->frames[state->frames_count - 1];
    size_t pos = (size_t) loc->stack.backend_data;

    frame->elements[pos].deallocatable = true;

    size_t i = frame->count - 1;
    for (; i > 0; i --) {
        bool d = frame->elements[i].deallocatable;
        if (!d)
            break;
    }

    size_t to_rem = frame->count - i;

    ASSERT(to_rem > 0);

    frame->count -= to_rem;

    if (to_rem > 16) {
        void *new = realloc(frame->elements,
                            sizeof(stackalloc_elem_t) * frame->count);
        if (new != NULL) {
            frame->elements = new;
        }
    }
}

void stackalloc_init(env_t *env) {
    stackalloc_state_t *state =
            &((backend_data_t *) env->backend_data)->stackalloc;

    state->sp = 0;
    state->frames_count = 1;
    state->frames_alloc = 16;
    state->frames = malloc(16 * sizeof(stackalloc_frame_t));
    state->frames[0].count = 0;
    state->frames[0].next_addr = 0;
    state->frames[0].elements = NULL;
}

void stackalloc_deinit(env_t env) {
    stackalloc_state_t *state =
            &((backend_data_t *) env.backend_data)->stackalloc;

    for (size_t i = 0; i < state->frames_count; i ++) {
        stackalloc_frame_t frame = state->frames[i];
        free(frame.elements);
    }

    free(state->frames);
}