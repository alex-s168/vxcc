#include "backend.h"
#include <stdio.h>
#include <stdlib.h>

static void err_text(void *impl_data, char *text) {
    fprintf(stderr, "%s", text);
}

static void err_end(void *impl_data, int should_exit) {
    if (should_exit) {
        exit(1);
    }
}

static errbuilder_t err() {
    return (errbuilder_t) {
        .text = err_text,
        .end = err_end,
    };
}

int main() {
    env_t env = { 0 };
    // char line[1024];
    // scanf("%1023[^\n]", line);
    char *line = "fpu;mmx;sse1;sse2;";
    env.target_str_extra = line;
    env.start_error = err;
    env.out_file = fopen("out.asm", "w");
    init(&env);

    printf("target configuration:\n");
    print_target(get_bd(env)->target);
    putchar('\n');

    bool need_arithm = true;
    datatype_t datatype = DT_FLOAT;
    size_t bit_size = 64;
    size_t bit_size_add = 64;

    bool exists = false;
    bool can_be_used = false;
    size_t similar_count;
    reg_with_owner_internal_t *similar;
    reg_with_owner_internal_t *reg = get_best_reg(env,
                                                  &exists,
                                                  &can_be_used,
                                                  &similar_count,
                                                  &similar,
                                                  need_arithm,
                                                  datatype,
                                                  bit_size,
                                                  bit_size_add);

    static char *str_true = "true";
    static char *str_false = "false";

#define ST_BOOL(B) (B ? str_true : str_false)

    printf("exists = %s\n", ST_BOOL(exists));
    printf("can_be_used = %s\n", ST_BOOL(can_be_used));
    printf("asm = %s\n", (reg != NULL) ? (reg->reg.asname) : "(null)");

#undef ST_BOOL

    location_t my_loc_a;
    bool ok_a = alloc_reg(env, &my_loc_a, false, true, DT_UINT, 32, 32);
    if (!ok_a) {
        return 1;
    }

    location_t my_loc_b;
    bool ok_b = alloc_reg(env, &my_loc_b, false, true, DT_UINT, 32, 32);
    if (!ok_b) {
        return 1;
    }

    move(env, &my_loc_a, &my_loc_b);

    location_t my_st_loc_d;
    allocate_stack(env, &my_st_loc_d, DT_UINT, 32, 32);

    location_t my_st_loc_e;
    allocate_stack(env, &my_st_loc_e, DT_UINT, 32, 32);

    move(env, &my_loc_b, &my_st_loc_d);

    location_t my_loc_f;
    bool ok_f = alloc_reg(env, &my_loc_f, false, true, DT_UINT, 32, 32);
    if (!ok_f) {
        return 1;
    }

    move(env, &my_loc_f, &my_st_loc_e);

    dealloc_reg(&my_loc_f);

    dealloc_reg(&my_loc_b);

    location_t my_loc_c;
    bool ok_c = alloc_reg(env, &my_loc_c, false, true, DT_UINT, 32, 32);
    if (!ok_c) {
        return 1;
    }

    move(env, &my_loc_c, &my_loc_a);

    deallocate_stack(&my_st_loc_e);

    location_t my_st_loc_g;
    allocate_stack(env, &my_st_loc_g, DT_UINT, 32, 32);

    deallocate_stack(&my_st_loc_d);

    move(env, &my_loc_c, &my_st_loc_g);

    deallocate_stack(&my_st_loc_g);

    dealloc_reg(&my_loc_c);

    fclose(env.out_file);

    deinit(env);
    return 0;
}