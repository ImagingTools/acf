#ifndef iqt_TDesignerGuiObserverCompBase_included
#define iqt_TDesignerGuiObserverCompBase_included


#include "imod/TSingleModelObserverBase.h"

#include "iqt/TGuiObserverWrap.h"
#include "iqt/TDesignerGuiCompBase.h"


namespace iqt
{


/**
	Base class for all Qt GUI componentes.
 */
template <class UI, class Model, class WidgetType = QWidget>
class TDesignerGuiObserverCompBase: public TGuiObserverWrap<TDesignerGuiCompBase<UI, WidgetType>, imod::TSingleModelObserverBase<Model> >
{
public:
	typedef TGuiObserverWrap<TDesignerGuiCompBase<UI, WidgetType>, imod::TSingleModelObserverBase<Model> > BaseClass;

	I_BEGIN_BASE_COMPONENT(TDesignerGuiObserverCompBase)
		I_REGISTER_INTERFACE(imod::IModelEditor)
		I_REGISTER_INTERFACE(imod::IObserver)
	I_END_COMPONENT
};


} // namespace iqt


#endif //!iqt_TDesignerGuiObserverCompBase_included


