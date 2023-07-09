include(${CMAKE_CURRENT_LIST_DIR}/ProjectRoot.cmake)

function(acf_get_root_dir identifier_to_use)
    set(${identifier_to_use} "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../../.." PARENT_SCOPE)
endfunction()

function (acf_custom_build PROJECT_BINARY_DIR ARXC_FILES ARXC_CONFIG ACF_CONVERT_REGISTRY ACF_CONVERT_CONFIG ACF_CONVERT_FILES ACF_ENV_VARS ACF_ARX_DEPS)

	set(ACF_ENV_VARS_SINGLE_STRING)

	foreach(_var ${ACF_ENV_VARS})
		string(CONCAT ACF_ENV_VARS_SINGLE_STRING "${ACF_ENV_VARS_SINGLE_STRING}\;${_var}")
	endforeach()
	
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

	get_target_name(TARGETNAME)

	set(ACFTOOLS "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../../../AcfTools")

	set(ARXCBIN "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../../Bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}/${ARX_COMPILER}")
	set(ACFBIN "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../../Bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}/${ACF_TOOL}")

	set(ARXC_OUTFILE_NAME C${PROJECT_NAME})
	set(ARXC_OUTFILE "${PROJECT_BINARY_DIR}/Generated/${PROJECT_NAME}/${ARXC_OUTFILE_NAME}")
	file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/Generated/${PROJECT_NAME}")

	set(HEADER_FILE_AUX "${ARXC_OUTFILE}.h")
	set(SOURCES_FILE_AUX "${ARXC_OUTFILE}.cpp")
	set(ARXC_OUTFILE "${ARXC_OUTFILE}.cpp")

	# collecting additional deps for ARX
	set(ARX_DEPS_LIST)
	if(ARXC_CONFIG)
		set(ARX_DEPS_FILE_PATH "${PROJECT_BINARY_DIR}/Generated/${PROJECT_NAME}/ArxDependsList.txt")
		set(ARX_ERRORS_FILE_PATH "${PROJECT_BINARY_DIR}/Generated/${PROJECT_NAME}/ArxDependsList_errors.txt")

		message("Collecting ARX dependences for ${PROJECT_NAME}")

		execute_process(
			COMMAND
				${ARXCBIN} ${ARXC_FILES} -mode depends -config ${ARXC_CONFIG} -conf_name ${CMAKE_BUILD_TYPE}_${TARGETNAME} -env_vars ${ACF_ENV_VARS_SINGLE_STRING}
			OUTPUT_FILE
				${ARX_DEPS_FILE_PATH}
			ERROR_FILE
				${ARX_ERRORS_FILE_PATH}
			RESULT_VARIABLE
				ARX_DEPS_GENERATION_RESULT_CODE
		)

		if(NOT ARX_DEPS_GENERATION_RESULT_CODE EQUAL 0)
			message("!!! ARX Cannot to create dependens")
		        message("TARGETNAME ${TARGETNAME}")

			file(STRINGS ${ARX_DEPS_FILE_PATH} ERRORS1_ARX_DEPS_LIST)
			message("${ERRORS1_ARX_DEPS_LIST}")

			file(STRINGS ${ARX_ERRORS_FILE_PATH} ERRORS2_ARX_DEPS_LIST)
			message("${ERRORS2_ARX_DEPS_LIST}")

			message("COMMAND!!! ${ARXCBIN} ${ARXC_FILES} -mode depends -config ${ARXC_CONFIG} -conf_name ${CMAKE_BUILD_TYPE}_${TARGETNAME} -env_vars ${ACF_ENV_VARS_SINGLE_STRING}")		
			message(FATAL_ERROR "!!! ARX finished unexpected [${ARX_DEPS_GENERATION_RESULT_CODE}]")		
		endif()

		file(STRINGS ${ARX_DEPS_FILE_PATH} ARX_DEPS_LIST)
	endif()

	add_custom_command(
		OUTPUT ${ARXC_OUTFILE} ${HEADER_FILE_AUX}
		COMMAND ${ARXCBIN}
		ARGS ${ARXC_FILES} -o ${ARXC_OUTFILE} -config ${ARXC_CONFIG} -conf_name ${CMAKE_BUILD_TYPE}_${TARGETNAME} -env_vars ${ACF_ENV_VARS_SINGLE_STRING} -v
		DEPENDS ${ARXCBIN} ${ARXC_FILES} ${ARX_DEPS_LIST} VERBATIM
		COMMENT "ArxcBinStarts: ${ARXCBIN} ${ARXC_FILES} -o ${ARXC_OUTFILE} -config ${ARXC_CONFIG} -conf_name ${TARGETNAME} -env_vars ${ACF_ENV_VARS_SINGLE_STRING} -v"
		)

	target_sources(${PROJECT_NAME} PRIVATE ${SOURCES_FILE_AUX})
	target_sources(${PROJECT_NAME} PRIVATE ${HEADER_FILE_AUX})

	if(WIN32)
		if(ACF_CONVERT_FILES)
			set(RC_FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.rc")
			set(RC_COMPILE_FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.res")

			set(ALL_ACF_ARX_DEPS ${ACF_CONVERT_FILES})
			if(ACF_ARX_DEPS)
				list(APPEND ALL_ACF_ARX_DEPS ${ACF_ARX_DEPS})
			endif()

			add_custom_command(
				OUTPUT ${RC_FILE}
				COMMAND ${ACFBIN}
				ARGS ${ACF_CONVERT_REGISTRY} -console -config ${ACF_CONVERT_CONFIG} -input ${ACF_CONVERT_FILES} -o ${RC_FILE} -env_vars ${ACF_ENV_VARS_SINGLE_STRING}
				COMMENT "Start convert ${PROJECT_NAME}.rc"
				DEPENDS ${ALL_ACF_ARX_DEPS} ${ARX_DEPS_LIST} VERBATIM)
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

endfunction()

