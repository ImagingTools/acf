#include <ibase/CConsoleApplicationComp.h>


// ACF includes
#include <icomp/CCompositeComponent.h>


namespace ibase
{


static int s_argc = 0;


// public methods

// reimplemented (ibase::IApplication)

bool CConsoleApplicationComp::InitializeApplication(int argc, char** argv)
{
	m_applicationArguments.clear();

	// parse arguments:
	for (int argIndex = 0; argIndex < argc; argIndex++){
		QByteArray arg = argv[argIndex];

		m_applicationArguments << QString::fromLocal8Bit(argv[argIndex]);
	}

	if (!m_applicationPtr.IsValid()){
		if (QCoreApplication::instance() != NULL)
		{
			m_applicationPtr.SetPtr(QCoreApplication::instance(), false);
		}
		else
		{
			s_argc = argc;
			m_applicationPtr.SetPtr(new QCoreApplication(s_argc, argv), true);
			if (!m_applicationPtr.IsValid()){
				return false;
			}
		}
	}

	connect(&m_consoleReader, SIGNAL(KeyPressedSignal(char)), this, SLOT(OnKeyPressed(char)));

	m_consoleReader.Start();

	if (m_applicationInfoCompPtr.IsValid())
	{
		QCoreApplication::setOrganizationName(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME));
		QCoreApplication::setApplicationName(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME));
	}

	icomp::CCompositeComponent* rootComponentPtr = NULL;

	for (		icomp::ICompositeComponent* componentPtr = const_cast<icomp::ICompositeComponent*>(GetParentComponent(true));
				componentPtr != NULL;
				componentPtr = const_cast<icomp::ICompositeComponent*>(componentPtr->GetParentComponent(true))){
		rootComponentPtr = dynamic_cast<icomp::CCompositeComponent*>(componentPtr);
	}

	if (rootComponentPtr != NULL){
		rootComponentPtr->EnsureAutoInitComponentsCreated();
	}

	for (int i = 0; i < m_componentsToInitializeCompPtr.GetCount(); ++i){
		istd::IPolymorphic* componentPtr = m_componentsToInitializeCompPtr[i];
		Q_UNUSED(componentPtr);
	}

	return true;
}


int CConsoleApplicationComp::Execute(int argc, char** argv)
{
	m_runtimeStatus.SetRuntimeStatus(ibase::IRuntimeStatusProvider::RS_STARTING);

	if (!InitializeApplication(argc, argv)){
		m_runtimeStatus.SetRuntimeStatus(ibase::IRuntimeStatusProvider::RS_SHUTDOWN);

		return -1;
	}

	m_runtimeStatus.SetRuntimeStatus(ibase::IRuntimeStatusProvider::RS_RUNNING);

	int retVal = m_applicationPtr->exec();

	m_runtimeStatus.SetRuntimeStatus(ibase::IRuntimeStatusProvider::RS_SHUTDOWN);

	OnApplicationShutdown();

	return retVal;
}


QString CConsoleApplicationComp::GetHelpText() const
{
	return QString();
}


QStringList CConsoleApplicationComp::GetApplicationArguments() const
{
	return m_applicationArguments;
}


// protected methods

void CConsoleApplicationComp::OnApplicationShutdown()
{
}


// reimplemented (icomp::CComponentBase)

void CConsoleApplicationComp::OnComponentDestroyed()
{
	m_consoleReader.Stop();

	BaseClass::OnComponentDestroyed();
}


// private slots

void CConsoleApplicationComp::OnKeyPressed(char ch)
{
	//if Q key pressed exit the application
	if (ch == 'Q' || ch == 'q'){
		QCoreApplication::exit();
	}
}


// public methods of the embedded class RuntimeStatus

CConsoleApplicationComp::RuntimeStatus::RuntimeStatus()
	:m_status(RS_NONE)
{
}


void CConsoleApplicationComp::RuntimeStatus::SetRuntimeStatus(IRuntimeStatusProvider::RuntimeStatus runtimeStatus)
{
	if (m_status != runtimeStatus){
		istd::CChangeNotifier changePtr(this);

		m_status = runtimeStatus;
	}
}


// reimplemented (ibase::IRuntimeStatusProvider)

ibase::IRuntimeStatusProvider::RuntimeStatus CConsoleApplicationComp::RuntimeStatus::GetRuntimeStatus() const
{
	return m_status;
}


} // namespace ibase


