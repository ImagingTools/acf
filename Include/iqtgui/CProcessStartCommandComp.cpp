#include <iqtgui/CProcessStartCommandComp.h>


// Qt includes
#include <QtCore/QProcess>


namespace iqtgui
{


// public methods

CProcessStartCommandComp::CProcessStartCommandComp()
	:m_startProcessCommand("Start...", 100, ibase::ICommand::CF_GLOBAL_MENU, GI_START_PROCESS_COMMAND)
{
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CProcessStartCommandComp::GetCommands() const
{
	return &m_rootCommands;
}


// protected methods

void CProcessStartCommandComp::CreateMenu()
{
	connect(&m_startProcessCommand, SIGNAL(triggered()), this, SLOT(OnStartProcessCommand()));

	m_rootCommands.InsertChild(&m_startProcessCommand);
}


QStringList CProcessStartCommandComp::GetProcessArguments() const
{
	QStringList retVal;

	for (int i = 0; i < m_processArgumentsAttrPtr.GetCount(); ++i){
		retVal += m_processArgumentsAttrPtr[i];
	}

	return retVal;
}


bool CProcessStartCommandComp::StartProcess(const QStringList& arguments)
{
	Q_ASSERT(m_applicationPathCompPtr.IsValid());

	QString applicationPath = m_applicationPathCompPtr->GetPath();
	QFileInfo applicationFileInfo(applicationPath);

#ifdef Q_OS_WIN32
	if (applicationFileInfo.suffix().isEmpty()){
		applicationPath += ".exe";
	}
#endif
#ifdef Q_OS_MAC
	if (!applicationFileInfo.exists() && applicationFileInfo.suffix().isEmpty() && applicationFileInfo.isBundle()){
		applicationPath += ".app";
	}
#endif

	if (!QFileInfo(applicationPath).exists()){
		SendErrorMessage(0, QString(QCoreApplication::tr("Executable file '%1' doesn't exist")).arg(applicationPath));

		return false;
	}

#ifndef QT_NO_PROCESS
	return QProcess::startDetached(applicationPath, arguments);
#endif // QT_NO_PROCESS

	return false;
}


// reimplemented (icomp::CComponentBase)

void CProcessStartCommandComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (!m_applicationPathCompPtr.IsValid()){
		m_startProcessCommand.setEnabled(false);
	}
	
	m_startProcessCommand.SetName(*m_startProcessCommandNameAttrPtr);

	CreateMenu();
}


// private slots

void CProcessStartCommandComp::OnStartProcessCommand()
{
	StartProcess(GetProcessArguments());
}


} // namespace iqtgui


