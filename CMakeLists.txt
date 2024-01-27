cmake_minimum_required(VERSION 3.27)
project(vxcc C)

set(CMAKE_C_STANDARD 11)

add_executable(vxcc main.c
        vxcc_backend.h
        vxcc.h
        vxcc_frontend.h
        backend/x86/backend.c
        backend/x86/backend.h
        backend/x86/regalloc/regalloc.c
        backend/x86/test.c
        backend/x86/regalloc/get_best_reg.c
        backend/x86/regalloc/reg_init.c
        backend/x86/stackalloc/stackalloc.c
        backend/x86/stackalloc/stackalloc.h)
