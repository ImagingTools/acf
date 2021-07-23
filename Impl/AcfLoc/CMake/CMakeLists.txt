project(AcfLoc LANGUAGES CXX)

set(ARXC_CONFIG "${ACFDIR}/Config/CMake/../AcfConfig.awc")
set(ARXC_FILES "${PROJECT_SOURCE_DIR}/../${PROJECT_NAME}.acc")

include(${ACFDIR}/Config/CMake/CustomBuild.cmake)
include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

add_dependencies(${PROJECT_NAME} Arxc Acf)
