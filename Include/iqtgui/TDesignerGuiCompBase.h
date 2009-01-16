#ifndef iqtgui_TDesignerGuiComponentBase_included
#define iqtgui_TDesignerGuiComponentBase_included


#include "qmetaobject.h"

#include "iqtgui/TGuiComponentBase.h"


namespace iqtgui
{


/**
	Base class for all Qt GUI componentes.
 */
template <class UI, class WidgetType = QWidget>
class TDesignerGuiCompBase: public TGuiComponentBase<WidgetType>, public UI
{
public:
	typedef TGuiComponentBase<WidgetType> BaseClass;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual QWidget* InitWidgetToParent(QWidget* parentPtr);

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnRetranslate();
};


// public methods

// reimplemented (iqtgui::CGuiComponentBase)

template <class UI, class WidgetType>
QWidget* TDesignerGuiCompBase<UI, WidgetType>::InitWidgetToParent(QWidget* parentPtr)
{
	I_ASSERT(!BaseClass::IsGuiCreated());

	QWidget* widgetPtr = BaseClass::InitWidgetToParent(parentPtr);

	UI::setupUi(widgetPtr);

	return widgetPtr;
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

template <class UI, class WidgetType>
void TDesignerGuiCompBase<UI, WidgetType>::OnRetranslate()
{
	QWidget* widgetPtr = BaseClass::GetWidget();

	if (widgetPtr != NULL){
		UI::retranslateUi(widgetPtr);
	}

	BaseClass::OnRetranslate();
}


} // namespace iqtgui


#endif // !iqtgui_TDesignerGuiComponentBase_included


