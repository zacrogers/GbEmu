cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(gb_emu)

include_directories(
    ../emu_core/inc
    ../emu_core/src
    )

target_sources(app PRIVATE
    src/main.cpp
    ../emu_core/src/registers.cpp
)
