#include "CQtPackage.h"

#include "iqt/CTabContainerGuiComp.h"
#include "iqt/CLoginGuiComp.h"


CQtPackage::CQtPackage()
:	BaseClass()
{
	acf::ComponentLibraryInfo info;
	info.setLibName("Qt");
	setLibraryInfo(info);

	registerComponent<iqt::CTabContainerGuiComp>("TabContainerGui");
	registerComponent<iqt::CLoginGuiComp>("LoginGui");
}


EXPORT_COMPONENT_LIBRARY(CQtPackage);