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


# custom build for ACF Registry Compiler (Arxc)

arxCompiler.name = ARX-Compiler
arxCompiler.CONFIG += no_link target_predeps
arxCompiler.output = $${ARXC_OUTDIR}/C${QMAKE_FILE_BASE}.cpp $${ARXC_OUTDIR}/C${QMAKE_FILE_BASE}.h
CONFIG(debug, debug|release){
	arxCompiler.commands = $$PWD/../../Bin/$$COMPILER_DIR/$$ARX_COMPILER ${QMAKE_FILE_IN} -o $${ARXC_OUTDIR}/C${QMAKE_FILE_BASE}.cpp -config $${ARXC_CONFIG} -v
}
CONFIG(release, debug|release){
	arxCompiler.commands = $$PWD/../../Bin/$$COMPILER_DIR/$$ARX_COMPILER ${QMAKE_FILE_IN} -o $${ARXC_OUTDIR}/C${QMAKE_FILE_BASE}.cpp -config $${ARXC_CONFIG}
}
arxCompiler.input = ARXC_FILES
arxCompiler.variable_out = SOURCES
arxCompiler.dependency_type = TYPE_C
arxCompiler.depends += $$PWD/../../Bin/$$COMPILER_DIR/$$ARX_COMPILER
arxCompiler.depend_command = $$PWD/../../Bin/$$COMPILER_DIR/$$ARX_COMPILER ${QMAKE_FILE_IN} -mode depends -config $${ARXC_CONFIG}
QMAKE_EXTRA_COMPILERS += arxCompiler


# custom build for ACF transformations

acfFileConvertCopy.name = ACF-FileConvertCopy
acfFileConvertCopy.CONFIG += no_link target_predeps
acfFileConvertCopy.output = $${ACF_CONVERT_OUTDIR}/${QMAKE_FILE_BASE}
acfFileConvertCopy.commands = $$PWD/../../Bin/$$COMPILER_DIR/$$ACF_TOOL $${ACF_CONVERT_REGISTRY} -config $${ACF_CONVERT_CONFIG} -input ${QMAKE_FILE_IN} -o $${ACF_CONVERT_OUTDIR}/${QMAKE_FILE_BASE}
acfFileConvertCopy.depends = $$PWD/../../Bin/$$COMPILER_DIR/$$ACF_TOOL $$PWD/../../Bin/$$COMPILER_DIR/$$ARX_COMPILER
acfFileConvertCopy.depend_command = $$PWD/../../Bin/$$COMPILER_DIR/$$ARX_COMPILER $${ACF_CONVERT_REGISTRY} -mode depends -config $${ACF_CONVERT_CONFIG}
acfFileConvertCopy.input = ACF_CONVERT_FILES
acfFileConvertCopy.variable_out =
QMAKE_EXTRA_COMPILERS += acfFileConvertCopy


# custom build for ACF conform translations

isEmpty(QMAKE_LRELEASE) {
	QMAKE_LRELEASE = $$QT_INSTALL_BINS/lrelease
}
updateqm.input = ACF_TRANSLATIONS
updateqm.output = $${ACF_TRANSLATIONS_OUTDIR}/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$[QT_INSTALL_BINS]$$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm $${ACF_TRANSLATIONS_OUTDIR}/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps
updateqm.dependency_type = TYPE_QRC
QMAKE_EXTRA_COMPILERS += updateqm


# custom build for Qt resources containing generated files
greaterThan(QT_MAJOR_VERSION, 4):GENERATE_RESOURCE_COMMANDS = $$COPY_FILE $$system_quote($$system_path($$OUT_PWD/../${QMAKE_FILE_BASE}.qrc)) $$system_quote($$system_path($$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/${QMAKE_FILE_BASE}.qrc)) && $$system_quote($$system_path($$[QT_INSTALL_BINS]/$$QMAKE_RCC)) -name ${QMAKE_FILE_BASE} $$system_quote($$system_path($$AUXINCLUDEPATH/GeneratedFiles/${QMAKE_FILE_BASE}.qrc)) -o $$system_quote($$system_path($${ACF_TRANSLATIONS_OUTDIR}//qrc_${QMAKE_FILE_BASE}.cpp))
else{	
	GENERATE_RESOURCE_COMMANDS = $$COPY_FILE $$OUT_PWD/../${QMAKE_FILE_BASE}.qrc $$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/${QMAKE_FILE_BASE}.qrc && $$[QT_INSTALL_BINS]/$$QMAKE_RCC -name ${QMAKE_FILE_BASE} $$AUXINCLUDEPATH/GeneratedFiles/${QMAKE_FILE_BASE}.qrc -o $${ACF_TRANSLATIONS_OUTDIR}//qrc_${QMAKE_FILE_BASE}.cpp
	win32{
		GENERATE_RESOURCE_COMMANDS ~= s,/,\\,g
		message($$GENERATE_RESOURCE_COMMANDS)
	}
}


generatedResources.name = Generated Resources Compiler
generatedResources.CONFIG += no_link target_predeps
generatedResources.output = $${ACF_TRANSLATIONS_OUTDIR}/qrc_${QMAKE_FILE_BASE}.cpp
generatedResources.commands = $$GENERATE_RESOURCE_COMMANDS

generatedResources.input = GENERATED_RESOURCES
generatedResources.variable_out = SOURCES
generatedResources.dependency_type = TYPE_C
generatedResources.depends += $$[QT_INSTALL_BINS]/$$QMAKE_RCC
QMAKE_EXTRA_COMPILERS += generatedResources
