#ifndef iqtgui_TControlledWidgetWrap_included
#define iqtgui_TControlledWidgetWrap_included


#include <QWidget>
#include <QCloseEvent>


#include "iqtgui/CGuiComponentBase.h"


namespace iqtgui
{


/**
	Base class for all Qt GUI componentes.
 */
template <class WidgetType>
class TControlledWidgetWrap: public WidgetType
{
public:
	typedef WidgetType BaseClass;

	TControlledWidgetWrap(CGuiComponentBase& ownerComponent, QWidget* parent = NULL);

protected:
	// reimplemented (QWidget)
	virtual void closeEvent(QCloseEvent* eventPtr);

private:
	CGuiComponentBase& m_ownerComponent;
};


// public methods

template <class WidgetType>
TControlledWidgetWrap<WidgetType>::TControlledWidgetWrap(CGuiComponentBase& ownerComponent, QWidget* parent)
	:BaseClass(parent),
	m_ownerComponent(ownerComponent)
{
}


// protected methods

// reimplemented (QWidget)

template <class WidgetType>
void TControlledWidgetWrap<WidgetType>::closeEvent(QCloseEvent* eventPtr)
{
	bool ignoreClose;
	m_ownerComponent.OnTryClose(&ignoreClose);
	if (ignoreClose){
		eventPtr->ignore();
	}
	else{
		eventPtr->accept();
	}
}


} // namespace iqtgui


#endif // !iqtgui_TControlledWidgetWrap_included


