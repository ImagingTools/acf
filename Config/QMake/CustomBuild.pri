#General ACF settings

win32{
	ARX_COMPILER = Arxc.exe
	ACF_TOOL = Acf.exe
}
else{
	ARX_COMPILER = Arxc
	ACF_TOOL = Acf
}


# custom build for Subversion Transformations

win*{
	svnTransformation.name = SVN-Rev-Transformation
	svnTransformation.CONFIG += target_predeps
	svnTransformation.output = ${QMAKE_FILE_IN_PATH}/Generated/${QMAKE_FILE_BASE}
	svnTransformation.commands = SubWCRev.exe $${SVNTR_REPOSITORY} ${QMAKE_FILE_NAME} $${SVNTR_DESTINATION}/${QMAKE_FILE_BASE}
	svnTransformation.input = SVNTR_FILES
	QMAKE_EXTRA_COMPILERS += svnTransformation
}


# custom build for ACF Registry Compiler (Arxc)

arxCompiler.name = ARX-Compiler
arxCompiler.CONFIG += no_link target_predeps
arxCompiler.output = ${QMAKE_FILE_IN_PATH}/Generated/C${QMAKE_FILE_BASE}.cpp ${QMAKE_FILE_IN_PATH}/Generated/C${QMAKE_FILE_BASE}.h
CONFIG(debug, debug|release){
	arxCompiler.commands = $$PWD/../../Bin/$$COMPILER_DIR/$$ARX_COMPILER ${QMAKE_FILE_IN} -o ${QMAKE_FILE_IN_PATH}/Generated/C${QMAKE_FILE_BASE}.cpp -config $${ARXC_CONFIG} -v
}
CONFIG(release, debug|release){
	arxCompiler.commands = $$PWD/../../Bin/$$COMPILER_DIR/$$ARX_COMPILER ${QMAKE_FILE_IN} -o ${QMAKE_FILE_IN_PATH}/Generated/C${QMAKE_FILE_BASE}.cpp -config $${ARXC_CONFIG}
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
acfFileConvertCopy.depends = $$PWD/../../Bin/$$COMPILER_DIR/$$ACF_TOOL
acfFileConvertCopy.input = ACF_CONVERT_FILES
acfFileConvertCopy.variable_out =
QMAKE_EXTRA_COMPILERS += acfFileConvertCopy


# custom build for ACF conform translations

isEmpty(QMAKE_LRELEASE) {
	QMAKE_LRELEASE = $$QT_INSTALL_BINS/lrelease
}
updateqm.input = ACF_TRANSLATIONS
updateqm.output = ${QMAKE_FILE_IN_PATH}/Generated/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$[QT_INSTALL_BINS]$$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_IN_PATH}/Generated/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps
updateqm.dependency_type = TYPE_QRC
QMAKE_EXTRA_COMPILERS += updateqm

copyQmToGenerate.input = COPYQMTOGENERATE
copyQmToGenerate.output = ../Generated/${QMAKE_FILE_BASE}.qm
win32:copyQmToGenerate.commands = xcopy ${QMAKE_FILE_IN} ../Generated/${QMAKE_FILE_BASE}.qm
else:copyQmToGenerate.commands = cp ${QMAKE_FILE_IN} ../Generated/${QMAKE_FILE_BASE}.qm
copyQmToGenerate.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += copyQmToGenerate
