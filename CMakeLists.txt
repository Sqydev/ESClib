cmake_minimum_required(VERSION 3.20)

project(ESClib
	VERSION 0.0.1
	LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(ESCLIB_BUILD_STATIC
	"Build static ESClib library"
	ON
)

option(ESCLIB_BUILD_SHARED
	"Build shared ESClib library"
	ON
)

option(ESCLIB_BUILD_TESTER
	"Build ESClib tester"
	OFF
)

option(ESCLIB_DEBUG
	"Enable ESClib debug configuration"
	OFF
)


set(
	ESCLIB_OUTPUT_DIR
	"${PROJECT_SOURCE_DIR}/lib"
	CACHE PATH
	"Directory where final ESClib binaries are placed"
)


file(GLOB ESCLIB_PUBLIC_SOURCES
	CONFIGURE_DEPENDS
	"${PROJECT_SOURCE_DIR}/src/*.c"
)

file(GLOB_RECURSE ESCLIB_PRIVATE_SOURCES
	CONFIGURE_DEPENDS
	"${PROJECT_SOURCE_DIR}/src/private/*.c"
)

set(ESCLIB_SOURCES
	${ESCLIB_PUBLIC_SOURCES}
	${ESCLIB_PRIVATE_SOURCES}
)

if(NOT ESCLIB_SOURCES)
	message(FATAL_ERROR
		"No ESClib sources found"
	)
endif()


add_library(esclib_build_options INTERFACE)

target_compile_options(esclib_build_options
	INTERFACE
		-Wall
		-Wextra
		-Werror
		-Wformat=2
		-Wshadow
		-Wstrict-prototypes
)

target_compile_definitions(esclib_build_options
	INTERFACE
		CL_TARGET_OPENCL_VERSION=300
)

if(NOT WIN32)
	target_compile_definitions(esclib_build_options
		INTERFACE
			_POSIX_C_SOURCE=200809L
	)
endif()


if(NOT ESCLIB_DEBUG)
	target_compile_options(esclib_build_options
		INTERFACE
			-O2
	)

	target_compile_definitions(esclib_build_options
		INTERFACE
			NDEBUG
	)
else()
	target_compile_options(esclib_build_options
		INTERFACE
			-Og
			-g3
			-fno-omit-frame-pointer
			-fsanitize=address,undefined
	)
endif()


function(esclib_configure_library TARGET_NAME)
	target_link_libraries(${TARGET_NAME}
		PRIVATE
			esclib_build_options
	)

	target_include_directories(${TARGET_NAME}
		PUBLIC
			"${PROJECT_SOURCE_DIR}/include"

		PRIVATE
			"${PROJECT_SOURCE_DIR}/src"
			"${PROJECT_SOURCE_DIR}/src/private"
			"${PROJECT_SOURCE_DIR}/src/linclude"
	)

	if(UNIX AND NOT APPLE)
		target_link_libraries(${TARGET_NAME}
			PRIVATE
				m
				${CMAKE_DL_LIBS}
		)
	endif()
endfunction()


if(ESCLIB_BUILD_STATIC)
	add_library(esclib_static STATIC
		${ESCLIB_SOURCES}
	)

	esclib_configure_library(esclib_static)

	set_target_properties(esclib_static PROPERTIES

		OUTPUT_NAME esclib

		ARCHIVE_OUTPUT_DIRECTORY
			"${ESCLIB_OUTPUT_DIR}"
	)
endif()


if(ESCLIB_BUILD_SHARED)
	add_library(esclib_shared SHARED
		${ESCLIB_SOURCES}
	)

	esclib_configure_library(esclib_shared)

	set_target_properties(esclib_shared PROPERTIES

		OUTPUT_NAME esclib

		POSITION_INDEPENDENT_CODE ON

		LIBRARY_OUTPUT_DIRECTORY
			"${ESCLIB_OUTPUT_DIR}"

		RUNTIME_OUTPUT_DIRECTORY
			"${ESCLIB_OUTPUT_DIR}"

		ARCHIVE_OUTPUT_DIRECTORY
			"${ESCLIB_OUTPUT_DIR}"
	)

	if(ESCLIB_DEBUG)
		target_link_options(esclib_shared
			PRIVATE
				-fsanitize=address,undefined
		)
	endif()
endif()


if(ESCLIB_BUILD_TESTER)
	if(NOT TARGET esclib_static)
		message(FATAL_ERROR
			"Tester requires ESCLIB_BUILD_STATIC=ON"
		)
	endif()


	file(GLOB_RECURSE TESTER_SOURCES
		CONFIGURE_DEPENDS
		"${PROJECT_SOURCE_DIR}/tester/src/*.c"
	)

	if(NOT TESTER_SOURCES)
		message(FATAL_ERROR
			"No tester sources found"
		)
	endif()


	add_executable(esclib_tester
		${TESTER_SOURCES}
	)

	target_include_directories(esclib_tester
		PRIVATE
			"${PROJECT_SOURCE_DIR}/include"
			"${PROJECT_SOURCE_DIR}/tester/src"
	)

	target_link_libraries(esclib_tester
		PRIVATE
			esclib_static
			esclib_build_options
	)

	if(ESCLIB_DEBUG)
		target_link_options(esclib_tester
			PRIVATE
				-fsanitize=address,undefined
		)
	endif()

	set_target_properties(esclib_tester PROPERTIES
		OUTPUT_NAME tester

		RUNTIME_OUTPUT_DIRECTORY
			"${ESCLIB_OUTPUT_DIR}"
	)
endif()
