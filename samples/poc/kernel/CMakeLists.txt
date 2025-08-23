cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{PSPDEV}/psp/share/pspdev.cmake" CACHE PATH "toolchain file")
project(kcall C) 

add_custom_command(
  OUTPUT exports.c
  COMMAND psp-build-exports -b ${CMAKE_CURRENT_SOURCE_DIR}/src/exports.exp > exports.c
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/exports.exp
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating exports"
)

add_executable(kcall src/main.c exports.c)

target_compile_options(kcall PRIVATE 
  -fsingle-precision-constant 
  -Ofast 
  -G0 
  -Wall
)

target_include_directories(kcall PRIVATE 
  $ENV{PSPDEV}/psp/sdk/include
)

target_link_options(kcall PRIVATE 
  -Wl,-q,-T$ENV{PSPDEV}/psp/sdk/lib/linkfile.prx 
  -nostartfiles 
  -Wl,-zmax-page-size=128
)

add_custom_command(
  OUTPUT kcall.prx
  COMMAND psp-prxgen $<TARGET_FILE:kcall> kcall.prx
  DEPENDS kcall
  COMMENT "Generating PRX"
)

add_custom_target(kcall_prx ALL DEPENDS kcall.prx)
