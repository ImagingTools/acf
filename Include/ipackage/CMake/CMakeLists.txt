project(ipackage LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)
