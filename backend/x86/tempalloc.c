#include "backend.h"

// first tries to allocate into a compatible register. If none are available, allocates onto the stack
int allocate_compatible(env_t env, location_t *dest, location_t *loc) {
    metadata_t meta = loc->meta;

    location_t reg_maybe;
    bool reg_ok = alloc_reg(env,
                            &reg_maybe,
                            false,
                            true,           // idonno   // TODO: refactor so this is saved in location_t (maybe move to datatype)
                            meta.datatype,
                            meta.bit_size,
                            meta.additional);

    if (reg_ok) {
        *dest = reg_maybe;
        return true;
    }

    allocate_stack(env, dest, meta.datatype, meta.bit_size, meta.additional);

    return true;
}