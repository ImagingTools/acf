project(UtilityClassesTest LANGUAGES CXX)


set(ARXC_CONFIG "")
set(ARXC_FILES "")
set(ARX_ENABLE_GENERATE_DEPENDENCIES_LIST OFF)

if(DEFINED ACFDIR)
	include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
	include(${ACFDIR}/Config/CMake/WindeployQt.cmake)

	include(${ACFDIR}/Config/CMake/AcfQt.cmake)
	include(${ACFDIR}/Config/CMake/AcfStdGui.cmake)
	include(${ACFDIR}/Config/CMake/AcfStd.cmake)
endif()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test)

# Add test source files
set(TEST_SOURCES
	${PROJECT_SOURCE_DIR}/../main.cpp
	${PROJECT_SOURCE_DIR}/../CRandomNumberTest.cpp
	${PROJECT_SOURCE_DIR}/../CDoubleTest.cpp
	${PROJECT_SOURCE_DIR}/../CIndex2dTest.cpp
	${PROJECT_SOURCE_DIR}/../CVector3dTest.cpp
	${PROJECT_SOURCE_DIR}/../CMatrix3dTest.cpp
)

set(TEST_HEADERS
	${PROJECT_SOURCE_DIR}/../CRandomNumberTest.h
	${PROJECT_SOURCE_DIR}/../CDoubleTest.h
	${PROJECT_SOURCE_DIR}/../CIndex2dTest.h
	${PROJECT_SOURCE_DIR}/../CVector3dTest.h
	${PROJECT_SOURCE_DIR}/../CMatrix3dTest.h
)

add_executable(${PROJECT_NAME} ${TEST_SOURCES} ${TEST_HEADERS})

target_link_libraries(${PROJECT_NAME}
	istd
	imath
	i3d
	i2d
	iser
	itest
	Qt${QT_VERSION_MAJOR}::Test
	Qt${QT_VERSION_MAJOR}::Core
)


if(WIN32 AND DEFINED ACFDIR)
	set(listFiles \"${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}/${PROJECT_NAME}.exe\")

	windeploy(${PROJECT_NAME} "" "${listFiles}")
endif()
