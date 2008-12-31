#ifndef iqtdoc_CMultiDocumentWorkspaceGuiComp_included
#define iqtdoc_CMultiDocumentWorkspaceGuiComp_included


// Qt includes
#include <QMdiArea>
#include <QMdiSubWindow>


// ACF includes
#include "ibase/IApplicationInfo.h"

#include "idoc/ICommandsProvider.h"
#include "idoc/CDocumentManagerBase.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/CHierarchicalCommand.h"


namespace iqtdoc
{


/**	
	This class is a Qt-based workspace implementation of a document manager.
*/
class CMultiDocumentWorkspaceGuiComp:
			public iqtgui::TGuiComponentBase<QMdiArea>, 
			public idoc::CDocumentManagerBase,
			public idoc::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqtgui::TGuiComponentBase<QMdiArea> BaseClass;
	typedef idoc::CDocumentManagerBase BaseClass2;

	I_BEGIN_COMPONENT(CMultiDocumentWorkspaceGuiComp);
		I_REGISTER_INTERFACE(idoc::IDocumentManager);
		I_REGISTER_INTERFACE(idoc::ICommandsProvider);
		I_ASSIGN(m_showMaximizedAttrPtr, "ShowViewMaximized", "At start shows the document view maximized", false, true);
		I_ASSIGN(m_maxRecentFilesCountAttrPtr, "MaxRecentFiles", "Maximal size of recent file list", true, 10);
		I_ASSIGN(m_documentTemplateCompPtr, "DocumentTemplate", "Document template", true, "DocumentTemplate");
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info", true, "ApplicationInfo");
	I_END_COMPONENT;

	enum GroupId
	{
		GI_WINDOW = 0x300,
		GI_DOCUMENT,
		GI_VIEW
	};

	CMultiDocumentWorkspaceGuiComp();

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnTryClose(bool* ignoredPtr = NULL);

	// reimplemented (iqtgui::CGuiComponentBase)
	void OnRetranslate();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

protected:
	/**
		Update titles of views or all views of specified document.
		\param	optional document object, if you want to update only views of single document.
	*/
	void UpdateAllTitles();

	/**
		Find view object associated with specified Qt widget.
		\param	widget	Qt widget representing view.
		\return	associated view or NULL if nothing is found.
	*/
	iqtgui::IGuiObject* GetViewFromWidget(const QWidget& widget) const;

	/**
		Creates signal/slot connnections for the implementation.
	*/
	virtual void CreateConnections();

	/**
		Creates the filter for the file selection dialog.
		\param	documentTypeIdPtr	optional ID of document type if only filter for single document type should be created.
	*/
	QString CreateFileDialogFilter(const std::string* documentTypeIdPtr = NULL) const;

	/**
		Called when number of windows changed.
	*/
	void OnViewsCountChanged();

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* obj, QEvent* event);

	// reimplemented (idoc::CDocumentManagerBase)
	virtual int GetMaxRecentFilesCount() const;
	virtual istd::CStringList GetOpenFileNames(const std::string* documentTypeIdPtr = NULL) const;
	virtual istd::CString GetSaveFileName(const std::string& documentTypeId) const;
	virtual void OnViewRegistered(istd::IPolymorphic* viewPtr);
	virtual void OnViewRemoved(istd::IPolymorphic* viewPtr);
	virtual bool QueryDocumentClose(const DocumentInfo& info);

	// reimplemented (imod::CMultiModelObserverBase)
	void OnUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (iqt:CGuiComponentBase)
	virtual void OnGuiCreated();	
	virtual void OnGuiDestroyed();

	// reimplemented (istd:IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

protected slots:
	void OnWindowActivated(QMdiSubWindow* window);
	void OnTileHorizontally();
	void OnTile();
	void OnCascade();
	void OnCloseAllViews();
	void OnWorkspaceModeChanged();

private:
	template <class Archive> 
	bool SerializeRecentFiles();

	void UpdateLastDirectory(const QString& filePath) const;

	iqtgui::CHierarchicalCommand m_commands;

	// global commands
	iqtgui::CHierarchicalCommand m_windowCommand;
	// window menu group
	iqtgui::CHierarchicalCommand m_cascadeCommand;
	iqtgui::CHierarchicalCommand m_tileHorizontallyCommand;
	iqtgui::CHierarchicalCommand m_tileVerticallyCommand;
	iqtgui::CHierarchicalCommand m_closeAllDocumentsCommand;
	iqtgui::CHierarchicalCommand m_workspaceModeCommand;
	iqtgui::CHierarchicalCommand m_subWindowCommand;
	iqtgui::CHierarchicalCommand m_tabbedCommand;

	I_ATTR(int, m_maxRecentFilesCountAttrPtr);
	I_ATTR(bool, m_showMaximizedAttrPtr);
	I_REF(idoc::IDocumentTemplate, m_documentTemplateCompPtr);
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);

	mutable QString m_lastDirectory;

	int m_viewsCount;
};


template <class Archive> 
bool CMultiDocumentWorkspaceGuiComp::SerializeRecentFiles()
{
	istd::CString applicationName = "ACF Application";
	istd::CString companyName = "ImagingTools";

	if (m_applicationInfoCompPtr.IsValid()){ 
		applicationName = m_applicationInfoCompPtr->GetApplicationName();
		companyName = m_applicationInfoCompPtr->GetCompanyName();
	}

	Archive archive(iqt::GetQString(companyName), iqt::GetQString(applicationName));
	
	return SerializeRecentFileList(archive);
}


} // namespace iqtdoc


#endif // !iqtdoc_CMultiDocumentWorkspaceGuiComp_included

