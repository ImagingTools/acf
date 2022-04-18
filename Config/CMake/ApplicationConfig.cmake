include(${CMAKE_CURRENT_LIST_DIR}/GeneralConfig.cmake)

file(GLOB UI_FILES "${PROJECT_SOURCE_DIR}/../*.ui")
file(GLOB HEADER_FILES "${PROJECT_SOURCE_DIR}/../*.h")
file(GLOB SOURCES_FILES "${PROJECT_SOURCE_DIR}/../*.cpp")
file(GLOB QRC_FILES "${PROJECT_SOURCE_DIR}/../*.qrc")

file(GLOB PROJECT_SRC
	${HEADER_FILES}
	${SOURCES_FILES}
)

if(ANDROID)
	add_library(${PROJECT_NAME} SHARED ${PROJECT_SRC})
else()
	add_executable(${PROJECT_NAME} ${PROJECT_SRC})
endif()

acf_create_moc(MOC_SOURCES "${HEADER_FILES}")
acf_wrap_resources(RESOURCES_FILES "${QRC_FILES}")
acf_wrap_ui(UI_SOURCES "${UI_FILES}")

target_sources(${PROJECT_NAME} PRIVATE ${MOC_SOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${UI_SOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${RESOURCES_FILES})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES_FILE_AUX})

set_property(
	TARGET ${PROJECT_NAME}
	PROPERTY POSITION_INDEPENDENT_CODE ON)

if(NOT ANDROID)
	set(outbindir "${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}")
	set_property(
		TARGET ${PROJECT_NAME}
		PROPERTY RUNTIME_OUTPUT_DIRECTORY ${outbindir})
endif()

if(ACF_CONVERT_FILES)
	set_target_properties( ${PROJECT_NAME} PROPERTIES LINK_FLAGS ${RC_COMPILE_FILE} )
endif()

acf_use_qt_modules()
