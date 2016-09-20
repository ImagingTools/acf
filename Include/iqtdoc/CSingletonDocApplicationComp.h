#ifndef iqtdoc_CSingletonDocApplicationComp_included
#define iqtdoc_CSingletonDocApplicationComp_included


// Qt includes
#include <QtCore/QSharedMemory>
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QString>
#include <QtCore/QStringList>

// ACF includes
#include <imod/TSingleModelObserverBase.h>
#include <icomp/CComponentBase.h>
#include <ibase/IApplicationInfo.h>
#include <ibase/IRuntimeStatusProvider.h>
#include <idoc/IDocumentManager.h>
#include <iqtgui/IGuiApplication.h>


namespace iqtdoc
{


/**
	Singleton application component.
	This component wraps a "real" application component implementation and allows only a single execution of the application instance.
*/
class CSingletonDocApplicationComp:
			public QObject,
			public icomp::CComponentBase,
			protected imod::TSingleModelObserverBase<ibase::IRuntimeStatusProvider>,
			virtual public ibase::IApplication
{
	Q_OBJECT
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<ibase::IRuntimeStatusProvider> BaseClass2;

	I_BEGIN_COMPONENT(CSingletonDocApplicationComp);
		I_REGISTER_INTERFACE(ibase::IApplication);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info used to identify the application in the system", false, "ApplicationInfo");
		I_ASSIGN(m_slaveApplicationCompPtr, "Application", "Application component to be executed", true, "Application");
		I_ASSIGN_TO(m_slaveGuiApplicationCompPtr, m_slaveApplicationCompPtr, false);
		I_ASSIGN(m_documentManagerCompPtr, "DocumentManager", "Document manager used for opening of documents triggered by the shell", false, "DocumentManager");
		I_ASSIGN(m_runtimeStatusProviderCompPtr, "RuntimeStatus", "Runtime status of the application", false, "RuntimeStatus");
		I_ASSIGN_TO(m_runtimeStatusProviderModelCompPtr, m_runtimeStatusProviderCompPtr, false);
	I_END_COMPONENT;

	CSingletonDocApplicationComp();

	// reimplemented (ibase::IApplication)
	virtual bool InitializeApplication(int argc, char** argv);
	virtual int Execute(int argc, char** argv);
	virtual QString GetHelpText() const;
	virtual QStringList GetApplicationArguments() const;

protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	void ShareDocumentsForOpening(int argc, char** argv);
	QStringList PopDocumentsForOpening() const;

private Q_SLOTS:
	void OnUpdateDocumentList();

private:
	enum{
		/**
			Maximal length of a document file path.
		*/
		MAX_DOCUMENT_PATH_LENGTH = 1024,
		/**
			Maximal number of document, which can be opened from the shell.
		*/
		MAX_DOCUMENTS_COUNT = 100
	};

	/**
		Info about running application.
	*/
	struct RunningProcessInfo
	{
		qint64 processId;
		char requestedDocuments[MAX_DOCUMENTS_COUNT][MAX_DOCUMENT_PATH_LENGTH];
	};

	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_REF(ibase::IApplication, m_slaveApplicationCompPtr);
	I_REF(iqtgui::IGuiApplication, m_slaveGuiApplicationCompPtr);

	istd::TDelPtr<QSharedMemory> m_processDataPtr;
	bool m_isAlreadyRunning;
	QTimer m_documentUpdateTimer;

	bool m_isApplicationInitialized;

	I_REF(idoc::IDocumentManager, m_documentManagerCompPtr);
	I_REF(ibase::IRuntimeStatusProvider, m_runtimeStatusProviderCompPtr);
	I_REF(imod::IModel, m_runtimeStatusProviderModelCompPtr);
};


} // namespace iqtdoc


#endif // !iqtdoc_CSingletonDocApplicationComp_included

