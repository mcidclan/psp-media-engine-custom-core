cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{PSPDEV}/psp/share/pspdev.cmake" CACHE PATH "toolchain file")
project(me-audio-shared C CXX ASM)

add_executable(me-audio-shared
  main.cpp
)

target_compile_options(me-audio-shared PRIVATE -Os -G0 -Wall -fno-pic -DM_PI=3.14159265358979323846f)

target_compile_options(me-audio-shared PRIVATE 
  $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
)

target_link_libraries(me-audio-shared
  me-core psppower pspdisplay pspctrl pspdebug pspge pspaudio
)

create_pbp_file(TARGET me-audio-shared TITLE "Me Audio Shared Buffer" BUILD_PRX)
