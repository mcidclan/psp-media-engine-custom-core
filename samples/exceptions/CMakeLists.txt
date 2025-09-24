cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{PSPDEV}/psp/share/pspdev.cmake" CACHE PATH "toolchain file")
project(me-exceptions C CXX ASM)

add_executable(meexceptions
  main.cpp
)

target_compile_options(meexceptions PRIVATE -Ofast -G0 -Wall -fno-pic)

target_compile_options(meexceptions PRIVATE 
  $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
)

target_link_libraries(meexceptions
  me-core psppower pspge pspdisplay pspctrl pspdebug
)

create_pbp_file(TARGET meexceptions TITLE "Me Exceptions")
