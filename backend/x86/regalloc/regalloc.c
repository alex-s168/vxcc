#include "../backend.h"
#include "regalloc.h"

reg_t alloc_reg(env_t env,
                datatype_t type,
                size_t bit_size,
                size_t additional) {

}

reg_t alloc_reg_force(env_t env,
                      datatype_t type,
                      size_t bit_size,
                      size_t additional) {

}

void dealloc_reg(reg_t reg) {
    // TODO: check for other locations if they should be promoted
}