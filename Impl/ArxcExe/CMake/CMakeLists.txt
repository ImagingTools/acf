include(${ACFDIR}/Config/CMake/WindeployQt.cmake)

project(Arxc LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)

if(WIN32)
	set(listFiles \"${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}${TARGETNAME}\")

	windeploy(${PROJECT_NAME} "" "${listFiles}")
	unset(listOptions)
endif()
