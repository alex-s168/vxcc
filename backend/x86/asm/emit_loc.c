#include "../backend.h"

static void emit_size(env_t env, size_t bits) {
    size_t bits_8 = bits + (8 - bits % 8);
    size_t bytes = bits_8 / 8;

    switch (bytes) {
        case 1:
            emit_str(env, "byte ");
            break;
        case 2:
            emit_str(env, "word ");
            break;
        case 3:case 4:
            emit_str(env, "dword ");
            break;
        case 5:case 6:case 7:case 8:
            emit_str(env, "qword ");
            break;
        case 9:case 10:case 11:case 12:
        case 13:case 14:case 15:case 16:
            emit_str(env, "xmmword ");  // or oword
            break;
        default:
            assert(env, false, "emit_size:size too big");
            break;
    }
}

void emit_loc(env_t env, location_t loc) {
    backend_data_t *bd = (backend_data_t *) env.backend_data;
    switch (loc.type) {
        case LT_ADDR_ABS: {
            emit_size(env, loc.addr_abs.bit_size);
            char buf[35];
            sprintf(buf, "[%zu]", loc.addr_abs.addr);
            emit_str(env, buf);
        }
        break;

        case LT_REG: {
            reg_with_owner_internal_t *l =
                    (reg_with_owner_internal_t *) (loc.reg.backend_data);
            emit_str(env, l->reg.asname);
        }
        break;

        case LT_STACK: {
            size_t sp = bd->stackalloc.sp;
            size_t addr = loc.stack.abs_addr;
            size_t offm = sp - addr;
            emit_size(env, loc.stack.bit_size);
            char buf[35];
            sprintf(buf, "[sp - %zu]", offm);
            emit_str(env, buf);
        }
        break;

        // TODO

        default: {
            assert(env, false, "emit_loc:no support");
        }
        break;
    }
}