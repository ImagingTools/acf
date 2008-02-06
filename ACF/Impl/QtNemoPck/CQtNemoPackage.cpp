#include "CQtNemoPackage.h"

#include "iqtnemo/CNemoSystemOverviewGuiComp.h"
#include "iqtnemo/CNemoSensorTreeGuiComp.h"
#include "iqtnemo/CNemoSensorsModelComp.h"


CQtNemoPackage::CQtNemoPackage()
	:acf::ComponentLibrary()
{
	using namespace iqtnemo;

	acf::ComponentLibraryInfo info;
	info.setLibName("Qt Nemo");
	setLibraryInfo(info);

	registerComponent<CNemoSystemOverviewGuiComp>("NemoSystemOverviewGui");
	registerComponent<CNemoSensorTreeGuiComp>("NemoSensorTreeGui");
	registerComponent<CNemoSensorsModelComp>("NemoSensorsModel");
}


EXPORT_COMPONENT_LIBRARY(CQtNemoPackage);