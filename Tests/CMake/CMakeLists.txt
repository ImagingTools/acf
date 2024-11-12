project(Tests LANGUAGES CXX)
cmake_minimum_required(VERSION 3.13.0)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../SelectionParamIntegrationTest/CMake" "${AUX_INCLUDE_DIR}/Tests/SelectionParamIntegrationTest")
