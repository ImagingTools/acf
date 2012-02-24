#include "iqtgui/CDialogApplicationComp.h"


// Qt includes
#include <QApplication>


namespace iqtgui
{


// reimplemented (ibase::IApplication)

int CDialogApplicationComp::Execute(int argc, char** argv)
{
	if (InitializeApplication(argc, argv)){
		std::string appStyle;

		// parse arguments:
		for (int index = 1; index < argc; index++){
			std::string arg = argv[index];
			if (arg == "-style"){
				if (index+1 < argc){
					appStyle = argv[index+1];
				}
			}
		}
		QApplication::setStyle(appStyle.c_str());

		TryShowSplashScreen();

		m_mainDialogCompPtr.IsValid();

		HideSplashScreen();

		if (m_mainDialogCompPtr.IsValid()){
			m_mainDialogCompPtr->ExecuteDialog(NULL);

			return 0;
		}
	}

	return -1;
}


QString CDialogApplicationComp::GetHelpText() const
{
	return "-style QtStyle\tname of Qt-specified style";
}


} // namespace iqtgui



