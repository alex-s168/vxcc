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
    char *line = "mmx;sse1;sse2;";
    env.target_str_extra = line;
    env.start_error = err;
    init(&env);

    print_target(get_bd(env)->target);
    putchar('\n');

    bool need_arithm = true;
    datatype_t datatype = DT_UINT;
    size_t bit_size = 64;
    size_t bit_size_add = 64;

    bool exists = false;
    bool can_be_used = false;
    size_t similar_count;
    reg_with_owner_internal_t *similar;
    reg_with_owner_internal_t reg = get_best_reg(env,
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
    printf("asm = %s\n", reg.reg.asname);

#undef ST_BOOL

    deinit(env);
    return 0;
}