cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{PSPDEV}/psp/share/pspdev.cmake" CACHE PATH "toolchain file")
project(me-core-vme C CXX ASM)

add_executable(me-core-vme
  main.cpp
)

target_compile_options(me-core-vme PRIVATE -Os -G0 -Wall -fno-pic)

target_compile_options(me-core-vme PRIVATE 
  $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
)

target_link_libraries(me-core-vme
  me-core psppower pspge pspdisplay pspctrl pspdebug
)

create_pbp_file(TARGET me-core-vme TITLE "Me Core VME" BUILD_PRX)
