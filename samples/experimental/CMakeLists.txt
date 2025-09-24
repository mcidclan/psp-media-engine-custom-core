cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{PSPDEV}/psp/share/pspdev.cmake" CACHE PATH "toolchain file")
project(me-dsp-exp C CXX ASM)

add_executable(medspexp
  main.cpp
)

target_compile_options(medspexp PRIVATE -Ofast -G0 -Wall -fno-pic)

target_compile_options(medspexp PRIVATE 
  $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
)

target_link_libraries(medspexp
  me-core psppower pspge pspdisplay pspctrl pspdebug
)

create_pbp_file(TARGET medspexp TITLE "Me DSP Experimental")
