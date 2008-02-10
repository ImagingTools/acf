#include "CNemoPackage.h"


CNemoPackage::CNemoPackage()
	:acf::ComponentLibrary()
{

//	using namespace inemo;

	acf::ComponentLibraryInfo info;
	info.setLibName("Nemo");
	setLibraryInfo(info);
}


EXPORT_COMPONENT_LIBRARY(CNemoPackage);