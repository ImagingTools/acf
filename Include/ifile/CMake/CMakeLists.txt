project(ifile LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Concurrent REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Concurrent)

