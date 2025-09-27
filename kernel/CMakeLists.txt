cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{PSPDEV}/psp/share/pspdev.cmake" CACHE PATH "toolchain file")
project(kcall C) 

add_custom_command(
  OUTPUT exports.c
  COMMAND psp-build-exports -b ${CMAKE_CURRENT_SOURCE_DIR}/exports.exp > exports.c
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/exports.exp
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating exports"
)

add_executable(kcall main.c exports.c)

target_compile_options(kcall PRIVATE
  -Wno-narrowing
  -fsingle-precision-constant 
  -Os 
  -G0
  -Wall
)

target_link_options(kcall PRIVATE 
  -nostartfiles
  -Wl,-q
  -Wl,-T$ENV{PSPDEV}/psp/sdk/lib/linkfile.prx
  -Wl,-zmax-page-size=128
  -Wl,--strip-all
)

add_custom_command(
  OUTPUT kcall.prx
  COMMAND psp-prxgen $<TARGET_FILE:kcall> kcall.prx
  COMMAND psp-fixup-imports kcall.prx
  DEPENDS kcall
  COMMENT "Generating PRX"
)

add_custom_target(kcall_prx ALL DEPENDS kcall.prx)
