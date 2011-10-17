#ifndef iqtdoc_CMultiDocumentWorkspaceGuiComp_included
#define iqtdoc_CMultiDocumentWorkspaceGuiComp_included


// Qt includes
#include <QMdiArea>
#include <QMdiSubWindow>


// ACF includes
#include "ibase/ICommandsProvider.h"
#include "idoc/CMultiDocumentManagerBase.h"

#include "iqt/ISettingsProvider.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TRestorableGuiWrap.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "iqtdoc/TQtDocumentManagerWrap.h"


namespace iqtdoc
{


/**	
	This class is a Qt-based workspace implementation of a document manager.
*/
class CMultiDocumentWorkspaceGuiComp:
			public iqtdoc::TQtDocumentManagerWrap<
						idoc::CMultiDocumentManagerBase,
						iqtgui::TRestorableGuiWrap<
									iqtgui::TGuiComponentBase<QMdiArea> > >,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqtdoc::TQtDocumentManagerWrap<
						idoc::CMultiDocumentManagerBase,
						iqtgui::TRestorableGuiWrap<
									iqtgui::TGuiComponentBase<QMdiArea> > > BaseClass;

	I_BEGIN_COMPONENT(CMultiDocumentWorkspaceGuiComp);
		I_REGISTER_INTERFACE(idoc::IDocumentManager);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_documentTemplateCompPtr, "DocumentTemplate", "Document template", true, "DocumentTemplate");
		I_ASSIGN(m_showMaximizedAttrPtr, "ShowViewMaximized", "At start shows the document view maximized", false, true);
		I_ASSIGN(m_allowViewRepeatingAttrPtr, "AllowViewRepeating", "If enabled, multiple views for the same document are allowed", false, true);
	I_END_COMPONENT;

	enum GroupId
	{
		GI_WINDOW = 0x300,
		GI_DOCUMENT,
		GI_VIEW
	};

	CMultiDocumentWorkspaceGuiComp();

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnTryClose(bool* ignoredPtr = NULL);

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
		Called when number of windows changed.
	*/
	void OnViewsCountChanged();

	// reimplemented (idoc::CMultiDocumentManagerBase)
	virtual istd::IChangeable* OpenDocument(
				const istd::CString& filePath,
				bool createView,
				const std::string& viewTypeId,
				std::string& documentTypeId);
	virtual void SetActiveView(istd::IPolymorphic* viewPtr);

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

	// reimplemented (TRestorableGuiWrap)
	virtual void OnRestoreSettings(const QSettings& settings);
	virtual void OnSaveSettings(QSettings& settings) const;

	// reimplemented (idoc::CMultiDocumentManagerBase)
	virtual void CloseAllDocuments();
	virtual istd::CStringList GetOpenFilePaths(const std::string* documentTypeIdPtr = NULL) const;
	virtual void OnViewRegistered(istd::IPolymorphic* viewPtr);
	virtual void OnViewRemoved(istd::IPolymorphic* viewPtr);
	virtual void QueryDocumentClose(const SingleDocumentData& info, bool* ignoredPtr);

	// reimplemented (iqt:CGuiComponentBase)
	virtual void OnGuiCreated();	
	virtual void OnGuiDestroyed();
	virtual void OnRetranslate();
	virtual void OnGuiRetranslate();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

	// reimplemented (istd:IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

protected Q_SLOTS:
	void OnWindowActivated(QMdiSubWindow* window);
	void OnTileHorizontally();
	void OnTile();
	void OnCascade();
	void OnCloseAllViews();
	void OnWorkspaceModeChanged();

private:
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

	I_REF(idoc::IDocumentTemplate, m_documentTemplateCompPtr);
	I_ATTR(bool, m_showMaximizedAttrPtr);
	I_ATTR(bool, m_allowViewRepeatingAttrPtr);
	
	mutable QString m_lastDirectory;

	int m_viewsCount;
};


} // namespace iqtdoc


#endif // !iqtdoc_CMultiDocumentWorkspaceGuiComp_included


