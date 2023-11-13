project(ComposedGuiTest LANGUAGES CXX)

if(WIN32)
	set(CMAKE_WIN32_EXECUTABLE "on")
endif()

set(ARXC_CONFIG "${PROJECT_SOURCE_DIR}/../Config/ComposedGuiTest.awc")
set(ARXC_FILES "${PROJECT_SOURCE_DIR}/../${PROJECT_NAME}.acc")
set(ARX_ENABLE_GENERATE_DEPENDENCIES_LIST OFF)

include(${ACFDIR}/Config/CMake/CustomBuild.cmake)
include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/WindeployQt.cmake)

target_link_libraries(${PROJECT_NAME} AcfLoc icmpstr icomp iwidgets)

find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS PrintSupport REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::PrintSupport)

include(${ACFDIR}/Config/CMake/AcfStdGui.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)

if(WIN32)
	set(listFiles \"${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}\")

	windeploy(${PROJECT_NAME} "" "${listFiles}")
	unset(listOptions)
endif()

