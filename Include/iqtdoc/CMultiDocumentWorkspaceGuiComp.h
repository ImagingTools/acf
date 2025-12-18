#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMdiSubWindow>
#else
#include <QtGui/QMdiArea>
#include <QtGui/QMdiSubWindow>
#endif

// ACF includes
#include <ibase/ICommandsProvider.h>
#include <iprm/ISelectionParam.h>
#include <iprm/IOptionsList.h>
#include <idoc/CMultiDocumentManagerBase.h>
#include <idoc/IDocumentTemplate.h>
#include <iqtgui/TGuiComponentBase.h>
#include <iqtgui/TRestorableGuiWrap.h>
#include <iqtgui/CHierarchicalCommand.h>
#include <iqtdoc/TQtDocumentManagerWrap.h>


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
		I_REGISTER_INTERFACE(idoc::IDocumentTypesInfo);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_REGISTER_SUBELEMENT(DocumentSelection);
		I_REGISTER_SUBELEMENT_INTERFACE(DocumentSelection, iprm::ISelectionParam, ExtractSelectionInfo);
		I_REGISTER_SUBELEMENT_INTERFACE(DocumentSelection, imod::IModel, ExtractSelectionInfo);
		I_REGISTER_SUBELEMENT_INTERFACE(DocumentSelection, istd::IChangeable, ExtractSelectionInfo);
		I_REGISTER_SUBELEMENT_INTERFACE(DocumentSelection, iprm::IOptionsList, ExtractSelectionInfo);
		I_ASSIGN(m_documentTemplateCompPtr, "DocumentTemplate", "Document template", true, "DocumentTemplate");
		I_ASSIGN(m_showMaximizedAttrPtr, "ShowViewMaximized", "At start shows the document view maximized", false, true);
		I_ASSIGN(m_showPathAsTipAttrPtr, "ShowFilePathAsToolTip", "if enabled, the current document file path will be shown as tool tip of document tab", true, false);
		I_ASSIGN(m_allowViewRepeatingAttrPtr, "AllowViewRepeating", "If enabled, multiple views for the same document are allowed", false, true);
		I_ASSIGN(m_rememberOpenDocumentsParamPtr, "RememberOpenDocumentsOnExit", "If enabled, restores open documents from previous session", false, "RememberOpenDocumentsOnExit");
		I_ASSIGN(m_workspaceBackgroundColorAttrPtr, "WorkspaceBackgroundColor", "Background color of the MDI workspace", false, "");
		I_ASSIGN(m_defaultCreatedDocumentTypeIdAttrPtr, "DefaultCreatedDocumentTypeId", "Type-ID of the document that should be created if the workspace is empty", false, "");
	I_END_COMPONENT;

	enum GroupId
	{
		GI_WINDOW = 0x300,
		GI_DOCUMENT,
		GI_VIEW
	};

	CMultiDocumentWorkspaceGuiComp();

	// reimplemented (idoc::IDocumentManager)
	virtual void SetActiveView(istd::IPolymorphic* viewPtr) override;

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const override;

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnTryClose(bool* ignoredPtr = NULL) override;
	virtual void OnGuiDesignChanged() override;

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
		Find index of document associated with specified Qt widget.
		\param	widget	Qt widget representing view.
		\return	index of document or negative value if nothing is found.
	*/
	int GetDocumentIndexFromWidget(const QWidget& widget) const;

	/**
		Creates signal/slot connnections for the implementation.
	*/
	virtual void CreateConnections();

	/**
		Called when number of windows changed.
	*/
	void OnViewsCountChanged();

	// reimplemented (idoc::CMultiDocumentManagerBase)
	virtual istd::IChangeableSharedPtr OpenSingleDocument(
				const QString& filePath,
				bool createView,
				const QByteArray& viewTypeId,
				QByteArray& documentTypeId,
				bool beQuiet,
				bool* ignoredPtr,
				ibase::IProgressManager* progressManagerPtr) override;

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr) override;

	// reimplemented (TRestorableGuiWrap)
	virtual void OnRestoreSettings(const QSettings& settings) override;
	virtual void OnSaveSettings(QSettings& settings) const override;

	// reimplemented (idoc::CMultiDocumentManagerBase)
	virtual void CloseAllDocuments() override;
	virtual QStringList GetOpenFilePaths(const QByteArray* documentTypeIdPtr = NULL) const override;
	virtual void OnViewRegistered(istd::IPolymorphic* viewPtr, const SingleDocumentData& documentData) override;
	virtual void OnViewRemoved(istd::IPolymorphic* viewPtr) override;
	virtual bool QueryDocumentSave(const SingleDocumentData& info, bool* ignoredPtr) override;

	// reimplemented (iqt:CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;
	virtual void OnRetranslate() override;
	virtual void OnGuiRetranslate() override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

	// reimplemented (istd:IChangeable)
	virtual void OnEndChanges(const ChangeSet& changeSet) override;

protected Q_SLOTS:
	void OnWindowActivated(QMdiSubWindow* window);
	void OnCloseAllViews();

private:
	class DocumentSelectionInfo: virtual public iprm::ISelectionParam, virtual public iprm::IOptionsList
	{
	public:
		DocumentSelectionInfo();

		void SetParent(CMultiDocumentWorkspaceGuiComp& parent);

		// reimplemented (iprm::ISelectionParam)
		virtual const iprm::IOptionsList* GetSelectionConstraints() const override;
		virtual int GetSelectedOptionIndex() const override;
		virtual bool SetSelectedOptionIndex(int index) override;
		virtual iprm::ISelectionParam* GetSubselection(int index) const override;

		// reimplemented (iprm::IOptionsList)
		virtual int GetOptionsFlags() const override;
		virtual int GetOptionsCount() const override;
		virtual QString GetOptionName(int index) const override;
		virtual QString GetOptionDescription(int index) const override;
		virtual QByteArray GetOptionId(int index) const override;
		virtual bool IsOptionEnabled(int index) const override;

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive) override;

	private:
		int m_selectedDocumentIndex;
		CMultiDocumentWorkspaceGuiComp* m_parent;
	};

	friend class imod::TModelWrap<DocumentSelectionInfo>;

	// static template methods for subelement access
	template <class InterfaceType>
	static InterfaceType* ExtractSelectionInfo(CMultiDocumentWorkspaceGuiComp& component)
	{
		return &component.m_documentSelectionInfo;
	}

	iqtgui::CHierarchicalCommand m_commands;

	// global commands
	iqtgui::CHierarchicalCommand m_windowCommand;

	// window menu group
	iqtgui::CHierarchicalCommand m_closeAllDocumentsCommand;

	mutable QString m_lastDirectory;

	int m_viewsCount;

	imod::TModelWrap<DocumentSelectionInfo> m_documentSelectionInfo;

	QString m_organizationName;
	QString m_applicationName;

	bool m_forceQuietClose;

	I_REF(idoc::IDocumentTemplate, m_documentTemplateCompPtr);
	I_REF(iprm::IEnableableParam, m_rememberOpenDocumentsParamPtr);
	I_ATTR(bool, m_showMaximizedAttrPtr);
	I_ATTR(bool, m_showPathAsTipAttrPtr);
	I_ATTR(bool, m_allowViewRepeatingAttrPtr);
	I_ATTR(QString, m_workspaceBackgroundColorAttrPtr);
	I_ATTR(QByteArray, m_defaultCreatedDocumentTypeIdAttrPtr);
};


} // namespace iqtdoc


