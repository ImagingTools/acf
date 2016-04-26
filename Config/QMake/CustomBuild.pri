#General ACF settings

win32{
	ARX_COMPILER = Arxc.exe
	ACF_TOOL = Acf.exe
	QMAKE_RCC = rcc.exe
	COPY_FILE = copy
}
else{
	ARX_COMPILER = Arxc
	ACF_TOOL = Acf
	QMAKE_RCC = rcc
	COPY_FILE = cp
}

# Get path to the ACF tools folder from the external variable:
ACFTOOLS = $$(ACF_TOOLS_BIN)

isEmpty(ACFTOOLS){
	# for non-cross compiling use local generated tools

	isEmpty(ACFDIRBUILD){
		# for non-shadow build use ARX compiler inside source tree

		ARXCBIN=$$PWD/../../Bin/$$COMPILER_DIR/$$ARX_COMPILER
		ACFBIN = $$PWD/../../Bin/$$COMPILER_DIR/$$ACF_TOOL
	}
	!isEmpty(ACFDIRBUILD){
		# set ARX compiler from shadow build

		ARXCBIN=$$ACFDIRBUILD/Bin/$$COMPILER_DIR/$$ARX_COMPILER
		ACFBIN=$$ACFDIRBUILD/Bin/$$COMPILER_DIR/$$ACF_TOOL
	}
}
else{
	# for cross compiling use external tools

	ARXCBIN=$$ACFTOOLS/$$ARX_COMPILER
	ACFBIN=$$ACFTOOLS/$$ACF_TOOL
}

# Correct ACF paths for windows
win32{
	ARXCBIN ~= s,/,\\,g
	ACFBIN ~= s,/,\\,g
}


# custom build for ACF Registry Compiler (Arxc)
ARX_COMPILER_OUTPUT = $${ARXC_OUTDIR}/C${QMAKE_FILE_BASE}.cpp $${ARXC_OUTDIR}/C${QMAKE_FILE_BASE}.h
ARX_COMPILER_COMMAND = $$ARXCBIN ${QMAKE_FILE_IN} -o $${ARXC_OUTDIR}/C${QMAKE_FILE_BASE}.cpp -config $${ARXC_CONFIG} -conf_name $$COMPILER_DIR

CONFIG(warn_on, warn_on|warn_off){
	ARX_COMPILER_COMMAND = $$ARX_COMPILER_COMMAND -v
}

win32{
	ARX_COMPILER_OUTPUT ~= s,/,\\,g
	ARX_COMPILER_COMMAND ~= s,/,\\,g
}

arxCompiler.name = ARX-Compiler
arxCompiler.CONFIG += no_link target_predeps
arxCompiler.output = $$ARX_COMPILER_OUTPUT
arxCompiler.commands = $$ARX_COMPILER_COMMAND
arxCompiler.input = ARXC_FILES
arxCompiler.variable_out = SOURCES
arxCompiler.dependency_type = TYPE_C
arxCompiler.depends += $$ARXCBIN
arxCompiler.depend_command = $$ARXCBIN ${QMAKE_FILE_IN} -mode depends -config $${ARXC_CONFIG} -conf_name $$COMPILER_DIR
QMAKE_EXTRA_COMPILERS += arxCompiler


# custom build for ACF transformations

ACF_CONVERT_COMMAND = $$ACFBIN $${ACF_CONVERT_REGISTRY} -config $${ACF_CONVERT_CONFIG} -input ${QMAKE_FILE_IN} -o $${ACF_CONVERT_OUTDIR}/${QMAKE_FILE_BASE}
ACF_CONVERT_OUTPUT = $${ACF_CONVERT_OUTDIR}/${QMAKE_FILE_BASE}
win32{
	ACF_CONVERT_COMMAND ~= s,/,\\,g
	ACF_CONVERT_OUTPUT ~= s,/,\\,g
}

acfFileConvertCopy.name = ACF-FileConvertCopy
acfFileConvertCopy.CONFIG += no_link target_predeps
acfFileConvertCopy.output = $$ACF_CONVERT_OUTPUT
acfFileConvertCopy.commands = $$ACF_CONVERT_COMMAND
acfFileConvertCopy.depends += $$ACFBIN
acfFileConvertCopy.depend_command = $$ARXCBIN $${ACF_CONVERT_REGISTRY} -mode depends -config $${ACF_CONVERT_CONFIG}
acfFileConvertCopy.input = ACF_CONVERT_FILES
acfFileConvertCopy.variable_out =
QMAKE_EXTRA_COMPILERS += acfFileConvertCopy


# custom build for ACF conform translations

isEmpty(QMAKE_LRELEASE){
	QMAKE_LRELEASE = $$QT_INSTALL_BINS/lrelease
}

UPDATE_QM_COMMAND = $$[QT_INSTALL_BINS]$$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm $${ACF_TRANSLATIONS_OUTDIR}/${QMAKE_FILE_BASE}.qm
TRANSLATION_OUTPUT_FILE = $${ACF_TRANSLATIONS_OUTDIR}/${QMAKE_FILE_BASE}.qm
win32{
	UPDATE_QM_COMMAND ~= s,/,\\,g
	TRANSLATION_OUTPUT_FILE ~= s,/,\\,g
}

updateqm.input = ACF_TRANSLATIONS
updateqm.output = $$TRANSLATION_OUTPUT_FILE
updateqm.commands = $$UPDATE_QM_COMMAND
updateqm.CONFIG += no_link target_predeps
updateqm.dependency_type = TYPE_QRC
QMAKE_EXTRA_COMPILERS += updateqm


# custom build for Qt resources containing generated files

GENERATE_RESOURCE_COMMANDS = $$COPY_FILE $$_PRO_FILE_PWD_/../${QMAKE_FILE_BASE}.qrc $$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/${QMAKE_FILE_BASE}.qrc && $$[QT_INSTALL_BINS]/$$QMAKE_RCC -name ${QMAKE_FILE_BASE} $$AUXINCLUDEPATH/GeneratedFiles/${QMAKE_FILE_BASE}.qrc -o $${ACF_TRANSLATIONS_OUTDIR}//qrc_${QMAKE_FILE_BASE}.cpp
GENERATED_RESOURCE_OUTPUT = $${ACF_TRANSLATIONS_OUTDIR}/qrc_${QMAKE_FILE_BASE}.cpp
win32{
	GENERATE_RESOURCE_COMMANDS ~= s,/,\\,g
	GENERATED_RESOURCE_OUTPUT ~= s,/,\\,g
}

generatedResources.name = Generated Resources Compiler
generatedResources.CONFIG += no_link target_predeps
generatedResources.output = $$GENERATED_RESOURCE_OUTPUT
generatedResources.commands = $$GENERATE_RESOURCE_COMMANDS

generatedResources.input = GENERATED_RESOURCES
generatedResources.variable_out = SOURCES
generatedResources.dependency_type = TYPE_C
generatedResources.depends += $$[QT_INSTALL_BINS]/$$QMAKE_RCC
QMAKE_EXTRA_COMPILERS += generatedResources


