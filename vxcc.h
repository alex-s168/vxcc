//
// Created by super on 26/01/2024.
//

#ifndef VXCC_VXCC_H
#define VXCC_VXCC_H

#include <stdio.h>

typedef enum {
    DT_SINT,
    DT_UINT,
    DT_FLOAT,
    DT_VEC_FLOAT,    // additional = element size
    DT_VEC_SINT,     // additional = element size
    DT_VEC_UINT,     // additional = element size
} datatype_t;

typedef struct {
    datatype_t type;
    size_t bit_size;
    size_t additional;

    void *backend_data;
} reg_t;

typedef struct {
    datatype_t type;
    size_t bit_size;
    size_t additional;
    void *data;

    void *backend_data;
} imm_t;

typedef struct {
    datatype_t type;
    size_t bit_size;
    size_t additional;
    void *addr;

    void *backend_data;
} addr_t;

typedef struct {
    datatype_t type;
    size_t bit_size;
    size_t additional;
    void *abs_addr;         // relative to function call

    void *backend_data;
} stack_t;

typedef struct {
    enum {
        LT_REG,
        LT_IMM,
        LT_ADDR,
        LT_STACK
    } type;
    void *backend_data;
    union {
        reg_t reg;
        imm_t imm;
        addr_t addr;
        stack_t stack;
    };
} location_t;

static inline
datatype_t location_datatype(location_t l) {
    // yes this works because all elements in the union start with the datatype
    return l.reg.type;
}

typedef struct {
    void (*text)(void *impl_data, char *t);
    void (*end)(void *impl_data, int should_exit);

    void *impl_data;
} errbuilder_t;

typedef struct {
    /* target */
    char *target_name;
    char *target_str_extra;
    void *backend_data;

    /* output */
    FILE *out_file;

    /* errors / warnings */
    errbuilder_t (*start_error)();
} env_t;

#endif //VXCC_VXCC_H
