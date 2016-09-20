#include <iqtdoc/CSingletonDocApplicationComp.h>


// Qt includes
#include <QtCore/QFileInfo>

#if defined(Q_OS_MAC) && (QT_VERSION < 0x050000)
#include <Carbon/Carbon.h>
#endif


namespace iqtdoc
{


CSingletonDocApplicationComp::CSingletonDocApplicationComp()
:	m_isAlreadyRunning(false),
	m_isApplicationInitialized(false)
{
}


// reimplemented (ibase::IApplication)

bool CSingletonDocApplicationComp::InitializeApplication(int argc, char** argv)
{
	if (!m_isApplicationInitialized && m_slaveApplicationCompPtr.IsValid()){
		if (m_slaveApplicationCompPtr->InitializeApplication(argc, argv)){
			if (m_runtimeStatusProviderModelCompPtr.IsValid()){
				if (m_runtimeStatusProviderModelCompPtr->AttachObserver(this)){
					OnUpdate(istd::IChangeable::GetNoChanges());
				}
			}

			m_isApplicationInitialized = true;
		}
	}

	return m_isApplicationInitialized;
}


int CSingletonDocApplicationComp::Execute(int argc, char** argv)
{
	if (m_isAlreadyRunning){
		ShareDocumentsForOpening(argc, argv);

		qDebug("Application already started");

		return 0;
	}

	if (InitializeApplication(argc, argv)){
		if (m_slaveApplicationCompPtr.IsValid()){
			return m_slaveApplicationCompPtr->Execute(argc, argv);
		}
	}

	return -1;
}


QString CSingletonDocApplicationComp::GetHelpText() const
{
	if (m_slaveApplicationCompPtr.IsValid()){
		return m_slaveApplicationCompPtr->GetHelpText();
	}

	return QString();
}


QStringList CSingletonDocApplicationComp::GetApplicationArguments() const
{
	if (m_slaveApplicationCompPtr.IsValid()){
		return m_slaveApplicationCompPtr->GetApplicationArguments();
	}

	return QStringList();
}


// protected methods

// reimplemented (imod::CSingleModelObserverBase)

void CSingletonDocApplicationComp::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	ibase::IRuntimeStatusProvider* objectPtr = GetObservedObject();
	if ((objectPtr != NULL) && objectPtr->GetRuntimeStatus() == ibase::IRuntimeStatusProvider::RS_RUNNING){
		if (m_documentManagerCompPtr.IsValid()){
			connect(&m_documentUpdateTimer, SIGNAL(timeout()), this, SLOT(OnUpdateDocumentList()));

			m_documentUpdateTimer.start(500);
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CSingletonDocApplicationComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_applicationInfoCompPtr.IsValid()){
		QString applicationId = QString("%1/%2/%3")
					.arg(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_PRODUCT_NAME, false))
					.arg(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME, false))
					.arg(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_SUBNAME, false));

		m_processDataPtr.SetPtr(new QSharedMemory(applicationId));

		m_isAlreadyRunning = false;

		if (!m_processDataPtr->attach()){
			if (m_processDataPtr->create(sizeof(RunningProcessInfo))){
				if (m_processDataPtr->lock()){
					RunningProcessInfo* dataPtr = (RunningProcessInfo*)m_processDataPtr->data();
					if (dataPtr != NULL){
						dataPtr->processId = QCoreApplication::applicationPid();
					}
				}

				m_processDataPtr->unlock();
			}
		}
		else{
			RunningProcessInfo* dataPtr = (RunningProcessInfo*)(m_processDataPtr->data());
			if (dataPtr->processId == 0){
				dataPtr->processId = QCoreApplication::applicationPid();
			}
			else{
#if defined(Q_OS_MAC) && (QT_VERSION < 0x050000)
				ProcessSerialNumber psn;
				if (GetProcessForPID(dataPtr->processId, &psn) == 0){
					m_isAlreadyRunning = true;
				}
#else
				m_isAlreadyRunning = true;
#endif//!Q_OS_MAC
			}
		}
	}
}


void CSingletonDocApplicationComp::OnComponentDestroyed()
{
	m_documentUpdateTimer.stop();

	EnsureModelDetached();

	if (m_processDataPtr->isAttached()){
		RunningProcessInfo* dataPtr = (RunningProcessInfo*)m_processDataPtr->data();
		if (dataPtr->processId == QCoreApplication::applicationPid()){
			dataPtr->processId = 0;
		}
		m_processDataPtr->detach();
	}

	BaseClass::OnComponentDestroyed();
}


// private methods

void CSingletonDocApplicationComp::ShareDocumentsForOpening(int argc, char** argv)
{
	// Set the list of documents need to be open by the document manager:
	if (m_processDataPtr->lock()){
		RunningProcessInfo* dataPtr = (RunningProcessInfo*)m_processDataPtr->data();
		if (dataPtr != NULL){
			memset(dataPtr->requestedDocuments, 0, sizeof(dataPtr->requestedDocuments));

			for (int argIndex = 1; argIndex < argc; ++argIndex){
				QFileInfo filePath(argv[argIndex]);
				if (filePath.exists()){
					int pathLength = qMin<int>(filePath.size(), MAX_DOCUMENT_PATH_LENGTH);

					memcpy(		dataPtr->requestedDocuments[argIndex - 1],
								filePath.absoluteFilePath().toUtf8().constData(),
								pathLength); 
				}
			}
		}
	}

	m_processDataPtr->unlock();
}


QStringList CSingletonDocApplicationComp::PopDocumentsForOpening() const
{
	QStringList retVal;

	// Set the list of documents need to be open by the document manager:
	if (m_processDataPtr->lock()){
		RunningProcessInfo* dataPtr = (RunningProcessInfo*)m_processDataPtr->data();
		if (dataPtr != NULL){
			for (int documentIndex = 0; documentIndex < MAX_DOCUMENTS_COUNT; ++documentIndex){
				QString filePath(dataPtr->requestedDocuments[documentIndex]);

				if (!filePath.isEmpty()){
					retVal.push_back(filePath);
				}
			}

			memset(dataPtr->requestedDocuments, 0, sizeof(dataPtr->requestedDocuments));
		}
	}

	m_processDataPtr->unlock();

	return retVal;
}


//private slots

void CSingletonDocApplicationComp::OnUpdateDocumentList()
{
	Q_ASSERT(m_documentManagerCompPtr.IsValid());

	QStringList documentsToOpen = PopDocumentsForOpening();

	for (int documentIndex = 0; documentIndex < documentsToOpen.count(); ++documentIndex){
		m_documentManagerCompPtr->OpenDocument(NULL, &documentsToOpen[documentIndex]);
	}

	if ((documentsToOpen.count() > 0) && m_slaveGuiApplicationCompPtr.IsValid()){
		const iqtgui::IGuiObject* guiPtr = m_slaveGuiApplicationCompPtr->GetApplicationGui();
		if (guiPtr != NULL){
			QWidget* widgetPtr = guiPtr->GetWidget();

			widgetPtr->activateWindow();
			widgetPtr->raise();
		}
	}
}


} // namespace iqtdoc


