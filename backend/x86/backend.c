#include "backend.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void emit_str(env_t env, char *str) {
    fwrite(str, 1, strlen(str), env.out_file);
}

static void parseTargetStr(env_t *env, backend_data_t *bd) {
    /* extra target str */
    char *cstr = env->target_str_extra;
    char *last = cstr;
    while (*cstr != '\0') {
        if (*cstr == ';') {
            if (strstr(last, "fpu;") == last) {
                bd->target.is_fpu = true;
            }
            else if (strstr(last, "mmx;") == last) {
                bd->target.is_mmx = true;
            }
            else if (strstr(last, "mmx+;") == last) {
                bd->target.is_mmxplus = true;
                bd->target.is_mmx = true;
            }
            else if (strstr(last, "sse1;") == last || strstr(last, "sse;") == last) {
                bd->target.is_sse1 = true;
            }
            else if (strstr(last, "sse2;") == last) {
                bd->target.is_sse1 = true;
                bd->target.is_sse2 = true;
            }

            last = cstr + 1;
        }
        cstr ++;
    }
}

void init(env_t *env) {
    backend_data_t *bd = malloc(sizeof(backend_data_t));
    if (bd == NULL) {
        errbuilder_t e = env->start_error();
        e.text(e.impl_data, "Out of memory!");
        e.end(e.impl_data, true);
        return;
    }

    memset(&bd->target, 0, sizeof(target_t));

    env->backend_data = bd;

    parseTargetStr(env, bd);

    regalloc_init(env);

    emit_str(*env, "  bits 32\n");
}

void deinit(env_t env) {
    backend_data_t *bd = (backend_data_t *) env.backend_data;
    free(bd);
}