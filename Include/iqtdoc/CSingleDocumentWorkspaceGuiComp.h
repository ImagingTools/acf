#ifndef iqtdoc_CSingleDocumentWorkspaceGuiComp_included
#define iqtdoc_CSingleDocumentWorkspaceGuiComp_included


// Qt includes
#include <QtGui/QWidget>

// ACF includes
#include "ibase/ICommandsProvider.h"
#include "idoc/CSingleDocumentManagerBase.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TRestorableGuiWrap.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "iqtdoc/TQtDocumentManagerWrap.h"


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
public:
	typedef iqtdoc::TQtDocumentManagerWrap<
						idoc::CSingleDocumentManagerBase,
						iqtgui::TRestorableGuiWrap<
									iqtgui::TGuiComponentBase<QWidget> > > BaseClass;

	I_BEGIN_COMPONENT(CSingleDocumentWorkspaceGuiComp);
		I_REGISTER_INTERFACE(idoc::IDocumentManager);
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

	// reimplemented (iqtgui::IGuiObject)
	virtual void OnTryClose(bool* ignoredPtr = NULL);

protected:
	/**
		Update titles of views or all views of specified document.
		\param	optional document object, if you want to update only views of single document.
	*/
	void UpdateTitle();

	// reimplemented (idoc::CSingleDocumentManagerBase)
	virtual QString GetOpenFilePath(const QByteArray* documentTypeIdPtr = NULL) const;
	virtual void OnViewRegistered(istd::IPolymorphic* viewPtr);
	virtual void OnViewRemoved(istd::IPolymorphic* viewPtr);
	virtual void QueryDocumentClose(bool* ignoredPtr);

	// reimplemented (iqt:CGuiComponentBase)
	virtual void OnGuiCreated();	
	virtual void OnGuiDestroyed();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

	// reimplemented (istd:IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

	// reimplemented (TRestorableGuiWrap)
	virtual void OnRestoreSettings(const QSettings& settings);

private:
	I_REF(idoc::IDocumentTemplate, m_documentTemplateCompPtr);
	I_REF(iprm::IEnableableParam, m_rememberOpenDocumentParamPtr);

	iqtgui::IGuiObject* m_lastViewPtr;

	QString m_organizationName;
	QString m_applicationName;
};


} // namespace iqtdoc


#endif // !iqtdoc_CSingleDocumentWorkspaceGuiComp_included

