#include "CNemoPackage.h"

#include "inemo/CNemoSystemModelComp.h"


CNemoPackage::CNemoPackage()
	:acf::ComponentLibrary()
{

	using namespace inemo;

	acf::ComponentLibraryInfo info;
	info.setLibName("Nemo");
	setLibraryInfo(info);

	registerComponent<CNemoSystemModelComp>("NemoSystemModel");
}


EXPORT_COMPONENT_LIBRARY(CNemoPackage);