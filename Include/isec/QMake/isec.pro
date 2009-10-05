TARGET = isec
TEMPLATE = lib
CONFIG += staticlib

CONFIG(debug, debug|release) {
	DESTDIR = ..\..\..\Lib\DebugQMake
}
CONFIG(release, debug|release) {
	DESTDIR = ..\..\..\Lib\ReleaseQMake
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += 


INCLUDEPATH += ../../ 

HEADERS += ../CComposedAuthorizationVerifierComp.h
HEADERS += ../CFixedAuthorisationVerifierComp.h
HEADERS += ../CStaticUserLoginComp.h
HEADERS += ../IAuthorizationVerifier.h
HEADERS += ../isec.h
HEADERS += ../IUserLogin.h
SOURCES += ../CComposedAuthorizationVerifierComp.cpp
SOURCES += ../CFixedAuthorisationVerifierComp.cpp
SOURCES += ../CStaticUserLoginComp.cpp
