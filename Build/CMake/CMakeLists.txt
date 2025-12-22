cmake_minimum_required(VERSION 3.26)

message(VERBOSE ${CMAKE_CURRENT_LIST_DIR})

project(Acf)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core)
find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS Core Widgets Core Gui Xml Network Svg Concurrent REQUIRED)

set(ACFDIR "${CMAKE_CURRENT_LIST_DIR}/../../../Acf")

include("${ACFDIR}/Config/CMake/ProjectRoot.cmake")

set(INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../Include")
set(IMPL_DIR "${CMAKE_CURRENT_LIST_DIR}/../../Impl")

set(BUILD_TESTING ON CACHE BOOL "Configures projects for testing")

message(${INCLUDE_DIR})

subdirlist(INCLUDE_DIRS ${INCLUDE_DIR})
subdirlist(IMPL_DIRS ${IMPL_DIR})

get_target_name(TARGETNAME)

if(NOT DEFINED BUILDDIR)
	set(BUILDDIR "${CMAKE_CURRENT_LIST_DIR}/../../../")
endif()

set(AUX_INCLUDE_DIR "${BUILDDIR}/Acf/AuxInclude/${TARGETNAME}/GeneratedFiles")
set(ACFBINDIR "${BUILDDIR}/Acf/Bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}")

include_directories(${AUX_INCLUDE_DIR}/..)

# Include

add_subdirectory("${INCLUDE_DIR}/istd/CMake" "${AUX_INCLUDE_DIR}/istd")
add_subdirectory("${INCLUDE_DIR}/iser/CMake" "${AUX_INCLUDE_DIR}/iser")
add_subdirectory("${INCLUDE_DIR}/ilog/CMake" "${AUX_INCLUDE_DIR}/ilog")
add_subdirectory("${INCLUDE_DIR}/i2d/CMake" "${AUX_INCLUDE_DIR}/i2d")
add_subdirectory("${INCLUDE_DIR}/i3d/CMake" "${AUX_INCLUDE_DIR}/i3d")
#
add_subdirectory("${INCLUDE_DIR}/iattr/CMake" "${AUX_INCLUDE_DIR}/iattr")
add_subdirectory("${INCLUDE_DIR}/iimg/CMake" "${AUX_INCLUDE_DIR}/iimg")
add_subdirectory("${INCLUDE_DIR}/imath/CMake" "${AUX_INCLUDE_DIR}/imath")
add_subdirectory("${INCLUDE_DIR}/icmm/CMake" "${AUX_INCLUDE_DIR}/icmm")
add_subdirectory("${INCLUDE_DIR}/imod/CMake" "${AUX_INCLUDE_DIR}/imod")
add_subdirectory("${INCLUDE_DIR}/icomp/CMake" "${AUX_INCLUDE_DIR}/icomp")

add_subdirectory("${INCLUDE_DIR}/idoc/CMake" "${AUX_INCLUDE_DIR}/idoc")
add_subdirectory("${INCLUDE_DIR}/iprm/CMake" "${AUX_INCLUDE_DIR}/iprm")
add_subdirectory("${INCLUDE_DIR}/ibase/CMake" "${AUX_INCLUDE_DIR}/ibase")
add_subdirectory("${INCLUDE_DIR}/iqt/CMake" "${AUX_INCLUDE_DIR}/iqt")
add_subdirectory("${INCLUDE_DIR}/ifile/CMake" "${AUX_INCLUDE_DIR}/ifile")
add_subdirectory("${INCLUDE_DIR}/iqt2d/CMake" "${AUX_INCLUDE_DIR}/iqt2d")
add_subdirectory("${INCLUDE_DIR}/iview/CMake" "${AUX_INCLUDE_DIR}/iview")
add_subdirectory("${INCLUDE_DIR}/iqtdoc/CMake" "${AUX_INCLUDE_DIR}/iqtdoc")
add_subdirectory("${INCLUDE_DIR}/iwidgets/CMake" "${AUX_INCLUDE_DIR}/iwidgets")
add_subdirectory("${INCLUDE_DIR}/iqtgui/CMake" "${AUX_INCLUDE_DIR}/iqtgui")
add_subdirectory("${INCLUDE_DIR}/ifilegui/CMake" "${AUX_INCLUDE_DIR}/ifilegui")
add_subdirectory("${INCLUDE_DIR}/iloggui/CMake" "${AUX_INCLUDE_DIR}/iloggui")
add_subdirectory("${INCLUDE_DIR}/iqtprm/CMake" "${AUX_INCLUDE_DIR}/iqtprm")
add_subdirectory("${INCLUDE_DIR}/ipackage/CMake" "${AUX_INCLUDE_DIR}/ipackage")

add_subdirectory("${INCLUDE_DIR}/itest/CMake" "${AUX_INCLUDE_DIR}/itest")

# Impl
if(NOT ANDROID)
add_subdirectory("${IMPL_DIR}/BasePck/CMake" "${AUX_INCLUDE_DIR}/BasePck")
add_subdirectory("${IMPL_DIR}/BitmapPck/CMake" "${AUX_INCLUDE_DIR}/BitmapPck")
add_subdirectory("${IMPL_DIR}/QtPck/CMake" "${AUX_INCLUDE_DIR}/QtPck")
add_subdirectory("${IMPL_DIR}/FilePck/CMake" "${AUX_INCLUDE_DIR}/FilePck")
add_subdirectory("${IMPL_DIR}/QtGuiPck/CMake" "${AUX_INCLUDE_DIR}/QtGuiPck")
add_subdirectory("${IMPL_DIR}/QtViewPck/CMake" "${AUX_INCLUDE_DIR}/QtViewPck")
add_subdirectory("${IMPL_DIR}/PackagePck/CMake" "${AUX_INCLUDE_DIR}/PackagePck")
add_subdirectory("${IMPL_DIR}/FileGuiPck/CMake" "${AUX_INCLUDE_DIR}/FileGuiPck")
add_subdirectory("${IMPL_DIR}/ArxcExe/CMake" "${AUX_INCLUDE_DIR}/ArxcExe")
add_subdirectory("${IMPL_DIR}/AcfExe/CMake" "${AUX_INCLUDE_DIR}/AcfExe")
add_dependencies(Acf Arxc)
add_subdirectory("${IMPL_DIR}/AcfLoc/CMake" "${AUX_INCLUDE_DIR}/AcfLoc")
add_dependencies(AcfLoc Acf)
add_subdirectory("${IMPL_DIR}/Tests/ComposedGuiTest/CMake" "${AUX_INCLUDE_DIR}/ComposedGuiTest")
add_dependencies(ComposedGuiTest Acf)
endif()

if (BUILD_TESTING)
	add_subdirectory("${INCLUDE_DIR}/ibase/Test/CMake" "${AUX_INCLUDE_DIR}/ibase/Test")
	add_dependencies(ibaseTest ibase)
	add_subdirectory("${INCLUDE_DIR}/iprm/Test/CMake" "${AUX_INCLUDE_DIR}/iprm/Test")
	add_dependencies(iprmTest iprm)
	add_subdirectory("${INCLUDE_DIR}/iser/Test/CMake" "${AUX_INCLUDE_DIR}/iser/Test")
	add_dependencies(iserTest iser)
	add_subdirectory("${INCLUDE_DIR}/ifile/Test/CMake" "${AUX_INCLUDE_DIR}/ifile/Test")
	add_dependencies(ifileTest ifile)
	add_subdirectory("${INCLUDE_DIR}/icmm/Test/CMake" "${AUX_INCLUDE_DIR}/icmm/Test")
	add_dependencies(icmmTest icmm iser)
	add_subdirectory("${INCLUDE_DIR}/i2d/Test/CMake" "${AUX_INCLUDE_DIR}/i2d/Test")
	add_dependencies(i2dTest i2d imath istd iser)
	add_subdirectory("../../Tests/SelectionParamIntegrationTest/CMake" "${AUX_INCLUDE_DIR}/Tests/SelectionParamIntegrationTest")
	add_dependencies(SelectionParamIntegrationTest iprm iser itest)
	add_subdirectory("../../Tests/ParamsManagerTest/CMake" "${AUX_INCLUDE_DIR}/Tests/ParamsManagerTest")
	add_dependencies(ParamsManagerTest iprm iser itest)
endif()
