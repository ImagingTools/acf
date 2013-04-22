#ifndef iloggui_CTextLogGuiComp_included
#define iloggui_CTextLogGuiComp_included


// ACF includes
#include "ilog/IMessageContainer.h"
#include "ilog/CMessage.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iloggui/Generated/ui_CTextLogGuiComp.h"


namespace iloggui
{


class CTextLogGuiComp:
	public iqtgui::TDesignerGuiObserverCompBase<Ui::CTextLogGuiComp, ilog::IMessageContainer>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CTextLogGuiComp, ilog::IMessageContainer> BaseClass;

	I_BEGIN_COMPONENT(CTextLogGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
};


} // namespace iloggui


#endif // !iloggui_CTextLogGuiComp_included

