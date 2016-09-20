#ifndef ifilegui_CExternalOpenDocumentCommandCompBase_included
#define ifilegui_CExternalOpenDocumentCommandCompBase_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <ibase/ICommandsProvider.h>
#include <ifile/IFileNameParam.h>
#include <ifile/IFilePersistence.h>
#include <ifile/IFilePersistenceInfo.h>
#include <iqtgui/CHierarchicalCommand.h>


namespace ifilegui
{


class CExternalOpenDocumentCommandCompBase:
			public QObject,
			public icomp::CComponentBase,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT
public:
	typedef icomp::CComponentBase BaseClass;

	enum GroupId
	{
		GI_EXERNAL_OPEN_COMMAND = 0x700
	};

	I_BEGIN_BASE_COMPONENT(CExternalOpenDocumentCommandCompBase);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_documentPersistenceCompPtr, "DocumentPersistence", "File persistence for the actual document type", true, "DocumentPersistence"); 
		I_ASSIGN_TO(m_documentPersistenceInfoCompPtr, m_documentPersistenceCompPtr, false);
		I_ASSIGN(m_documentFileCompPtr, "DocumentFilePath", "Path used for saving the current document", true, "DocumentFilePath");
		I_ASSIGN(m_applicationPathCompPtr, "ApplicationPath", "Path to the external executable used for opening the document file", true, "ApplicationPath");
		I_ASSIGN(m_openDocumentCommandNameAttrPtr, "CommandName", "Name/Text of the open document command shown in File menu", true, "Open In...");
	I_END_COMPONENT;

	CExternalOpenDocumentCommandCompBase();

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

protected:
	// abstract methods
	virtual const istd::IChangeable* GetDocumentPtr() const = 0;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private Q_SLOTS:
	void OnOpenDocument();

protected:
	iqtgui::CHierarchicalCommand m_rootCommands;
	iqtgui::CHierarchicalCommand m_fileCommands;
	iqtgui::CHierarchicalCommand m_openDocumentCommand;
	
	I_REF(ifile::IFilePersistence, m_documentPersistenceCompPtr);
	I_REF(ifile::IFilePersistenceInfo, m_documentPersistenceInfoCompPtr);
	I_REF(ifile::IFileNameParam, m_documentFileCompPtr);
	I_REF(ifile::IFileNameParam, m_applicationPathCompPtr);
	I_ATTR(QString, m_openDocumentCommandNameAttrPtr);
};


}  // namespace ifilegui


#endif // ifilegui_CExternalOpenDocumentCommandCompBase_included


