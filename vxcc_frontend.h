#ifndef VXCC_VXCC_FRONTEND_H
#define VXCC_VXCC_FRONTEND_H

#include "vxcc.h"

typedef struct {
#include "vxcc_backend.h"
} *backend_t;

// TODO
void load_backend(backend_t b) {
#define SYMBOL(x) 0
    b->init             = SYMBOL("init");
    b->allocate         = SYMBOL("allocate");
    b->deallocate       = SYMBOL("deallocate");
    b->move             = SYMBOL("move");
#undef SYMBOL
}

#endif //VXCC_VXCC_FRONTEND_H
