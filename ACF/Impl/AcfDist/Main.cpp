#include <QApplication>

#include "CDistributeDialog.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QApplication::setStyle("cleanlooks");

	Q_INIT_RESOURCE(AcfDist);

	QCoreApplication::setOrganizationName("Imagingtools");
	QCoreApplication::setOrganizationDomain("imagingtools.de");
	QCoreApplication::setApplicationName("ACFDist");

	QFileInfo applicationInfo(app.applicationFilePath());
	QString fileName = applicationInfo.fileName();

	CDistributeDialog mainDlg(fileName);

	mainDlg.exec();
}
