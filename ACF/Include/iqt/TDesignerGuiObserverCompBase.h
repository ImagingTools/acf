#ifndef iqt_TDesignerGuiObserverCompBase_included
#define iqt_TDesignerGuiObserverCompBase_included


#include "iqt/TGuiObserverWrap.h"
#include "iqt/TDesignerGuiCompBase.h"


namespace iqt
{


/**
	Base class for all Qt GUI componentes.
 */
template <class UI, class Observer, class WidgetType = QWidget>
class TDesignerGuiObserverCompBase: public TGuiObserverWrap<TDesignerGuiCompBase<UI, WidgetType>, Observer>
{
public:
	typedef TGuiObserverWrap<TDesignerGuiCompBase<UI, WidgetType>, Observer> BaseClass;
};


} // namespace iqt


#endif //!iqt_TDesignerGuiObserverCompBase_included


