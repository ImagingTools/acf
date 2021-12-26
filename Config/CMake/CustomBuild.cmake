
if(WIN32)
    set(ARX_COMPILER "Arxc.exe")
    set(ACF_TOOL "Acf.exe")
    set(QMAKE_RCC "rcc.exe")
    set(COPY_FILE "copy")
	set(QMAKE_LRELEASE "lrelease.exe")
else()
    set(ARX_COMPILER "Arxc")
    set(ACF_TOOL "Acf")
    set(QMAKE_RCC "rcc")
    set(COPY_FILE "cp")
	set(QMAKE_LRELEASE "lrelease")
endif()

set(PROJECT_BINARY_DIR ${AUX_INCLUDE_DIR}/${PROJECT_NAME})
message("PROJECT_BINARY_DIR " ${PROJECT_BINARY_DIR})

set(ACFTOOLS "$ENV{ACFDIR}/../AcfTools")

set(ARXCBIN "${ACFDIR}/Bin/${CMAKE_BUILD_TYPE}${TARGETNAME}/${ARX_COMPILER}")
set(ACFBIN "${ACFDIR}/Bin/${CMAKE_BUILD_TYPE}${TARGETNAME}/${ACF_TOOL}")

get_filename_component(ARXC_FILES_NAME "${ARXC_FILES}" NAME_WE)

set(ARXC_OUTFILE_NAME C${ARXC_FILES_NAME}.cpp)

# collecting additional deps for ARX
set(ARX_DEPS_LIST)
if(ARXC_CONFIG AND ARX_GENERATE_DEPENDENCIES_LIST)
	set(ARX_DEPS_FILE_PATH "${AUX_INCLUDE_DIR}/${PROJECT_NAME}/ArxDependsList.txt")
	set(ARX_ERRORS_FILE_PATH "${AUX_INCLUDE_DIR}/${PROJECT_NAME}/ArxDependsList_errors.txt")

	message("Collectiong ARX dependences for ${PROJECT_NAME}")
	message("Executing: ${ARXCBIN} ${ARXC_FILES} -mode depends -config ${ARXC_CONFIG} -conf_name ${TARGETNAME} -env_vars ${ENV_VARS}")

	execute_process(
		COMMAND
			${ARXCBIN} ${ARXC_FILES} -mode depends -config ${ARXC_CONFIG} -conf_name ${TARGETNAME} -env_vars ${ENV_VARS}
		OUTPUT_FILE
			${ARX_DEPS_FILE_PATH}
		ERROR_FILE
			${ARX_ERRORS_FILE_PATH}
		RESULT_VARIABLE
			ARX_DEPS_GENERATION_RESULT_CODE
	)

	if(NOT ARX_DEPS_GENERATION_RESULT_CODE EQUAL 0)
		message(FATAL_ERROR "Cannot to create dependens [${ARX_DEPS_GENERATION_RESULT_CODE}] see '${ARX_DEPS_FILE_PATH}' and ${ARX_ERRORS_FILE_PATH}' files for detales")
	endif()

	file(STRINGS ${ARX_DEPS_FILE_PATH} ARX_DEPS_LIST)
	message("${PROJECT_NAME} ARX deps detected:-----------------------------")
	message("${ARX_DEPS_LIST}")
	message("-----------------------------")
endif()

add_custom_command(OUTPUT ${ARXC_OUTFILE_NAME}
	COMMAND ${ARXCBIN}
	ARGS ${ARXC_FILES} -o ${ARXC_OUTFILE_NAME} -config ${ARXC_CONFIG} -conf_name ${TARGETNAME} -env_vars ${ENV_VARS} -v
	DEPENDS ${ARXCBIN} ${ARXC_FILES} ${ARX_DEPS_LIST} VERBATIM)

set(HEADER_FILE_AUX "${AUX_INCLUDE_DIR}/${PROJECT_NAME}/C${ARXC_FILES_NAME}.h")
set(SOURCES_FILE_AUX "${AUX_INCLUDE_DIR}/${PROJECT_NAME}/C${ARXC_FILES_NAME}.cpp")


if(WIN32)
	if(ACF_CONVERT_FILES)
		set(RC_FILE "${AUX_INCLUDE_DIR}/${PROJECT_NAME}/${PROJECT_NAME}.rc")
		set(RC_COMPILE_FILE "${AUX_INCLUDE_DIR}/${PROJECT_NAME}/${PROJECT_NAME}.res")

		add_custom_command(
			OUTPUT ${RC_FILE}
			COMMAND ${ACFBIN}
			ARGS ${ACF_CONVERT_REGISTRY} -console -config ${ACF_CONVERT_CONFIG} -input ${ACF_CONVERT_FILES} -o ${RC_FILE} -env_vars ${ENV_VARS}
			COMMENT "Start convert ${PROJECT_NAME}.rc"
			DEPENDS ${ACF_CONVERT_FILES} ${ARX_DEPS_LIST} VERBATIM)
		add_custom_target(CONVERT_FILES${PROJECT_NAME} ALL DEPENDS ${RC_FILE})
		add_custom_command(
			OUTPUT ${RC_COMPILE_FILE}
			COMMAND ${CMAKE_RC_COMPILER} ${RC_FILE}
			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
			COMMENT "Start ${PROJECT_NAME}.rc compiler"
			DEPENDS ${RC_FILE} ${ARX_DEPS_LIST} VERBATIM)
		add_custom_target(COMPILE_RES${PROJECT_NAME} ALL DEPENDS ${RC_COMPILE_FILE})

	endif()
endif()


if(ACF_TRANSLATIONS)
	add_custom_command(OUTPUT ${TRANSLATION_OUTPUT_FILE}
		COMMAND ${QMAKE_LRELEASE}
		ARGS ${ACF_TRANSLATIONS} -qm ${TRANSLATION_OUTPUT_FILE}
		DEPENDS ${ACF_TRANSLATIONS})
	add_custom_target(LRELEASE${PROJECT_NAME} ALL DEPENDS ${TRANSLATION_OUTPUT_FILE})
endif()

set_property(SOURCE ${SOURCES_FILE_AUX} PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE ${HEADER_FILE_AUX} PROPERTY SKIP_AUTOMOC ON)

