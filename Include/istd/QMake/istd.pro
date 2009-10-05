TARGET = istd
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

HEADERS += ../CBitManip.h
HEADERS += ../CChangeDelegator.h
HEADERS += ../CClassInfo.h
HEADERS += ../CCrcCalculator.h
HEADERS += ../CFastBinaryIndex.h
HEADERS += ../CIndex2d.h
HEADERS += ../CRandomNumber.h
HEADERS += ../CRange.h
HEADERS += ../CStaticServicesProvider.h
HEADERS += ../CString.h
HEADERS += ../CVarIndex.h
HEADERS += ../IChangeable.h
HEADERS += ../IContainerInfo.h
HEADERS += ../IDuplicatable.h
HEADERS += ../IEnableable.h
HEADERS += ../IFactoryInfo.h
HEADERS += ../IFileSystemObserver.h
HEADERS += ../ILogger.h
HEADERS += ../INamed.h
HEADERS += ../IPolymorphic.h
HEADERS += ../IServicesProvider.h
HEADERS += ../istd.h
HEADERS += ../ITrace.h
HEADERS += ../TArray.h
HEADERS += ../TCachedUpdateManagerWrap.h
HEADERS += ../TCascadedMap.h
HEADERS += ../TChangeBinder.h
HEADERS += ../TChangeDelegator.h
HEADERS += ../TChangeNotifier.h
HEADERS += ../TComposedFactory.h
HEADERS += ../TDelPtr.h
HEADERS += ../TIFactory.h
HEADERS += ../TIHierarchical.h
HEADERS += ../TIndex.h
HEADERS += ../TOptDelPtr.h
HEADERS += ../TOptPointerVector.h
HEADERS += ../TPointer.h
HEADERS += ../TPointerVector.h
HEADERS += ../TRetSmartPtr.h
HEADERS += ../TSingleFactory.h
HEADERS += ../TSmartPtr.h
HEADERS += ../TUpdateManagerWrap.h
HEADERS += ../TVarArray.h
HEADERS += ../AcfVersion.h.xtrsvn
HEADERS += ../generated\AcfVersion.h
SOURCES += ../CBitManip.cpp
SOURCES += ../CChangeDelegator.cpp
SOURCES += ../CClassInfo.cpp
SOURCES += ../CCrcCalculator.cpp
SOURCES += ../CRandomNumber.cpp
SOURCES += ../CRange.cpp
SOURCES += ../CStaticServicesProvider.cpp
SOURCES += ../CString.cpp
SOURCES += ../CVarIndex.cpp
SOURCES += ../istd.cpp
