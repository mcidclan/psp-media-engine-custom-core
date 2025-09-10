cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{PSPDEV}/psp/share/pspdev.cmake" CACHE PATH "toolchain file")
project(me-core-demo C CXX ASM)

add_executable(mecore
  main.cpp
)

target_compile_options(mecore PRIVATE -Ofast -G0 -Wall -fno-pic)

target_compile_options(mecore PRIVATE 
  $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
)

target_include_directories(mecore PRIVATE 
  ${CMAKE_SOURCE_DIR}/../..
)

target_link_libraries(mecore
  ${CMAKE_SOURCE_DIR}/../../libme-core.a
  psppower pspge pspdisplay pspctrl pspdebug
)

create_pbp_file(TARGET mecore TITLE "Me Core Demo")
