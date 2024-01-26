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

location_t XS allocate XE (int *ok, datatype_t type, size_t bit_size, size_t additional, size_t align_bits, size_t pad_bits);

void XS deallocate XE(location_t);

void XS move XE(env_t, location_t dst, location_t src);

#undef XS
#undef XE