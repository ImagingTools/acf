#ifndef iqtdoc_CExternalOpenDocumentCommandComp_included
#define iqtdoc_CExternalOpenDocumentCommandComp_included


// ACF includes
#include <imod/TSingleModelObserverBase.h>
#include <icomp/CComponentBase.h>
#include <ibase/ICommandsProvider.h>
#include <ifile/IFileNameParam.h>
#include <ifile/IFilePersistence.h>
#include <ifile/IFilePersistenceInfo.h>
#include <idoc/IDocumentManager.h>
#include <iqtgui/CHierarchicalCommand.h>


namespace iqtdoc
{


class CExternalOpenDocumentCommandComp:
			public QObject,
			public icomp::CComponentBase,
			protected imod::TSingleModelObserverBase<idoc::IDocumentManager>,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CExternalOpenDocumentCommandComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_documentPersistenceCompPtr, "DocumentPersistence", "File persistence for the actual document type", true, "DocumentPersistence"); 
		I_ASSIGN_TO(m_documentPersistenceInfoCompPtr, m_documentPersistenceCompPtr, false);
		I_ASSIGN(m_documentManagerCompPtr, "DocumentManager", "Manager of the documents in the MDI workspace", true, "DocumentManager");
		I_ASSIGN_TO(m_documentManagerModelCompPtr, m_documentManagerCompPtr, true);
		I_ASSIGN(m_documentFileCompPtr, "DocumentFilePath", "Path used for saving the current document", true, "DocumentFilePath");
		I_ASSIGN(m_applicationPathCompPtr, "ApplicationPath", "Path to the external executable used for opening the document file", true, "ApplicationPath");
		I_ASSIGN(m_documentTypeIdAttrPtr, "DocumentTypeId", "ID of the document type used by MDI workspace", true, "Default");
		I_ASSIGN(m_openDocumentCommandNameAttrPtr, "CommandName", "Name/Text of the open document command shown in File menu", true, "Open In...");
	I_END_COMPONENT;

	CExternalOpenDocumentCommandComp();

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

protected:
	virtual void UpdateCommands();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private Q_SLOTS:
	void OnOpenDocument();

private:
	iqtgui::CHierarchicalCommand m_rootCommands;
	iqtgui::CHierarchicalCommand m_fileCommands;
	iqtgui::CHierarchicalCommand m_openDocumentCommand;
	
	I_REF(ifile::IFilePersistence, m_documentPersistenceCompPtr);
	I_REF(ifile::IFilePersistenceInfo, m_documentPersistenceInfoCompPtr);
	I_REF(idoc::IDocumentManager, m_documentManagerCompPtr);
	I_REF(imod::IModel, m_documentManagerModelCompPtr);
	I_REF(ifile::IFileNameParam, m_documentFileCompPtr);
	I_REF(ifile::IFileNameParam, m_applicationPathCompPtr);
	I_ATTR(QByteArray, m_documentTypeIdAttrPtr);
	I_ATTR(QString, m_openDocumentCommandNameAttrPtr);
};


}  // namespace iqtdoc


#endif // iqtdoc_CExternalOpenDocumentCommandComp_included


