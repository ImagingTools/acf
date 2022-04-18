project(Acf LANGUAGES CXX)

set(ARX_DISABLE_GENERATE_DEPENDENCIES_LIST ON)

include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)

if(WIN32)
	set(listFiles \"${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}\")
	windeploy(${PROJECT_NAME} "" "${listFiles}")
	unset(listOptions)
endif()