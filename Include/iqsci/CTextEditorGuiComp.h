#ifndef iqsci_CTextEditorGuiComp_included
#define iqsci_CTextEditorGuiComp_included


// ACF includes
#include "imod/TSingleModelObserverBase.h"

#include "ibase/ITextDocument.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TGuiObserverWrap.h"

#include "iqsci/CTextEditor.h"


namespace iqsci
{


class CTextEditorGuiComp: public iqtgui::TGuiObserverWrap<
			iqtgui::TGuiComponentBase<CTextEditor>, 
			imod::TSingleModelObserverBase<ibase::ITextDocument> >,
			virtual public idoc::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<CTextEditor>,
				imod::TSingleModelObserverBase<ibase::ITextDocument> > BaseClass;

	I_BEGIN_COMPONENT(CTextEditorGuiComp)
		I_REGISTER_INTERFACE(idoc::ICommandsProvider)
		I_REGISTER_INTERFACE(imod::IObserver)
		I_ASSIGN(m_languageAttrPtr, "Language", "Use syntax highlighting for defined language", false, "Language");
		I_ASSIGN(m_useFoldingAttrPtr, "UseFolding", "Use folding in the text document", false, true);
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

protected slots:
	virtual void OnTextChanged();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnRetranslate();

private:
	I_ATTR(bool, m_useFoldingAttrPtr);
	I_ATTR(istd::CString, m_languageAttrPtr);
};


} // namespace iqsci


#endif // !iqsci_CTextEditorGuiComp_included


