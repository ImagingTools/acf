// Qt includes
#include <QCoreApplication>
#include <QApplication>

// ACF includes
#include "ibase/IApplication.h"
#include "iqt/CDefaultServicesProvider.h"

#include "Generated/CCompositor.h"


int main(int argc, char *argv[])
{
	QApplication::setStyle("plastique");

	Q_INIT_RESOURCE(iqtgui);
	Q_INIT_RESOURCE(icmpstr);

	iqt::CDefaultServicesProvider::RegisterServices();

	CCompositor instance;

	ibase::IApplication* applicationPtr = instance.GetInterface<ibase::IApplication>();

	if (applicationPtr != NULL){
		return applicationPtr->Execute(argc, argv);
	}

	return -1;
}

