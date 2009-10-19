TARGET = icomp
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

HEADERS += ../CBaseComponentStaticInfo.h
HEADERS += ../CComponentAccessor.h
HEADERS += ../CComponentAddress.h
HEADERS += ../CComponentBase.h
HEADERS += ../CComponentContext.h
HEADERS += ../CComponentStaticInfoBase.h
HEADERS += ../CCompositeComponent.h
HEADERS += ../CCompositeComponentContext.h
HEADERS += ../CCompositeComponentStaticInfo.h
HEADERS += ../CCompositePackageStaticInfo.h
HEADERS += ../CFactoryAttribute.h
HEADERS += ../CInterfaceManipBase.h
HEADERS += ../CMultiFactoryAttribute.h
HEADERS += ../CMultiReferenceAttribute.h
HEADERS += ../CPackageStaticInfo.h
HEADERS += ../CReferenceAttribute.h
HEADERS += ../CRegistry.h
HEADERS += ../CRegistryElement.h
HEADERS += ../CSimComponentContextBase.h
HEADERS += ../export.h
HEADERS += ../IAttributeStaticInfo.h
HEADERS += ../icomp.h
HEADERS += ../IComponent.h
HEADERS += ../IComponentContext.h
HEADERS += ../IComponentStaticInfo.h
HEADERS += ../IRealAttributeStaticInfo.h
HEADERS += ../IRegistriesManager.h
HEADERS += ../IRegistry.h
HEADERS += ../IRegistryElement.h
HEADERS += ../IRegistryLoader.h
HEADERS += ../IRegistryLoaderProvider.h
HEADERS += ../TAttributeStaticInfo.h
HEADERS += ../TComponentRegistrator.h
HEADERS += ../TComponentStaticInfo.h
HEADERS += ../TComponentWrap.h
HEADERS += ../TFactoryMember.h
HEADERS += ../TInterfaceRegistrator.h
HEADERS += ../TMakeComponentWrap.h
HEADERS += ../TModelCompWrap.h
HEADERS += ../TMultiAttribute.h
HEADERS += ../TMultiAttributeMember.h
HEADERS += ../TMultiFactoryMember.h
HEADERS += ../TMultiReferenceMember.h
HEADERS += ../TReferenceMember.h
HEADERS += ../TSelectableAttribute.h
HEADERS += ../TSimComponentsFactory.h
HEADERS += ../TSimComponentWrap.h
HEADERS += ../TAttribute.h
HEADERS += ../TAttributeMember.h
SOURCES += ../CBaseComponentStaticInfo.cpp
SOURCES += ../CComponentAccessor.cpp
SOURCES += ../CComponentAddress.cpp
SOURCES += ../CComponentBase.cpp
SOURCES += ../CComponentContext.cpp
SOURCES += ../CComponentStaticInfoBase.cpp
SOURCES += ../CCompositeComponent.cpp
SOURCES += ../CCompositeComponentContext.cpp
SOURCES += ../CCompositeComponentStaticInfo.cpp
SOURCES += ../CCompositePackageStaticInfo.cpp
SOURCES += ../CInterfaceManipBase.cpp
SOURCES += ../CPackageStaticInfo.cpp
SOURCES += ../CRegistry.cpp
SOURCES += ../CRegistryElement.cpp
SOURCES += ../CSimComponentContextBase.cpp
