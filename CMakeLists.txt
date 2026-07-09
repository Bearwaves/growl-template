cmake_minimum_required (VERSION 3.20)

cmake_policy(SET CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(mytitle)

set(NAME "My Title")
set(SHORTNAME "Title")
set(IDENTIFIER "com.bearwaves.mytitle")
set(COPYRIGHT "2026 Bearwaves Limited")
set(VERSION "1.0.0")
set(BUILD_NUMBER "1")

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
	target_sources(mytitle PRIVATE
		res/ios/Assets.xcassets
		assets_built/assets.growl
	)
	set_source_files_properties(assets_built/assets.growl
		PROPERTIES MACOSX_PACKAGE_LOCATION Resources
	)
	set(RESOURCE_FILES
		"res/ios/LaunchScreen.storyboard"
		"res/ios/Assets.xcassets"
	)
	set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "15.6")
	set_target_properties(mytitle
		PROPERTIES
		MACOSX_BUNDLE YES
		XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${IDENTIFIER}"
		MACOSX_BUNDLE_GUI_IDENTIFIER "${IDENTIFIER}"
		XCODE_ATTRIBUTE_GENERATE_INFOPLIST_FILE "true"
		XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
		XCODE_ATTRIBUTE_PRODUCT_NAME "${NAME}"
		MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/res/ios/Info.plist.in"
		XCODE_ATTRIBUTE_INFOPLIST_KEY_CFBundleDisplayName "${SHORTNAME}"
		XCODE_ATTRIBUTE_INFOPLIST_KEY_UIRequiresFullScreen "true"
		XCODE_ATTRIBUTE_INFOPLIST_KEY_UIStatusBarHidden "true"
		XCODE_ATTRIBUTE_INFOPLIST_KEY_UILaunchStoryboardName "LaunchScreen.storyboard"
		XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "Icon"
		XCODE_ATTRIBUTE_MARKETING_VERSION "${VERSION}"
		XCODE_ATTRIBUTE_CURRENT_PROJECT_VERSION "${BUILD_NUMBER}"
		XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
		XCODE_ATTRIBUTE_SKIP_INSTALL "false"
		XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "$(inherited)"
		RESOURCE "${RESOURCE_FILES}"
	)
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
