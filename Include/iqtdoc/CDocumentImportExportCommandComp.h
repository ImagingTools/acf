#ifndef iqtdoc_CDocumentImportExportCommandComp_included
#define iqtdoc_CDocumentImportExportCommandComp_included


// ACF includes
#include <imod/TSingleModelObserverBase.h>
#include <icomp/CComponentBase.h>
#include <ibase/ICommandsProvider.h>
#include <ifile/IFilePersistence.h>
#include <ifile/IFilePersistenceInfo.h>
#include <idoc/IDocumentManager.h>
#include <iqtgui/CHierarchicalCommand.h>


namespace iqtdoc
{


/**
	Component used for triggering of the document import/export command from the file menu of a document based application.
*/
class CDocumentImportExportCommandComp:
			public QObject,
			public icomp::CComponentBase,
			protected imod::TSingleModelObserverBase<idoc::IDocumentManager>,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CDocumentImportExportCommandComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_documentPersistenceCompPtr, "DocumentPersistence", "File persistence for the actual document type", true, "DocumentPersistence"); 
		I_ASSIGN_TO(m_documentPersistenceInfoCompPtr, m_documentPersistenceCompPtr, true);
		I_ASSIGN(m_documentManagerCompPtr, "DocumentManager", "Manager of the documents in the MDI workspace", true, "DocumentManager");
		I_ASSIGN_TO(m_documentManagerModelCompPtr, m_documentManagerCompPtr, true);
		I_ASSIGN(m_documentTypeIdAttrPtr, "DocumentTypeId", "ID of the document type used by MDI workspace", true, "Default");
		I_ASSIGN(m_importCommandNameAttrPtr, "ImportCommandName", "Name/Text of the import command shown in File menu", true, "Import...");
		I_ASSIGN(m_exportCommandNameAttrPtr, "ExportCommandName", "Name/Text of the export command shown in File menu", true, "Export...");
	I_END_COMPONENT;

	CDocumentImportExportCommandComp();

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected Q_SLOTS:
	virtual void OnImport();
	virtual void OnExport();

protected:
	virtual void UpdateCommands();

protected:
	iqtgui::CHierarchicalCommand m_rootCommands;
	iqtgui::CHierarchicalCommand m_fileCommands;
	iqtgui::CHierarchicalCommand m_importCommand;
	iqtgui::CHierarchicalCommand m_exportCommand;
	
	I_REF(idoc::IDocumentManager, m_documentManagerCompPtr);
	I_REF(imod::IModel, m_documentManagerModelCompPtr);
	I_REF(ifile::IFilePersistence, m_documentPersistenceCompPtr);
	I_REF(ifile::IFilePersistenceInfo, m_documentPersistenceInfoCompPtr);
	I_ATTR(QByteArray, m_documentTypeIdAttrPtr);
	I_ATTR(QString, m_importCommandNameAttrPtr);
	I_ATTR(QString, m_exportCommandNameAttrPtr);
};


}  // namespace iqtdoc


#endif // !iqtdoc_CDocumentImportExportCommandComp_included


