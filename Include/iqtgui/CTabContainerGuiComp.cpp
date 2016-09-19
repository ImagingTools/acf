#include "iqtgui/CTabContainerGuiComp.h"


// Qt includes
#if QT_VERSION >= 0x050000
#include <QtWidgets/QTabWidget>
#else
#include <QtGui/QTabWidget>
#endif


namespace iqtgui
{


// protected methods

// reimplemented (iqtgui::CMultiPageGuiCompBase)

int CTabContainerGuiComp::GetPagesCount() const
{
	return m_slaveWidgetsCompPtr.GetCount();
}


iqtgui::IGuiObject* CTabContainerGuiComp::GetPageGuiComponent(int pageIndex) const
{
	Q_ASSERT(pageIndex >= 0);
	Q_ASSERT(pageIndex < m_slaveWidgetsCompPtr.GetCount());

	return m_slaveWidgetsCompPtr[pageIndex];
}


int CTabContainerGuiComp::GetDesignType() const
{
	return iwidgets::CMultiPageWidget::DT_TAB_WIDGET;
}


// reimplemented (iqtgui::CGuiComponentBase)

void CTabContainerGuiComp::OnGuiCreated()
{
	Q_ASSERT(IsGuiCreated());

	BaseClass::OnGuiCreated();

	iwidgets::CMultiPageWidget* widgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(GetWidget());
	Q_ASSERT(widgetPtr != NULL);

	QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(widgetPtr->GetContainerWidgetPtr());
	if (tabWidgetPtr != NULL){
		if (*m_useTriangularTabsAttrPtr){
			tabWidgetPtr->setTabShape(QTabWidget::Triangular);
		}

		tabWidgetPtr->setTabPosition(QTabWidget::TabPosition(*m_tabOrientationAttrPtr));

		tabWidgetPtr->setDocumentMode(*m_useDocumentModeAttrPtr);

		// setup the corner widget:
		if (m_cornerGuiCompPtr.IsValid()){
			if (m_cornerGuiCompPtr->CreateGui(NULL)){
				tabWidgetPtr->setCornerWidget(m_cornerGuiCompPtr->GetWidget());
			}
		}
	}
}


void CTabContainerGuiComp::OnGuiDestroyed()
{
	if (m_cornerGuiCompPtr.IsValid()){
		iwidgets::CMultiPageWidget* widgetPtr = dynamic_cast<iwidgets::CMultiPageWidget*>(GetWidget());
		Q_ASSERT(widgetPtr != NULL);
		QTabWidget* tabWidgetPtr = dynamic_cast<QTabWidget*>(widgetPtr->GetContainerWidgetPtr());
		if (tabWidgetPtr != NULL){
			tabWidgetPtr->setCornerWidget(NULL);
		}

		if (m_cornerGuiCompPtr->IsGuiCreated()){
			m_cornerGuiCompPtr->DestroyGui();
		}
	}
	BaseClass::OnGuiDestroyed();
}


} // namespace iqtgui


