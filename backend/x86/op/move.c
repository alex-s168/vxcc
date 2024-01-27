#include "../backend.h"

void move(env_t env, location_t *dst, location_t *src) {
    // TODO consider all possible combinations

    // remove this:
    emit_str(env, "mov ");
    emit_loc(env, *dst);
    emit_str(env, ", ");
    emit_loc(env, *src);
    emit_str(env, "\n");
}