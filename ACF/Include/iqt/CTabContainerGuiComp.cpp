#include "iqt/CTabContainerGuiComp.h"


#include <QGridLayout>
#include <QFrame>


namespace iqt
{


// public methods

// reimplemented (iqt::CComponentBase)

void CTabContainerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	I_ASSERT(IsGuiCreated());

	QTabWidget* widgetPtr = GetQtWidget();
	I_ASSERT(widgetPtr != NULL);
	if (widgetPtr == NULL){
		return;
	}

	if (m_useTriangularTabsAttrPtr.IsValid() && m_useTriangularTabsAttrPtr->GetValue()){
		widgetPtr->setTabShape(QTabWidget::Triangular);
	}

	if (m_tabOrientationAttrPtr.IsValid()){
		widgetPtr->setTabPosition(QTabWidget::TabPosition(m_tabOrientationAttrPtr->GetValue()));
	}

	if (m_iconSizeAttrPtr.IsValid()){
		QSize iconSize = QSize(m_iconSizeAttrPtr->GetValue(), m_iconSizeAttrPtr->GetValue());
		if (!iconSize.isNull() && iconSize.isValid() && !iconSize.isEmpty()){
			widgetPtr->setIconSize(iconSize);
		}
	}

	if (m_tabNamesAttrPtr.IsValid()){
		int tabCount = m_tabNamesAttrPtr.GetCount();
		for (int tabIndex = 0; tabIndex < tabCount; tabIndex++){
			QString tabName = iqt::GetQString(m_tabNamesAttrPtr[tabIndex]);

			QWidget* tab = new QWidget(widgetPtr);
			int addTabIndex = widgetPtr->addTab(tab, tabName);

			bool isAttached = false;
			if (tabIndex < m_slaveWidgetsCompPtr.GetCount()){
				iqt::IGuiObject* guiPtr = m_slaveWidgetsCompPtr[tabIndex];
				if (guiPtr != NULL){
					isAttached = guiPtr->CreateGui(tab);
				}
			}

			if (m_iconsProviderCompPtr.IsValid()){
				int iconCount = m_iconsProviderCompPtr->GetIconCount();			
				if (tabIndex < iconCount){
					QIcon icon = m_iconsProviderCompPtr->GetIcon(tabIndex);

					widgetPtr->setTabIcon(addTabIndex, icon);
				}
			}

			if (!isAttached){
				widgetPtr->setTabEnabled(addTabIndex, false);
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


} // namespace iqt
