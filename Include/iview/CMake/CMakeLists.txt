project(iview LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

qt5_use_modules(${PROJECT_NAME} Widgets)
