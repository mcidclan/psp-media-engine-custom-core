project(kcall C) 

add_custom_command(
    OUTPUT exports.c
    COMMAND psp-build-exports -b src/exports.exp > exports.c
    DEPENDS src/exports.exp
    COMMENT "Generating exports"
)

add_executable(kcall src/main.c exports.c)
target_link_options(kcall PRIVATE 
    -Wl,-q,-T${PSPDEV}/psp/sdk/lib/linkfile.prx 
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
