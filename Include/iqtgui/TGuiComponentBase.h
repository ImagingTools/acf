#ifndef iqtgui_TGuiComponentBase_included
#define iqtgui_TGuiComponentBase_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif


// ACF includes
#include "iqtgui/CGuiComponentBase.h"


namespace iqtgui
{


/**	Base class for all Qt GUI componentes.
 */
template <class WidgetType>
class TGuiComponentBase: public CGuiComponentBase
{
public:
	typedef CGuiComponentBase BaseClass;

	TGuiComponentBase();

	virtual WidgetType* GetQtWidget() const;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual QWidget* CreateQtWidget(QWidget* parentPtr);
};


// public methods

template <class WidgetType>
TGuiComponentBase<WidgetType>::TGuiComponentBase()
{
}


template <class WidgetType>
WidgetType* TGuiComponentBase<WidgetType>::GetQtWidget() const
{
	return dynamic_cast<WidgetType*>(GetWidget());
}


// reimplemented (iqtgui::CGuiComponentBase)

template <class WidgetType>
QWidget* TGuiComponentBase<WidgetType>::CreateQtWidget(QWidget* parentPtr)
{
	Q_ASSERT(!IsGuiCreated());

	WidgetType* widgetPtr = new WidgetType(parentPtr);

	return widgetPtr;
}


} // namespace iqtgui


#endif // !iqtgui_TGuiComponentBase_included


