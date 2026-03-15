execute_process(
  COMMAND psp-gcc --version
  OUTPUT_VARIABLE PSP_GCC_VERSION_OUTPUT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX MATCH "([0-9]+\\.[0-9]+\\.[0-9]+)" PSP_GCC_VERSION "${PSP_GCC_VERSION_OUTPUT}")
string(REGEX MATCH "^[^\n]*" PSP_GCC_VERSION_LINE "${PSP_GCC_VERSION_OUTPUT}")
if(PSP_GCC_VERSION VERSION_LESS "15.1.1")
  message(WARNING "Please make sure you have the minimum gcc version:\n    Required: psp-gcc (GCC) 15.1.1 20250425\n    Current:  ${PSP_GCC_VERSION_LINE}")
endif()

cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE "$ENV{PSPDEV}/psp/share/pspdev.cmake" CACHE PATH "toolchain file")
project(me-core C ASM)

add_subdirectory(kernel)

add_custom_command(
  OUTPUT kcall.S
  COMMAND psp-build-exports -s ${CMAKE_SOURCE_DIR}/kernel/exports.exp > kcall.S
  DEPENDS kcall_prx
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating kcall imports"
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/embedded.c
  COMMAND xxd -i ${CMAKE_CURRENT_BINARY_DIR}/kernel/kcall.prx > ${CMAKE_CURRENT_BINARY_DIR}/embedded.c
  COMMAND sed -i 's/unsigned char [^[]*/unsigned char embedded_kcall/' ${CMAKE_CURRENT_BINARY_DIR}/embedded.c
  COMMAND sed -i 's/unsigned int [^ ]*_len/unsigned int embedded_kcall_len/' ${CMAKE_CURRENT_BINARY_DIR}/embedded.c
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/kernel/kcall.prx
  COMMENT "Embedding kcall.prx into C file"
)

add_custom_target(kcall_files DEPENDS kcall.S)

add_library(me-core STATIC
  me-lib.c
  me-core-custom.c
  me-core-mapping.c
  context.S
  embedded.c
  ${CMAKE_CURRENT_BINARY_DIR}/kcall.S
)

add_dependencies(me-core kcall_files)

target_compile_options(me-core PRIVATE 
  -Os
  -G0
  -Wall 
  -fno-pic
)

target_include_directories(me-core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS me-core
  ARCHIVE DESTINATION "$ENV{PSPDEV}/psp/lib"
)

file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
list(FILTER HEADER_FILES EXCLUDE REGEX ".*me-core-mapping\\.def\\.h$")

install(FILES ${HEADER_FILES}
  DESTINATION "$ENV{PSPDEV}/psp/include/me-core-mapper"
)

file(GLOB KERNEL_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.h")

install(FILES ${KERNEL_HEADER_FILES}
  DESTINATION "$ENV{PSPDEV}/psp/include/me-core-mapper/kernel"
)
