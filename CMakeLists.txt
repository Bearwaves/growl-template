cmake_minimum_required (VERSION 3.20)

cmake_policy(SET CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(mytitle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
	"src/main.cpp"
	"src/game.cpp"
	)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /permissive-")
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic")
	set(CMAKE_OBJCXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif ()

add_subdirectory(growl)

if (GROWL_ANDROID)
	add_library(mytitle SHARED ${SOURCES} ${HEADERS})
else ()
	add_executable(mytitle ${SOURCES} ${HEADERS})
endif ()

if (GROWL_DESKTOP)
	target_link_libraries(mytitle PRIVATE growl-platform-desktop)
elseif (GROWL_IOS)
	target_link_libraries(mytitle PRIVATE growl-platform-ios)
	target_compile_options(mytitle PRIVATE -x objective-c++)
elseif (GROWL_ANDROID)
	target_link_libraries(mytitle PUBLIC growl-platform-android)
elseif (GROWL_WEB)
	target_link_libraries(mytitle PRIVATE growl-platform-web)
	target_link_options(mytitle PRIVATE --preload-file assets.growl)
	set(CMAKE_EXECUTABLE_SUFFIX .html)
	# We need to use this over the more modern target_link_options as CMake
	# currently struggles with dollar signs in linker flags.
	set(CMAKE_EXE_LINKER_FLAGS
		"${CMAKE_EXE_LINKER_FLAGS} -sDEFAULT_LIBRARY_FUNCS_TO_INCLUDE=$ccall"
		)
endif()
