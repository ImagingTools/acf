# -------------------------------------------------
# Project created by QtCreator 2008-11-21T16:41:01
# -------------------------------------------------
QT -= core \
    gui
TARGET = istd
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH+=$(ACFDIR)/include
SOURCES += ../istd.cpp \
    ../CVarIndex.cpp \
    ../CString.cpp \
    ../CStaticServicesProvider.cpp \
    ../CRange.cpp \
    ../CRandomNumber.cpp \
    ../CCrcCalculator.cpp \
    ../CChangeDelegator.cpp \
    ../CBitManip.cpp
HEADERS += ../TUpdateManagerWrap.h \
    ../TSmartPtr.h \
    ../TSingleFactory.h \
    ../TPointerVector.h \
    ../TPointer.h \
    ../TOptPointerVector.h \
    ../TOptDelPtr.h \
    ../TIndex.h \
    ../TIHierarchical.h \
    ../TIFactory.h \
    ../TDelPtr.h \
    ../TComposedFactory.h \
    ../TChangeNotifier.h \
    ../TChangeDelegator.h \
    ../TChangeBinder.h \
    ../TCascadedMap.h \
    ../TCachedUpdateManagerWrap.h \
    ../TArray.h \
    ../ITrace.h \
    ../istd.h \
    ../IServicesProvider.h \
    ../IPolymorphic.h \
    ../INamed.h \
    ../IEnableable.h \
    ../IDuplicatable.h \
    ../IContainerInfo.h \
    ../IChangeable.h \
    ../CVarIndex.h \
    ../CString.h \
    ../CStaticServicesProvider.h \
    ../CRange.h \
    ../CRandomNumber.h \
    ../CIndex2d.h \
    ../CFastBinaryIndex.h \
    ../CCrcCalculator.h \
    ../CChangeDelegator.h \
    ../CBitManip.h \
    ../TVarArray.h
