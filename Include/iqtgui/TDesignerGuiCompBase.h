#ifndef iqtgui_TDesignerGuiCompBase_included
#define iqtgui_TDesignerGuiCompBase_included


// Qt includes
#include "qmetaobject.h"


// ACF includes
#include "iqtgui/TGuiComponentBase.h"


namespace iqtgui
{


/**
	Base class for all Qt GUI componentes, which use the Qt designer's UI.
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
	virtual void OnGuiRetranslate();
};


// public methods

// reimplemented (iqtgui::CGuiComponentBase)

template <class UI, class WidgetType>
QWidget* TDesignerGuiCompBase<UI, WidgetType>::InitWidgetToParent(QWidget* parentPtr)
{
	I_ASSERT(!BaseClass::IsGuiCreated());

	QWidget* widgetPtr = BaseClass::InitWidgetToParent(parentPtr);

	WidgetType* concreteWidgetPtr = dynamic_cast<WidgetType*>(widgetPtr);
	I_ASSERT(concreteWidgetPtr != NULL);

	UI::setupUi(concreteWidgetPtr);

	return concreteWidgetPtr;
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

template <class UI, class WidgetType>
void TDesignerGuiCompBase<UI, WidgetType>::OnGuiRetranslate()
{
	WidgetType* widgetPtr = dynamic_cast<WidgetType*>(BaseClass::GetWidget());
	if (widgetPtr != NULL){
		UI::retranslateUi(widgetPtr);
	}

	BaseClass::OnGuiRetranslate();
}


} // namespace iqtgui


#endif // !iqtgui_TDesignerGuiCompBase_included


