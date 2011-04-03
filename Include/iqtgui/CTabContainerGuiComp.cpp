#include "iqtgui/CTabContainerGuiComp.h"


// Qt includes
#include <QGridLayout>
#include <QFrame>


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::CComponentBase)

void CTabContainerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	I_ASSERT(IsGuiCreated());

	QTabWidget* widgetPtr = GetQtWidget();
	I_ASSERT(widgetPtr != NULL);
	if (widgetPtr == NULL){
		return;
	}

	if (*m_useTriangularTabsAttrPtr){
		widgetPtr->setTabShape(QTabWidget::Triangular);
	}

	widgetPtr->setTabPosition(QTabWidget::TabPosition(*m_tabOrientationAttrPtr));

	if (m_iconSizeAttrPtr.IsValid()){
		QSize iconSize = QSize(m_iconSizeAttrPtr->GetValue(), m_iconSizeAttrPtr->GetValue());
		if (!iconSize.isNull() && iconSize.isValid() && !iconSize.isEmpty()){
			widgetPtr->setIconSize(iconSize);
		}
	}

	if (m_tabNamesAttrPtr.IsValid()){
		int tabCount = istd::Min(m_tabNamesAttrPtr.GetCount(), m_slaveWidgetsCompPtr.GetCount());
		for (int tabIndex = 0; tabIndex < tabCount; tabIndex++){
			QString tabName = iqt::GetQString(m_tabNamesAttrPtr[tabIndex]);

			iqtgui::IGuiObject* guiPtr = m_slaveWidgetsCompPtr[tabIndex];
			if ((guiPtr == NULL) || !guiPtr->CreateGui(widgetPtr)){
				continue;
			}

			QWidget* pageWidget = new QWidget(widgetPtr);
			QVBoxLayout* pageLayout = new QVBoxLayout(pageWidget);

			pageLayout->addWidget(guiPtr->GetWidget());

			int addTabIndex = widgetPtr->addTab(pageWidget, tabName);

			if (m_iconsProviderCompPtr.IsValid()){
				int iconCount = m_iconsProviderCompPtr->GetIconCount();			
				if (tabIndex < iconCount){
					QIcon icon = m_iconsProviderCompPtr->GetIcon(tabIndex);

					widgetPtr->setTabIcon(addTabIndex, icon);
				}
			}
		}
	}

	// setup the corner widget:
	if (m_cornerGuiCompPtr.IsValid()){
		if (m_cornerGuiCompPtr->CreateGui(NULL)){
			widgetPtr->setCornerWidget(m_cornerGuiCompPtr->GetWidget());
		}
	}
}


void CTabContainerGuiComp::OnGuiDestroyed()
{
	I_ASSERT(m_slaveWidgetsCompPtr.IsValid());

	if (m_slaveWidgetsCompPtr.IsValid()){
		int slaveWidgetsCount = m_slaveWidgetsCompPtr.GetCount();
		for (int widgetIndex = 0; widgetIndex < slaveWidgetsCount; widgetIndex++){
			m_slaveWidgetsCompPtr[widgetIndex]->DestroyGui();
		}
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtgui


