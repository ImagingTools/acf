#include <iqtgui/CDialogApplicationComp.h>


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif


namespace iqtgui
{


// reimplemented (ibase::IApplication)

int CDialogApplicationComp::Execute(int argc, char** argv)
{
	m_runtimeStatus.SetRuntimeStatus(ibase::IRuntimeStatusProvider::RS_STARTING);

	if (InitializeApplication(argc, argv)){
		QByteArray appStyle;

		// parse arguments:
		for (int index = 1; index < argc; index++){
			QByteArray arg = argv[index];
			if (arg == "-style"){
				if (index+1 < argc){
					appStyle = argv[index+1];
				}
			}
		}
		QApplication::setStyle(appStyle);

		TryShowSplashScreen();

		m_mainDialogCompPtr.EnsureInitialized();

		HideSplashScreen();

		if (m_mainDialogCompPtr.IsValid()){
			m_mainDialogCompPtr->ExecuteDialog(NULL);

			m_runtimeStatus.SetRuntimeStatus(ibase::IRuntimeStatusProvider::RS_SHUTDOWN);

			return 0;
		}
	}

	m_runtimeStatus.SetRuntimeStatus(ibase::IRuntimeStatusProvider::RS_SHUTDOWN);

	return -1;
}


QString CDialogApplicationComp::GetHelpText() const
{
	return "-style QtStyle\tname of Qt-specified style";
}


} // namespace iqtgui



