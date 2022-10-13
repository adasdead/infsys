add_executable(infsys)

target_include_directories(infsys PRIVATE .)

target_sources(infsys PRIVATE
    application.c

    # CPU
    cpu/cpuinfo.c
    cpu/cpucache.c
)

if (WIN32)
    target_sources(infsys PRIVATE
        windows/gui.c
    )
else()

endif()