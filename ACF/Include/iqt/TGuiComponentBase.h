#ifndef iqt_TGuiComponentBase_included
#define iqt_TGuiComponentBase_included


#include <QWidget>

#include "iqt/CGuiComponentBase.h"


namespace iqt
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

	// reimplemented (iqt::CGuiComponentBase)
	virtual QWidget* InitWidgetToParent(QWidget* parentPtr);
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


// reimplemented (iqt::CGuiComponentBase)

template <class WidgetType>
QWidget* TGuiComponentBase<WidgetType>::InitWidgetToParent(QWidget* parentPtr)
{
	I_ASSERT(!IsGuiCreated());

	WidgetType* widgetPtr = new WidgetType(parentPtr);

	return widgetPtr;
}


} // namespace iqt


#endif //!iqt_TGuiComponentBase_included


