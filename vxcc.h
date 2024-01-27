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
    size_t addr;

    void *backend_data;
} addr_abs_t;

typedef struct {
    datatype_t type;
    size_t bit_size;
    size_t additional;
    size_t abs_addr;         // relative to function call

    void *backend_data;
} stack_t;

struct location_s;

typedef struct {
    datatype_t datatype;
    size_t combined_bit_size;
    size_t additional;

    size_t locations_len;
    struct location_s *locations;
} combined_location_t;

// TODO: refactor to use everywhere
typedef struct {
    datatype_t datatype;
    size_t bit_size;
    size_t additional;
} metadata_t;

typedef struct location_s {
    enum {
        LT_REG,
        LT_IMM,
        LT_ADDR_ABS,
        LT_STACK,
        LT_COMBINED
    } type;
    void *backend_data;
    union {
        reg_t reg;
        imm_t imm;
        addr_abs_t addr_abs;
        stack_t stack;
        combined_location_t combined;
        metadata_t meta;
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
