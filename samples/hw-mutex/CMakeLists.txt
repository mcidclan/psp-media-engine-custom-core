cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{PSPDEV}/psp/share/pspdev.cmake" CACHE PATH "toolchain file")
project(me-core-demo C CXX ASM)

add_subdirectory(kernel)

add_custom_command(
  OUTPUT kcall.S
  COMMAND psp-build-exports -s ${CMAKE_SOURCE_DIR}/kernel/src/exports.exp > kcall.S
  DEPENDS kcall_prx
  COMMENT "Generating imports"
)

add_executable(mehwmutex
  kcall.S
  main.cpp
  ${CMAKE_SOURCE_DIR}/../../me-core-mapping.c
)
  
target_compile_options(mehwmutex PRIVATE -Ofast -G0 -Wall -fno-pic)

target_compile_options(mehwmutex PRIVATE 
  $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
)

target_include_directories(mehwmutex PRIVATE 
  kernel/src
  ${CMAKE_SOURCE_DIR}/../..
)

target_link_libraries(mehwmutex psppower pspge pspdisplay pspctrl pspdebug)

create_pbp_file(TARGET mehwmutex TITLE "Me Hw Mutex Demo")

add_custom_command(TARGET mehwmutex POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_BINARY_DIR}/kernel/kcall.prx
  ${CMAKE_BINARY_DIR}/kcall.prx
  DEPENDS kcall_prx
  COMMENT "Copying kcall.prx to build directory"
)
