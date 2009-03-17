#ifndef iqtgui_TDesignerBasicGui_included
#define iqtgui_TDesignerBasicGui_included


#include "qmetaobject.h"

#include "iqtgui/TGuiComponentBase.h"


namespace iqtgui
{


/**
	Simple integration of designer-generated GUI with QT widget class.
 */
template <class UI, class WidgetType = QDialog>
class TDesignerBasicGui: public WidgetType, public UI
{
public:
	typedef WidgetType BaseClass;

	TDesignerBasicGui(QWidget* parentPtr = 0, Qt::WindowFlags f = 0);
};


// public methods

// reimplemented (iqtgui::CGuiComponentBase)

template <class UI, class WidgetType>
TDesignerBasicGui<UI, WidgetType>::TDesignerBasicGui(QWidget* parentPtr, Qt::WindowFlags f)
:	BaseClass(parentPtr, f)
{
	UI::setupUi(this);
}


} // namespace iqtgui


#endif // !iqtgui_TDesignerBasicGui_included


