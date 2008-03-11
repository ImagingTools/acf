#include "iqt/CApplicationComp.h"


#include <QApplication>


namespace iqt
{


// reimplemented (ibase::IApplication)

int CApplicationComp::Execute(int argc, char** argv)
{
	int retVal = -1;

	if (m_mainGuiCompPtr.IsValid()){
		::std::string appStyle;

		// parse arguments:
		for (int index = 1; index < argc; index++){
			std::string arg = argv[index];
			if (arg == "-style"){
				if (index+1 < argc){
					appStyle = argv[index+1];
				}
			}
		}

		QApplication application(argc, argv);
		application.setStyle(appStyle.c_str());

		m_mainGuiCompPtr->CreateGui(NULL);
		QWidget* mainWidegtPtr = m_mainGuiCompPtr->GetWidget();

		if (mainWidegtPtr != NULL){
			mainWidegtPtr->show();

			// start application loop:
			retVal = application.exec();
		}

		m_mainGuiCompPtr->DestroyGui();
	}

	return retVal;
}


istd::CString CApplicationComp::GetHelpText() const
{
	return "";
}


} // namespace iqt



