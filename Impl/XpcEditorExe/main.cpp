// Qt includes
#include <QtCore/QCoreApplication>
#include <QtGui/QApplication>

// ACF includes
#include "ibase/IApplication.h"

#include "Generated/CXpcEditor.h"


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqtgui);
	Q_INIT_RESOURCE(icmpstr);
	Q_INIT_RESOURCE(AcfLoc);
	Q_INIT_RESOURCE(XpcEditor);

	QApplication::setStyle("plastique");

	CXpcEditor instance;

	ibase::IApplication* applicationPtr = instance.GetInterface<ibase::IApplication>();
	if (applicationPtr != NULL){
		return applicationPtr->Execute(argc, argv);
	}

	return -1;
}


