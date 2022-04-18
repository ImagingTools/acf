include(${CMAKE_CURRENT_LIST_DIR}/GeneralConfig.cmake)

include_directories(${PROJECT_SOURCE_DIR}/../../../)

file(GLOB HEADER_FILES "${PROJECT_SOURCE_DIR}/../*.h")
file(GLOB SOURCES_FILES "${PROJECT_SOURCE_DIR}/../*.cpp")

file(GLOB PROJECT_SRC
	${HEADER_FILES}
	${SOURCES_FILES}
)


add_library(${PROJECT_NAME} SHARED ${PROJECT_SRC})

set(TARGET_EXT ".arp")
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ${TARGET_EXT})


set(outlibdir "${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}")

set_target_properties( ${PROJECT_NAME}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${outlibdir}
	LIBRARY_OUTPUT_DIRECTORY ${outlibdir}
	RUNTIME_OUTPUT_DIRECTORY ${outlibdir}
	POSITION_INDEPENDENT_CODE ON
)

acf_use_qt_modules()

