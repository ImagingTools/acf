#Standard settings for an ACF shared library project

include(${ACFDIR}/Config/CMake/GeneralConfig.cmake)

include_directories(${PROJECT_SOURCE_DIR}/../../../../)

file(GLOB HEADER_FILES "${PROJECT_SOURCE_DIR}/../*.h")
file(GLOB SOURCES_FILES "${PROJECT_SOURCE_DIR}/../*.cpp")

file(GLOB PROJECT_SRC
        ${HEADER_FILES}
        ${SOURCES_FILES}
)
add_library(${PROJECT_NAME} SHARED ${PROJECT_SRC})

acf_create_moc(MOC_SOURCES "${HEADER_FILES}")
acf_wrap_resources(RESOURCES_FILES "${QRC_FILES}")
acf_wrap_ui(UI_SOURCES "${UI_FILES}")

target_sources(${PROJECT_NAME} PRIVATE ${MOC_SOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${UI_SOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${RESOURCES_FILES})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES_FILE_AUX})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ${TARGET_EXT})

set(outbindir "${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}${TARGETNAME}/Plugins")
set(LIBRARY_OUTPUT_PATH "${outbindir}")



