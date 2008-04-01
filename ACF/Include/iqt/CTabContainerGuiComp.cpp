#ifdef OLD_ACF_SUPPORT

#include <QGridLayout>
#include <QFrame>

#include "iqt/CTabContainerGuiComp.h"


namespace iqt
{


CTabContainerGuiComp::CTabContainerGuiComp()
:	BaseClass(), 
	m_slaveGuisCompIfPtr(this, "SlaveGuiComponents"),
	m_cornerGuiCompIfPtr(this, "CornerWidget"),
	m_tabNamesAttr(this, "TabNames"),
	m_iconsProviderCompIfPtr(this, "TabIcons"),
	m_iconSizeAttr(16, this, "IconSize"),
	m_useTriangularTabsAttr(false, this, "UseTriangularTabs"),
	m_tabOrientationAttr(0, this, "TabOrientation")
{
}


CTabContainerGuiComp::~CTabContainerGuiComp()
{
}


// reimplemented (acf::QtAbstractGuiComponent)

void CTabContainerGuiComp::OnGuiCreated()
{
	if (m_widget != NULL){
		if (m_useTriangularTabsAttr.GetValue()){
			m_widget->setTabShape(QTabWidget::Triangular);
		}

		m_widget->setTabPosition(QTabWidget::TabPosition(m_tabOrientationAttr.GetValue()));

		QSize iconSize = QSize(m_iconSizeAttr.GetValue(), m_iconSizeAttr.GetValue());
		if (!iconSize.isNull() && iconSize.isValid() && !iconSize.isEmpty()){
			m_widget->setIconSize(iconSize);
		}

		int tabCount = m_tabNamesAttr.GetCount();
		for (int tabIndex = 0; tabIndex < tabCount; tabIndex++){
			QString tabName = iqt::GetQString(m_tabNamesAttr.GetValue(tabIndex));

			QWidget* tab = new QWidget(m_widget);
			int addTabIndex = m_widget->addTab(tab, tabName);
			
			bool isAttached = false;
			if (tabIndex < m_slaveGuisCompIfPtr.dependencyCount()){
				iqt::IGuiObject* guiPtr = m_slaveGuisCompIfPtr.GetInterfacePtr(tabIndex);
				if (guiPtr != NULL){
					guiPtr->AttachTo(tab);

					isAttached = true;
				}
			}
			
			if (m_iconsProviderCompIfPtr.IsValid()){
				int iconCount = m_iconsProviderCompIfPtr->iconCount();			
				if (tabIndex < iconCount){
					QIcon icon = m_iconsProviderCompIfPtr->getIcon(tabIndex);

					m_widget-> setTabIcon(addTabIndex, icon);
				}
			}

			if (!isAttached){
				m_widget->setTabEnabled(addTabIndex, false);
			}
		}
		
		// setup the corner widget:
		if (m_cornerGuiCompIfPtr.IsValid()){
			QFrame* cornerFrame = new QFrame(m_widget);
			m_widget->setCornerWidget(cornerFrame);

			m_cornerGuiCompIfPtr->AttachTo(cornerFrame);
		}
	}
}


void CTabContainerGuiComp::OnGuiDestroyed()
{
}


} // namespace iqt


#endif // OLD_ACF_SUPPORT
