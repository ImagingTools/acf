set(ARXC_OUTDIR "${AUX_INCLUDE_DIR}")
set(ARX_ENABLE_GENERATE_DEPENDENCIES_LIST ON)

include(${ACFDIR}/Config/CMake/CustomBuild.cmake)

set(TARGET_EXT ".plugin")

include(${ACFDIR}/Config/CMake/SharedLibraryConfig.cmake)
include(${IMTCOREDIR}/Config/CMake/ImtCore.cmake)

target_link_libraries(${PROJECT_NAME} i2d imtbase imtapp imtdb imtfile imtservice imtauth imtcol)
target_link_libraries(${PROJECT_NAME} ifile istd idoc)

target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Sql)
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Xml)

