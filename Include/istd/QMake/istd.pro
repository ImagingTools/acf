include(../../../Config/QMake/Include.config)

TARGET = istd

win32:UpdateSvnVersion.commands = "UpdateVersion.bat"
QMAKE_EXTRA_TARGETS += UpdateSvnVersion

UpdateSvnVersionHook.depends = UpdateSvnVersion
CONFIG(debug,debug|release):UpdateSvnVersionHook.target = Makefile.Debug
CONFIG(release,debug|release):UpdateSvnVersionHook.target = Makefile.Release
QMAKE_EXTRA_TARGETS += UpdateSvnVersionHook

HEADERS += ../Generated/AcfVersion.h
