include(${CMAKE_CURRENT_LIST_DIR}/GeneralConfig.cmake)

file(GLOB UI_FILES "${PROJECT_SOURCE_DIR}/../*.ui")
file(GLOB HEADER_FILES "${PROJECT_SOURCE_DIR}/../*.h")
file(GLOB SOURCES_FILES "${PROJECT_SOURCE_DIR}/../*.cpp")
file(GLOB SOURCES_FILES_C "${PROJECT_SOURCE_DIR}/../*.c")
file(GLOB QRC_FILES "${PROJECT_SOURCE_DIR}/../*.qrc")

file(GLOB PROJECT_SRC
	${HEADER_FILES}
	${SOURCES_FILES}
	${SOURCES_FILES_C}
	${ADITIONAL_SOURCES_FILES}
)


add_library(${PROJECT_NAME} STATIC ${PROJECT_SRC})

acf_create_moc(MOC_SOURCES "${HEADER_FILES}")
acf_wrap_resources(RESOURCES_FILES "${QRC_FILES}")
acf_wrap_ui(UI_SOURCES "${UI_FILES}")

target_sources(${PROJECT_NAME} PRIVATE ${MOC_SOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${UI_SOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${RESOURCES_FILES})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES_FILE_AUX})

set(outlibdir "${AUX_INCLUDE_DIR}/../../../Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME}")

set_target_properties( ${PROJECT_NAME}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${outlibdir}
	LIBRARY_OUTPUT_DIRECTORY ${outlibdir}
	RUNTIME_OUTPUT_DIRECTORY ${outlibdir}
	POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Core )

