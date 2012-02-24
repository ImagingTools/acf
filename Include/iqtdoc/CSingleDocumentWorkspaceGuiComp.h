#ifndef iqtdoc_CSingleDocumentWorkspaceGuiComp_included
#define iqtdoc_CSingleDocumentWorkspaceGuiComp_included


// Qt includes
#include <QWidget>


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
	virtual QString GetOpenFilePath(const std::string* documentTypeIdPtr = NULL) const;
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

private:

	I_REF(idoc::IDocumentTemplate, m_documentTemplateCompPtr);

	iqtgui::IGuiObject* m_lastViewPtr;
};


} // namespace iqtdoc


#endif // !iqtdoc_CSingleDocumentWorkspaceGuiComp_included

