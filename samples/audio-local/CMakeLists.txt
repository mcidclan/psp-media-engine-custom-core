cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{PSPDEV}/psp/share/pspdev.cmake" CACHE PATH "toolchain file")
project(audio-local C CXX ASM)

add_executable(audio-local
  main.cpp
)

target_compile_options(audio-local PRIVATE -Os -G0 -Wall -fno-pic)

target_compile_options(audio-local PRIVATE 
  $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
)

target_link_libraries(audio-local
  me-core psppower pspge pspdisplay pspctrl pspdebug pspaudio
)

create_pbp_file(TARGET audio-local TITLE "audio_hw_test" BUILD_PRX)
