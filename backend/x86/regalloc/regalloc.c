#include "../backend.h"
#include "regalloc.h"

bool alloc_reg(env_t env,
               location_t *dest,
               bool force,
               bool need_arithm,
               datatype_t type,
               size_t bit_size,
               size_t additional) {

    bool exists;
    bool can_be_used;
    size_t similar_count;
    reg_with_owner_internal_t *similar;

    reg_with_owner_internal_t *reg = get_best_reg(env,
                                                  &exists,
                                                  &can_be_used,
                                                  &similar_count,
                                                  &similar,
                                                  need_arithm,
                                                  type,
                                                  bit_size,
                                                  additional);

    if (!exists || (!can_be_used && !force)) {
        return false;
    }

    if (reg->reg.bit_size < bit_size) {
        return false;
    }

    location_t l = { .type = LT_REG, .reg = get_reg(reg), .backend_data = env.backend_data };

    if (!can_be_used) {
        ASSERT(force);

        location_t *old_owner = reg->owner;
        location_t temp;
        (void) allocate_compatible(env, &temp, &l);
        move(env, &temp, &l);
        *old_owner = temp;
        reg->owner = dest;
        *dest = l;

        return true;
    }

    ASSERT(exists);

    *dest = l;
    reg->owner = dest;
    return true;
}

void dealloc_reg(location_t *reg) {
    reg_with_owner_internal_t *l = (reg_with_owner_internal_t *) (reg->reg.backend_data);
    l->owner = NULL;
}