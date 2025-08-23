cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{PSPDEV}/psp/share/pspdev.cmake" CACHE PATH "toolchain file")
project(me-core-demo CXX ASM)

add_subdirectory(kernel)

add_custom_command(
  OUTPUT kcall.S
  COMMAND psp-build-exports -s ${CMAKE_SOURCE_DIR}/kernel/src/exports.exp > kcall.S
  DEPENDS kcall_prx
  COMMENT "Generating imports"
)

add_executable(mecore kcall.S main.cpp)
target_compile_options(mecore PRIVATE -Ofast -G0 -Wall -fno-pic -std=c++11 -fno-rtti -fno-exceptions)

target_include_directories(mecore PRIVATE 
    kernel/src
    ${CMAKE_SOURCE_DIR}/../..
)

target_include_directories(mecore PRIVATE kernel/src)
target_link_libraries(mecore psppower pspge pspdisplay pspctrl pspdebug)

create_pbp_file(TARGET mecore TITLE "Me Core Demo")

add_custom_command(TARGET mecore POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_BINARY_DIR}/kernel/kcall.prx
  ${CMAKE_BINARY_DIR}/kcall.prx
  DEPENDS kcall_prx
  COMMENT "Copying kcall.prx to build directory"
)
