#include "CQtNemoPackage.h"



CQtNemoPackage::CQtNemoPackage()
	:acf::ComponentLibrary()
{
	acf::ComponentLibraryInfo info;
	info.setLibName("Qt Nemo");
	setLibraryInfo(info);
}


EXPORT_COMPONENT_LIBRARY(CQtNemoPackage);