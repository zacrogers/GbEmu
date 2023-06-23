cmake_minimum_required(VERSION 3.20.0)


set(LIB_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/modules/etl)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
find_package(etl REQUIRED)
project(gb_emu)

FILE(GLOB app_sources src/*.cpp)
target_sources(app PRIVATE ${app_sources})


# include_directories(
#     ../emu_core/inc
#     ../emu_core/src
# )

target_link_libraries(app PRIVATE etl::etl)
target_sources(app PRIVATE
src/main.cpp
# ../emu_core/src/cpu.cpp
# ../emu_core/src/registers.cpp
# ../emu_core/src/process.cpp
# ../emu_core/src/instructions.cpp
# ../emu_core/src/cart.cpp
)
# add_library(etl::etl STATIC IMPORTED GLOBAL)
# set_target_properties(etl::etl PROPERTIES IMPORTED_LOCATION ${LIB_DIR}/OtherLib.a)
