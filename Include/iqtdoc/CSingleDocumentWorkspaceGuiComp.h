#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

// ACF includes
#include <ibase/ICommandsProvider.h>
#include <idoc/IDocumentTemplate.h>
#include <idoc/CSingleDocumentManagerBase.h>
#include <iqtgui/TGuiComponentBase.h>
#include <iqtgui/TRestorableGuiWrap.h>
#include <iqtgui/CHierarchicalCommand.h>
#include <iqtdoc/TQtDocumentManagerWrap.h>


namespace iqtdoc
{


/**
	Qt-based workspace implementation of a single document manager.
*/
class CSingleDocumentWorkspaceGuiComp:
			public iqtdoc::TQtDocumentManagerWrap<
						idoc::CSingleDocumentManagerBase,
						iqtgui::TRestorableGuiWrap<
									iqtgui::TGuiComponentBase<QWidget> > >
{
	Q_OBJECT

public:
	typedef iqtdoc::TQtDocumentManagerWrap<
						idoc::CSingleDocumentManagerBase,
						iqtgui::TRestorableGuiWrap<
									iqtgui::TGuiComponentBase<QWidget> > > BaseClass;

	I_BEGIN_COMPONENT(CSingleDocumentWorkspaceGuiComp);
		I_REGISTER_INTERFACE(idoc::IDocumentManager);
		I_ASSIGN(m_defaultDocumentTitleAttrPtr, "DefaultDocumentTitle", "Name of the untitled document", true, "<no name>");
		I_ASSIGN(m_documentTemplateCompPtr, "DocumentTemplate", "Document template", true, "DocumentTemplate");
		I_ASSIGN(m_rememberOpenDocumentParamPtr, "RememberOpenDocumentOnExit", "If enabled, restores open document from previous session", false, "RememberOpenDocumentOnExit");
	I_END_COMPONENT;

	enum GroupId
	{
		GI_WINDOW = 0x300,
		GI_DOCUMENT,
		GI_VIEW
	};

	CSingleDocumentWorkspaceGuiComp();

	// reimplemented (idoc::IDocumentManager)
	virtual void SetActiveView(istd::IPolymorphic* viewPtr) override;

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnTryClose(bool* ignoredPtr = NULL) override;

protected:
	/**
		Update titles of views or all views of specified document.
	*/
	virtual void UpdateTitle();

	/**
		Generate the title of the current document.
	*/
	virtual QString GenerateDocumentTitle() const;

	// reimplemented (idoc::CSingleDocumentManagerBase)
	virtual QString GetOpenFilePath(const QByteArray* documentTypeIdPtr = NULL) const override;
	virtual void OnViewRegistered(istd::IPolymorphic* viewPtr) override;
	virtual void OnViewRemoved(istd::IPolymorphic* viewPtr) override;
	virtual void OnDocumentRegistered() override;
	virtual void OnDocumentClosed() override;
	virtual bool QueryDocumentSave(bool* ignoredPtr) override;

	// reimplemented (iqt:CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

	// reimplemented (istd:IChangeable)
	virtual void OnEndChanges(const ChangeSet& changeSet) override;

	// reimplemented (TRestorableGuiWrap)
	virtual void OnRestoreSettings(const QSettings& settings) override;

protected:
	I_TEXTATTR(m_defaultDocumentTitleAttrPtr);
	I_REF(idoc::IDocumentTemplate, m_documentTemplateCompPtr);
	I_REF(iprm::IEnableableParam, m_rememberOpenDocumentParamPtr);

	iqtgui::IGuiObject* m_lastViewPtr;

	QString m_organizationName;
	QString m_applicationName;
};


} // namespace iqtdoc


