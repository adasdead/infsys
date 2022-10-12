add_executable(infsys)

target_include_directories(infsys PRIVATE .)

target_sources(infsys PRIVATE
    infsys.c
    cpu/cpuinfo.c
)

if (WIN32)
    target_sources(infsys PRIVATE
        windows/gui.c
    )
else()

endif()