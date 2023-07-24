#General base configuration should be included from all ACF-based projects

message("PROJECT_NAME " ${PROJECT_NAME})
message("CMAKE_CURRENT_LIST_DIR " ${CMAKE_CURRENT_LIST_DIR})
message("PROJECT_SOURCE_DIR " ${PROJECT_SOURCE_DIR})
message("BINARY_DIR " ${BINARY_DIR})

set(COMPILER_NAME "CLang")

if(${MSVC})
	set(COMPILER_NAME "VC")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /wd4127 /wd4250 /wd4347 /wd4355 /wd4365 /wd4481 /wd4505 /wd4510 /wd4511 /wd4512 /wd4548 /wd4571 /wd4619 /wd4625 /wd4626 /wd4640 /wd4702 /wd4710 /wd4820 /wd4826 /we4701")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /MP /fp:fast")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /bigobj")

	if(${MSVC_TOOLSET_VERSION} STREQUAL 90)
		string(REPLACE "/Gd" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
		string(REPLACE "/GD" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
		string(REPLACE "-Gd" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
		string(REPLACE "-GD" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	endif()

	if(${MSVC_TOOLSET_VERSION} STREQUAL 100)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
	endif()

	if(${MSVC_TOOLSET_VERSION} STREQUAL 110)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
	endif()

	if(${MSVC_TOOLSET_VERSION} STREQUAL 120)
		add_compile_options(-std=c++11)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996 /Qpar /Gy /Gw /FS")
	endif()

	if(${MSVC_TOOLSET_VERSION} STREQUAL 140)
		set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /Qpar /Gy /Gw /FS /Zc:threadSafeInit-")
		set(CMAKE_CXX_FLAGS_RELEASE_INIT "${CMAKE_CXX_FLAGS_RELEASE_INIT} /Ot /Oi /Ob2 /GS-")
		if(${CMAKE_CXX_COMPILER_ARCHITECTURE_ID} STREQUAL x64)
			set(CMAKE_CXX_LINK_LIBRARY_FLAG "${CMAKE_CXX_LINK_LIBRARY_FLAG} /MACHINE:X64")
		endif()
	endif()

	if(${MSVC_TOOLSET_VERSION} STREQUAL 141)
		set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /Qpar /Gy /Gw /FS /Zc:threadSafeInit-")
		set(CMAKE_CXX_FLAGS_RELEASE_INIT "${CMAKE_CXX_FLAGS_RELEASE_INIT} /Ot /Oi /Ob2 /GS-")
	endif()

	if(${MSVC_TOOLSET_VERSION} STREQUAL 142)
		set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /Qpar /Gy /Gw /FS /Zc:threadSafeInit-")
		set(CMAKE_CXX_FLAGS_RELEASE_INIT "${CMAKE_CXX_FLAGS_RELEASE_INIT} /Ot /Oi /Ob2 /GS-")
	endif()

	if(${MSVC_TOOLSET_VERSION} STREQUAL 143)
		set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /Qpar /Gy /Gw /FS /Zc:threadSafeInit-")
		set(CMAKE_CXX_FLAGS_RELEASE_INIT "${CMAKE_CXX_FLAGS_RELEASE_INIT} /Ot /Oi /Ob2 /GS-")
	endif()

endif()


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(AUXINCLUDEDIR "AuxInclude/${TARGETNAME}/GeneratedFiles")
set(AUXINCLUDEPATH "${PROJECT_SOURCE_DIR}/../../../${AUXINCLUDEDIR}")
#set(ACF_TRANSLATIONS_OUTDIR "${AUXINCLUDEPATH}/${TARGETNAME}")

find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS Core Widgets Core Gui Xml Network Svg Concurrent REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/../../")

include_directories("${INCLUDE_DIR}")
include_directories("${IMPL_DIR}")


function(acf_create_moc outfiles)
	if(QT_VERSION_MAJOR EQUAL 5)
		qt5_wrap_cpp("${outfiles}" ${ARGN} )
	elseif(QT_VERSION_MAJOR EQUAL 6)
		qt6_wrap_cpp("${outfiles}" ${ARGN} )
	endif()
	set("${outfiles}" "${${outfiles}}" PARENT_SCOPE)
endfunction()

function(acf_wrap_resources outfiles)
	if(QT_VERSION_MAJOR EQUAL 5)
		qt5_add_resources("${outfiles}" ${ARGN})
	elseif(QT_VERSION_MAJOR EQUAL 6)
		qt6_add_resources("${outfiles}" ${ARGN})
	endif()
	set("${outfiles}" "${${outfiles}}" PARENT_SCOPE)
endfunction()

function(acf_wrap_ui outfiles)
	if(QT_VERSION_MAJOR EQUAL 5)
		qt5_wrap_ui("${outfiles}" ${ARGN})
	elseif(QT_VERSION_MAJOR EQUAL 6)
		qt6_wrap_ui("${outfiles}" ${ARGN})
	endif()
	set("${outfiles}" "${${outfiles}}" PARENT_SCOPE)
endfunction()

if(${MSVC})
	link_libraries("Mpr.lib")
endif()

function(acf_use_qt_base_modules)
	target_link_libraries(${PROJECT_NAME}  Qt${QT_VERSION_MAJOR}::Core)
	target_link_libraries(${PROJECT_NAME}  Qt${QT_VERSION_MAJOR}::Xml)
	target_link_libraries(${PROJECT_NAME}  Qt${QT_VERSION_MAJOR}::Network)
	target_link_libraries(${PROJECT_NAME}  Qt${QT_VERSION_MAJOR}::Concurrent)
endfunction()

function(acf_use_qt_graphics_modules)
	target_link_libraries(${PROJECT_NAME}  Qt${QT_VERSION_MAJOR}::Widgets)
	target_link_libraries(${PROJECT_NAME}  Qt${QT_VERSION_MAJOR}::Gui)
	target_link_libraries(${PROJECT_NAME}  Qt${QT_VERSION_MAJOR}::Svg)
endfunction()
