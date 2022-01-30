cmake_minimum_required(VERSION 3.1.0)

message("CMAKE_VERSION " ${CMAKE_VERSION})

if(NOT DEFINED QTDIR)
	set(QTDIR "$ENV{QTDIR}")
endif()

if(NOT DEFINED CMAKE_PREFIX_PATH)
	set(CMAKE_PREFIX_PATH "${QTDIR}")
	message("set CMAKE_PREFIX_PATH " ${CMAKE_PREFIX_PATH})
	message("Qt5Core_DIR " ${Qt5Core_DIR})
endif()


#Acf
if(NOT DEFINED ACFDIR)
	set(ACFDIR "$ENV{ACFDIR}")
endif()
if(NOT DEFINED ACFCONFIGDIR)
	set(ACFCONFIGDIR "$ENV{ACFCONFIGDIR}")
endif()
if(NOT DEFINED OPENCVDIR_4_5_3)
	set(OPENCVDIR_4_5_3 "${ACFDIR}/../3rdParty/OpenCV/4.5.3")
endif()

include("${ACFDIR}/Config/CMake/ProjectRoot.cmake")

get_target_name(TARGETNAME)

include_directories("${ACFDIR}/AuxInclude/Qt${QT_VERSION_MAJOR}_${TARGETNAME}")
include_directories("${ACFDIR}/Include")
include_directories("${ACFDIR}/Impl")

link_directories(${ACFDIR}/Lib/${CMAKE_BUILD_TYPE}${TARGETNAME})
