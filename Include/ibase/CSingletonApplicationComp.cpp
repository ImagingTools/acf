#include "ibase/CSingletonApplicationComp.h"


// Qt includes
#include <QtCore/QString>
#include <QtCore/QFileInfo>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

#ifdef Q_OS_MAC
#include <Carbon/Carbon.h>
#endif


namespace ibase
{


CSingletonApplicationComp::CSingletonApplicationComp()
	:m_isAlreadyRunning(false)
{
}


// reimplemented (ibase::IApplication)

bool CSingletonApplicationComp::InitializeApplication(int argc, char** argv)
{
	if (m_isAlreadyRunning){
		ShareDocumentsForOpening(argc, argv);

		qDebug("Application already started");

		return false;
	}

	if (m_slaveApplicationCompPtr.IsValid()){
		return m_slaveApplicationCompPtr->InitializeApplication(argc, argv);
	}

	return false;
}


int CSingletonApplicationComp::Execute(int argc, char** argv)
{
	if (m_isAlreadyRunning){
		ShareDocumentsForOpening(argc, argv);

		qDebug("Application already started");

		return 0;
	}

	if (m_slaveApplicationCompPtr.IsValid()){
		return m_slaveApplicationCompPtr->Execute(argc, argv);
	}

	return -1;
}


QString CSingletonApplicationComp::GetHelpText() const
{
	if (m_slaveApplicationCompPtr.IsValid()){
		return m_slaveApplicationCompPtr->GetHelpText();
	}

	return QString();
}


QStringList CSingletonApplicationComp::GetApplicationArguments() const
{
	if (m_slaveApplicationCompPtr.IsValid()){
		return m_slaveApplicationCompPtr->GetApplicationArguments();
	}

	return QStringList();
}


// protected methods

// reimplemented (imod::CSingleModelObserverBase)

void CSingletonApplicationComp::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	ibase::IRuntimeStatusProvider* objectPtr = GetObjectPtr();
	if ((objectPtr != NULL) && objectPtr->GetRuntimeStatus() == ibase::IRuntimeStatusProvider::RS_RUNNING){
		if (m_documentManagerCompPtr.IsValid()){
			connect(&m_documentUpdateTimer, SIGNAL(timeout()), this, SLOT(OnUpdateDocumentList()));

			m_documentUpdateTimer.start(500);
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CSingletonApplicationComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_applicationInfoCompPtr.IsValid()){
		QString applicationId = QString("%1/%2/%3")
					.arg(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_PRODUCT_NAME, false))
					.arg(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME, false))
					.arg(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_SUBNAME, false));

		m_processData.SetPtr(new QSharedMemory(applicationId));

		m_isAlreadyRunning = false;

		if (!m_processData->attach()){
			if (m_processData->create(sizeof(RunningProcessInfo))){
				if (m_processData->lock()){
					RunningProcessInfo* dataPtr = (RunningProcessInfo*)m_processData->data();
					if (dataPtr != NULL){
						dataPtr->processId = QCoreApplication::applicationPid();
					}
				}

				m_processData->unlock();
			}
		}
		else{
			RunningProcessInfo* dataPtr = (RunningProcessInfo*)(m_processData->data());
			if (dataPtr->processId == 0){
				dataPtr->processId = QCoreApplication::applicationPid();
			}
			else{
#ifdef Q_OS_MAC
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

	if (m_runtimeStatusProviderModelCompPtr.IsValid()){
		m_runtimeStatusProviderModelCompPtr->AttachObserver(this);
	}
}


void CSingletonApplicationComp::OnComponentDestroyed()
{
	m_documentUpdateTimer.stop();

	EnsureModelDetached();

	if (m_processData->isAttached()){
		RunningProcessInfo* dataPtr = (RunningProcessInfo*)m_processData->data();
		if (dataPtr->processId == QCoreApplication::applicationPid()){
			dataPtr->processId = 0;
		}
		m_processData->detach();
	}

	BaseClass::OnComponentDestroyed();
}


// private methods

void CSingletonApplicationComp::ShareDocumentsForOpening(int argc, char** argv)
{
	// Set the list of documents need to be open by the document manager:
	if (m_processData->lock()){
		RunningProcessInfo* dataPtr = (RunningProcessInfo*)m_processData->data();
		if (dataPtr != NULL){
			memset(dataPtr->requestedDocuments, 0, sizeof(dataPtr->requestedDocuments));

			for (int argIndex = 1; argIndex < argc; ++argIndex){
				QFileInfo filePath(argv[argIndex]);
				if (filePath.exists()){
					int pathLength = qMin<int>(filePath.size(), MAX_DOCUMENT_PATH);

					memcpy(
								dataPtr->requestedDocuments[argIndex - 1],
								filePath.absoluteFilePath().toUtf8().constData(),
								pathLength); 
				}
			}
		}
	}

	m_processData->unlock();
}


QStringList CSingletonApplicationComp::PopDocumentsForOpening() const
{
	QStringList retVal;

	// Set the list of documents need to be open by the document manager:
	if (m_processData->lock()){
		RunningProcessInfo* dataPtr = (RunningProcessInfo*)m_processData->data();
		if (dataPtr != NULL){
			for (int documentIndex = 0; documentIndex < MAX_DOCUMENTS; ++documentIndex){
				QString filePath(dataPtr->requestedDocuments[documentIndex]);

				if (!filePath.isEmpty()){
					retVal.push_back(filePath);
				}
			}
		}

		memset(dataPtr->requestedDocuments, 0, sizeof(dataPtr->requestedDocuments));
	}

	m_processData->unlock();

	return retVal;
}


//private slots

void CSingletonApplicationComp::OnUpdateDocumentList()
{
	Q_ASSERT(m_documentManagerCompPtr.IsValid());

	QStringList documentsToOpen = PopDocumentsForOpening();

	for (int documentIndex = 0; documentIndex < documentsToOpen.count(); ++documentIndex){
		m_documentManagerCompPtr->OpenDocument(NULL, &documentsToOpen[documentIndex]);
	}
}


} // namespace ibase



