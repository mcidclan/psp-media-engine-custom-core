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
  OUTPUT kcall.module.o
  COMMAND ${CMAKE_COMMAND} -E copy
          ${CMAKE_CURRENT_BINARY_DIR}/kernel/kcall.prx
          kcall.prx

  COMMAND psp-objcopy -I binary -O elf32-littlemips -B mips
            --rename-section .data=.data,alloc,load,data,contents
            --set-section-alignment .data=64
            --redefine-sym _binary_kcall_prx_start=kcall_module_start
            --redefine-sym _binary_kcall_prx_end=kcall_module_end
            --redefine-sym _binary_kcall_prx_size=kcall_module_size
            kcall.prx kcall.module.o

  DEPENDS kcall_prx
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Converting PRX to object file"
)

add_custom_target(kcall_files DEPENDS kcall.S kcall.module.o)

add_library(me-core STATIC
  me-lib.c
  me-core-mapping.c
  context.S
  ${CMAKE_CURRENT_BINARY_DIR}/kcall.S
  ${CMAKE_CURRENT_BINARY_DIR}/kcall.module.o
)

add_dependencies(me-core kcall_files)

target_compile_options(me-core PRIVATE 
  -Ofast
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
