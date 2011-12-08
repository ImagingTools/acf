#General ACF settings

win32{
        ARX_COMPILER = Arxc.exe
}
else{
        ARX_COMPILER = Arxc
}

# custom build for Subversion Transformations
win32{
        svnTransformation.name = SvnTransf
        svnTransformation.CONFIG += target_predeps
        svnTransformation.output = ${QMAKE_FILE_IN_PATH}/Generated/${QMAKE_FILE_BASE}
        svnTransformation.commands = SubWCRev.exe $${SVNTR_REPOSITORY} ${QMAKE_FILE_NAME} $${SVNTR_DESTINATION}/${QMAKE_FILE_BASE}
        svnTransformation.input = SVNTR_FILES
        QMAKE_EXTRA_COMPILERS += svnTransformation
}

# custom build for ACF Registry Compiler (Arxc)
arxCompiler.name = ACF-Components
arxCompiler.CONFIG += target_predeps
arxCompiler.output = ${QMAKE_FILE_IN_PATH}/Generated/C${QMAKE_FILE_BASE}.cpp
arxCompiler.commands = $$PWD/../../Bin/$$COMPILER_DIR/$$ARX_COMPILER ${QMAKE_FILE_IN} -o ${QMAKE_FILE_IN_PATH}/Generated/C${QMAKE_FILE_BASE}.cpp -config $${ARXC_CONFIG} -v
arxCompiler.depends = $$PWD/../../Bin/$$COMPILER_DIR/$$ARX_COMPILER
arxCompiler.input = ARXC_FILES
arxCompiler.variable_out = SOURCES
QMAKE_EXTRA_COMPILERS += arxCompiler

# custom build for ACF conform translations
isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$QT_INSTALL_BINS\\lrelease.exe
    else:QMAKE_LRELEASE = $$QT_INSTALL_BINS/lrelease
}
updateqm.input = TRANSLATIONS
updateqm.output = ${QMAKE_FILE_IN_PATH}/Generated/${QMAKE_FILE_BASE}.qm
updateqm.commands = $(QTDIR)/bin/$$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_IN_PATH}/Generated/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += updateqm
PRE_TARGETDEPS += compiler_updateqm_make_all

copyQmToGenerate.input = COPYQMTOGENERATE
copyQmToGenerate.output = ../Generated/${QMAKE_FILE_BASE}.qm
copyQmToGenerate.commands = cp ${QMAKE_FILE_IN} ../Generated/${QMAKE_FILE_BASE}.qm
copyQmToGenerate.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += copyQmToGenerate
PRE_TARGETDEPS += compiler_copyQmToGenerate_make_all
