cmake_minimum_required(VERSION 3.7)
set (CMAKE_CXX_STANDARD 17)
project(GameboyEmulatorDesktop)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(etl REQUIRED)


include_directories (
    ${SDL2_INCLUDE_DIRS}
    ../emu_core/inc
    ../emu_core/src
)

add_executable(GameboyEmulatorDesktop
    src/main.cpp
    ../emu_core/src/cpu.cpp
    ../emu_core/src/cart.cpp
    ../emu_core/src/registers.cpp
    ../emu_core/src/process.cpp
    ../emu_core/src/instructions.cpp
)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf)
target_link_libraries(${PROJECT_NAME}     etl::etl)
