cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{PSPDEV}/psp/share/pspdev.cmake" CACHE PATH "toolchain file")
project(me-interrupt-sleep C CXX ASM)

add_executable(mesleep
  main.cpp
)

target_compile_options(mesleep PRIVATE -Ofast -G0 -Wall -fno-pic)

target_compile_options(mesleep PRIVATE 
  $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
)

target_include_directories(mesleep PRIVATE 
  ${CMAKE_SOURCE_DIR}/../..
)

target_link_libraries(mesleep
  ${CMAKE_SOURCE_DIR}/../../libme-core.a
  psppower pspge pspdisplay pspctrl pspdebug
)

create_pbp_file(TARGET mesleep TITLE "Me Interrupt Sleep")
