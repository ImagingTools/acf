#ifndef iqtgui_TDesignerGuiObserverCompBase_included
#define iqtgui_TDesignerGuiObserverCompBase_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

// ACF includes
#include <imod/TSingleModelObserverBase.h>
#include <ibase/TModelObserverCompWrap.h>
#include <iqtgui/TGuiObserverWrap.h>
#include <iqtgui/TDesignerGuiCompBase.h>


namespace iqtgui
{


/**
	Base class for all Qt GUI components.

	\ingroup ModelObserver
 */
template <class UI, class Model, class WidgetType = QWidget>
class TDesignerGuiObserverCompBase:
			public ibase::TModelObserverCompBaseWrap<
						iqtgui::TGuiObserverWrap< iqtgui::TDesignerGuiCompBase<UI, WidgetType>, imod::TSingleModelObserverBase<Model> > >
{
public:
	typedef ibase::TModelObserverCompBaseWrap<
				iqtgui::TGuiObserverWrap< iqtgui::TDesignerGuiCompBase<UI, WidgetType>, imod::TSingleModelObserverBase<Model> > > BaseClass;

	I_BEGIN_BASE_COMPONENT(TDesignerGuiObserverCompBase);
		I_REGISTER_INTERFACE(imod::IModelEditor);
		I_ASSIGN(m_isReadOnlyAttrPtr, "EditorReadOnly", "If enabled, the editor for the data model is always read only", false, false);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void SetReadOnly(bool state);

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(bool, m_isReadOnlyAttrPtr);
};


// public methods

// reimplemented (imod::IModelEditor)

template <class UI, class Model, class WidgetType>
void TDesignerGuiObserverCompBase<UI, Model, WidgetType>::SetReadOnly(bool state)
{
	// Skip state changes for "permanent-read-only" configured editors:
	if (m_isReadOnlyAttrPtr.IsValid() && *m_isReadOnlyAttrPtr){
		return;
	}
	
	BaseClass::SetReadOnly(state);
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

template <class UI, class Model, class WidgetType>
void TDesignerGuiObserverCompBase<UI, Model, WidgetType>::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_isReadOnlyAttrPtr.IsValid() && *m_isReadOnlyAttrPtr){
		BaseClass::SetReadOnly(true);
	}
}


// reimplemented (icomp::CComponentBase)

template <class UI, class Model, class WidgetType>
void TDesignerGuiObserverCompBase<UI, Model, WidgetType>::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_isReadOnlyAttrPtr.IsValid()){
		this->m_isReadOnly = *m_isReadOnlyAttrPtr;
	}
}


} // namespace iqtgui


#endif // !iqtgui_TDesignerGuiObserverCompBase_included


