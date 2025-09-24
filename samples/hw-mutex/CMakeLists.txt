cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{PSPDEV}/psp/share/pspdev.cmake" CACHE PATH "toolchain file")
project(me-core-demo C CXX ASM)

add_executable(mehwmutex
  main.cpp
)

target_compile_options(mehwmutex PRIVATE -Ofast -G0 -Wall -fno-pic)

target_compile_options(mehwmutex PRIVATE 
  $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
)

target_link_libraries(mehwmutex
  me-core psppower pspge pspdisplay pspctrl pspdebug
)

create_pbp_file(TARGET mehwmutex TITLE "Me Hw Mutex Demo")
