project(Acf LANGUAGES CXX)

set(ARX_ENABLE_GENERATE_DEPENDENCIES_LIST OFF)

include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)
include(${ACFDIR}/Config/CMake/AcfStdGui.cmake)

if(WIN32)
	set(listFiles \"${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}\")
	windeploy(${PROJECT_NAME} "" "${listFiles}")
	unset(listFiles)
endif()

mac_deploy_qt(
	REMOVE_DEBUGSYM
	TARGET ${PROJECT_NAME}
	TARGET_FILE_NAME ${PROJECT_NAME}
)

