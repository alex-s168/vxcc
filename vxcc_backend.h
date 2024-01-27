/*
 This file will be included into backends and into backend_t struct in vxcc_frontend.h
 */

#ifndef VXCC_VXCC_H
#include "vxcc.h"
#endif

#ifndef VXCC_BACKEND_IMPL
#define XS (*
#define XE )
#else
#define XS /* */
#define XE /* */
#endif

void XS init XE (env_t *);

void XS deinit XE (env_t);

int XS allocate XE (env_t env,
                    location_t *dest,
                    int is_temp,
                    int needs_arithm,
                    datatype_t type,
                    size_t bit_size,
                    size_t additional,
                    int align_bits, // -1 is automatic
                    int pad_bits);  // -1 is automatic

int XS allocate_compatible XE (env_t env, location_t *dest, location_t *loc);

void XS deallocate XE (env_t env, location_t *loc);

void XS move XE (env_t, location_t *dst, location_t *src);

#undef XS
#undef XE