find_package(OpenCL REQUIRED)

add_executable(infsys)

target_include_directories(infsys
	PRIVATE .
)

target_sources(infsys
	PRIVATE infsys.c
			hardware/bios.c
            hardware/cpu/info.c
            hardware/cpu/cache.c
)

target_link_libraries(infsys
	PRIVATE OpenCL::OpenCL
)

if (WIN32)
    target_sources(infsys
    		PRIVATE ui/win_ui.c
    )
else ()
    find_package(PkgConfig REQUIRED)

	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

	target_include_directories(infsys
		PRIVATE ${GTK3_INCLUDE_DIRS}
	)

	target_link_directories(infsys
		PRIVATE ${GTK3_LIBRARY_DIRS}
	)

	target_link_libraries(infsys
		PRIVATE ${GTK3_LIBRARIES}
	)

	target_sources(infsys
		PRIVATE ui/unix_ui.c
    )
endif ()