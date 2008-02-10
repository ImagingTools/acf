#include "CQtNemoPackage.h"

#include "iqtnemo/CNemoSystemOverviewGuiComp.h"
#include "iqtnemo/CNemoSensorTreeGuiComp.h"
#include "iqtnemo/CNemoSystemModelComp.h"
#include "iqtnemo/CNemoSensorPlotGuiComp.h"


CQtNemoPackage::CQtNemoPackage()
	:acf::ComponentLibrary()
{
	using namespace iqtnemo;

	acf::ComponentLibraryInfo info;
	info.setLibName("Qt Nemo");
	setLibraryInfo(info);

	registerComponent<CNemoSystemOverviewGuiComp>("NemoSystemOverviewGui");
	registerComponent<CNemoSensorTreeGuiComp>("NemoSensorTreeGui");
	registerComponent<CNemoSystemModelComp>("NemoSystemModel");
	registerComponent<CNemoSensorPlotGuiComp>("NemoSensorPlotGui");
}


EXPORT_COMPONENT_LIBRARY(CQtNemoPackage);