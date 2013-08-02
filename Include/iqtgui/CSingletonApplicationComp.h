#ifndef iqtgui_CSingletonApplicationComp_included
#define iqtgui_CSingletonApplicationComp_included


// Qt includes
#include <QtCore/QSharedMemory>
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QStringList>

// ACF includes
#include "imod/TSingleModelObserverBase.h"
#include "icomp/CComponentBase.h"
#include "ibase/IApplication.h"
#include "ibase/IApplicationInfo.h"
#include "idoc/IDocumentManager.h"
#include "ibase/IRuntimeStatusProvider.h"


namespace iqtgui
{


/**
	Singleton application component.
	This component wraps a "real" application component implementation and allows only a single execution of the application instance.
*/
class CSingletonApplicationComp:
			public QObject,
			public icomp::CComponentBase,
			protected imod::TSingleModelObserverBase<ibase::IRuntimeStatusProvider>,
			virtual public ibase::IApplication
{
	Q_OBJECT
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<ibase::IRuntimeStatusProvider> BaseClass2;

	I_BEGIN_COMPONENT(CSingletonApplicationComp);
		I_REGISTER_INTERFACE(ibase::IApplication);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info used to identify the application in the system", false, "ApplicationInfo");
		I_ASSIGN(m_slaveApplicationCompPtr, "Application", "Application component to be executed", true, "Application");
		I_ASSIGN(m_documentManagerCompPtr, "DocumentManager", "Document manager used for opening of documents triggered by the shell", false, "DocumentManager");
		I_ASSIGN(m_runtimeStatusProviderCompPtr, "RuntimeStatus", "Runtime status of the application", false, "RuntimeStatus");
		I_ASSIGN_TO(m_runtimeStatusProviderModelCompPtr, m_runtimeStatusProviderCompPtr, false);
	I_END_COMPONENT;

	CSingletonApplicationComp();

	// reimplemented (ibase::IApplication)
	virtual bool InitializeApplication(int argc, char** argv);
	virtual int Execute(int argc, char** argv);
	virtual QString GetHelpText() const;
	virtual QStringList GetApplicationArguments() const;

protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	void ShareDocumentsForOpening(int argc, char** argv);
	QStringList PopDocumentsForOpening() const;

private Q_SLOTS:
	void OnUpdateDocumentList();

private:
	/**
		Maximal length of a document file path.
	*/
	static const int MAX_DOCUMENT_PATH = 1024;

	/**
		Maximal number of document, which can be opened from the shell.
	*/
	static const int MAX_DOCUMENTS = 100;

	/**
		Info about running application.
	*/
	struct RunningProcessInfo
	{
		qint64 processId;
		char requestedDocuments[MAX_DOCUMENTS][MAX_DOCUMENT_PATH];
	};

	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_REF(ibase::IApplication, m_slaveApplicationCompPtr);

	istd::TDelPtr<QSharedMemory> m_processData;
	bool m_isAlreadyRunning;
	QTimer m_documentUpdateTimer;

	I_REF(idoc::IDocumentManager, m_documentManagerCompPtr);
	I_REF(ibase::IRuntimeStatusProvider, m_runtimeStatusProviderCompPtr);
	I_REF(imod::IModel, m_runtimeStatusProviderModelCompPtr);
};


} // namespace iqtgui


#endif // iqtgui_CSingletonApplicationComp_included

