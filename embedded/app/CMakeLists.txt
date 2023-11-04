cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
find_package(etl REQUIRED)
FILE(GLOB app_sources src/*.cpp)
# target_compile_options(app PRIVATE -DLUA_32BITS -DLUA_C89_NUMBERS)
target_sources(app PRIVATE ${app_sources})
# target_sources(app PRIVATE src/states/pong.cpp)
# target_sources(app PRIVATE src/states/main_menu.cpp)
project(gb_emu)

if(CONFIG_VROOM_ENABLE_WIFI)
    target_sources(app PRIVATE src/connectivity/wifi.cpp)
endif()

zephyr_include_directories(../modules/etl)
# zephyr_include_directories(../modules/lua)
# include_directories(
#     ../emu_core/inc
#     ../emu_core/src
# )


target_link_libraries(app PRIVATE etl::etl)
target_sources(app PRIVATE src/main.cpp)
# ../emu_core/src/cpu.cpp
# ../emu_core/src/registers.cpp
# ../emu_core/src/process.cpp
# ../emu_core/src/instructions.cpp
# ../emu_core/src/cart.cpp
# )
# add_library(etl::etl STATIC IMPORTED GLOBAL)
# set_target_properties(etl::etl PROPERTIES IMPORTED_LOCATION ${LIB_DIR}/OtherLib.a)
