TARGET = iqt
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


INCLUDEPATH += ../../ "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QTDIR)/include/QtNetwork" 

HEADERS += ../CApplicationEnvironment.h
HEADERS += ../CApplicationSettingsProviderComp.h
HEADERS += ../CBitmap.h
HEADERS += ../CBitmapLoaderComp.h
HEADERS += ../CBitmapSurface.h
HEADERS += ../CCopyProcessorComp.h
HEADERS += ../CCriticalSection.h
HEADERS += ../CDateTime.h
HEADERS += ../CDefaultRegistryLoaderProvider.h
HEADERS += ../CDefaultServicesProvider.h
HEADERS += ../CDirList.h
HEADERS += ../CDllFunctionsProvider.h
HEADERS += ../CFileInfoCopyComp.h
HEADERS += ../CFileList.h
HEADERS += ../CFileListProviderComp.h
HEADERS += ../CFileSystem.h
HEADERS += ../CIniSettingsProviderComp.h
HEADERS += ../CNetworkWriteArchive.h
HEADERS += ../CPackagesLoaderComp.h
HEADERS += ../CReflectedBitmapBase.h
HEADERS += ../CSettingsArchiveBase.h
HEADERS += ../CSettingsReadArchive.h
HEADERS += ../CSettingsSerializerComp.h
HEADERS += ../CSettingsWriteArchive.h
HEADERS += ../CSignalBlocker.h
HEADERS += ../CTimer.h
HEADERS += ../CXmlFileReadArchive.h
HEADERS += ../CXmlFileWriteArchive.h
HEADERS += ../IQImageProvider.h
HEADERS += ../iqt.h
HEADERS += ../ISettingsProvider.h
HEADERS += ../ITranslationManager.h
SOURCES += ../CApplicationEnvironment.cpp
SOURCES += ../CApplicationSettingsProviderComp.cpp
SOURCES += ../CBitmap.cpp
SOURCES += ../CBitmapLoaderComp.cpp
SOURCES += ../CBitmapSurface.cpp
SOURCES += ../CCopyProcessorComp.cpp
SOURCES += ../CCriticalSection.cpp
SOURCES += ../CDateTime.cpp
SOURCES += ../CDefaultRegistryLoaderProvider.cpp
SOURCES += ../CDefaultServicesProvider.cpp
SOURCES += ../CDirList.cpp
SOURCES += ../CDllFunctionsProvider.cpp
SOURCES += ../CFileInfoCopyComp.cpp
SOURCES += ../CFileList.cpp
SOURCES += ../CFileListProviderComp.cpp
SOURCES += ../CFileSystem.cpp
SOURCES += ../CIniSettingsProviderComp.cpp
SOURCES += ../CNetworkWriteArchive.cpp
SOURCES += ../CPackagesLoaderComp.cpp
SOURCES += ../CReflectedBitmapBase.cpp
SOURCES += ../CSettingsArchiveBase.cpp
SOURCES += ../CSettingsReadArchive.cpp
SOURCES += ../CSettingsSerializerComp.cpp
SOURCES += ../CSettingsWriteArchive.cpp
SOURCES += ../CSignalBlocker.cpp
SOURCES += ../CTimer.cpp
SOURCES += ../CXmlFileReadArchive.cpp
SOURCES += ../CXmlFileWriteArchive.cpp
SOURCES += ../iqt.cpp
SOURCES += ../generated\moc_CNetworkWriteArchive.cpp
