cmake_minimum_required(VERSION 3.27)
project(vxcc C)

set(CMAKE_C_STANDARD 11)

add_executable(vxcc main.c
        vxcc_backend.h
        vxcc.h
        vxcc_frontend.h
        backend/x86/backend.c
        backend/x86/backend.h
        backend/x86/regalloc.c)
