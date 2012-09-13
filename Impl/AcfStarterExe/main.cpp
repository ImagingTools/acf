// Qt includes
#include <QtCore/QFileInfo>
#include <QtGui/QMessageBox>

// Project includes
#include "CAcfStarter.h"
#include "Generated/ui_CAcfStarter.h"


int main(int argc, char *argv[])
{		
	QApplication::setStyle("plastique");
	QApplication app(argc, argv);

	if (argc > 1){
		QFileInfo confFile = QString(argv[1]);

		if (!confFile.exists()){
			QMessageBox::warning(
						NULL,
						QObject::tr("ACF Starter"),
						QObject::tr("File '%1' does not exist").arg(confFile.absolutePath()),
						QMessageBox::Ok);		
			return 1;
		}

		if (confFile.suffix() == "xpc"){	
			CAcfStarter starter(argv);
			starter.RunACF(1000);
			starter.show();				
			
			return app.exec();			
		}
		else{
			QMessageBox::warning(
						NULL,
						QObject::tr("ACF Starter"),
						QObject::tr("Invalid file format"),
						QMessageBox::Ok);			
			return 1;
		}	

	}
	else{
		CAcfStarter starter;

		return app.exec();
	}
}


