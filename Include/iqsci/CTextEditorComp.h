#ifndef iqsci_CTextEditorComp_included
#define iqsci_CTextEditorComp_included


// ACF includes
#include "imod/TSingleModelObserverBase.h"

#include "ibase/ITextDocument.h"

#include "idoc/ICommandsProvider.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TGuiObserverWrap.h"

#include "iqsci/CTextEditor.h"


namespace iqsci
{


class CTextEditorComp: public iqtgui::TGuiObserverWrap<
			iqtgui::TGuiComponentBase<CTextEditor>, 
			imod::TSingleModelObserverBase<ibase::ITextDocument> >
{
	Q_OBJECT

public:
	typedef iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<CTextEditor>,
				imod::TSingleModelObserverBase<ibase::ITextDocument> > BaseClass;

	I_BEGIN_COMPONENT(CTextEditorComp)
		I_REGISTER_INTERFACE(imod::IObserver)
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

protected slots:
	virtual void OnTextChanged();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnRetranslate();
};


} // namespace iqsci


#endif // !iqsci_CTextEditorComp_included


