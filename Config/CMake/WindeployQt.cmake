find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS Core REQUIRED)

# get absolute path to qmake, then use it to find windeployqt executable

get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

message("_qt_bin_dir " ${_qt_bin_dir}/windeployqt.exe)

function(windeploy target listOptions listFiles)
    add_custom_command(TARGET ${target} POST_BUILD
		COMMAND "${Qt5_DIR}/../../../bin/windeployqt.exe"
		--verbose 1
		${listOptions}
		${listFiles}
		
		COMMENT "Deploying Qt libraries using windeployqt for compilation target '${target}' ..."
    )
	message("text:  ${options} ${files}")
endfunction()

