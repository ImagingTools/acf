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
	m_iconSizeAttr(16, this, "IconSize")
{
}


CTabContainerGuiComp::~CTabContainerGuiComp()
{
}


// reimplemented (acf::QtAbstractGuiComponent)

void CTabContainerGuiComp::initializeGui()
{
	if (m_widget != NULL){
		QSize iconSize = QSize(m_iconSizeAttr.value(), m_iconSizeAttr.value());
		if (!iconSize.isNull() && iconSize.isValid() && !iconSize.isEmpty()){
			m_widget->setIconSize(iconSize);
		}

		int tabCount = m_tabNamesAttr.count();
		for (int tabIndex = 0; tabIndex < tabCount; tabIndex++){
			QString tabName = iqt::GetQString(m_tabNamesAttr.value(tabIndex));

			QWidget* tab = new QWidget(m_widget);
			int addTabIndex = m_widget->addTab(tab, tabName);
			
			bool isAttached = false;
			if (tabIndex < m_slaveGuisCompIfPtr.dependencyCount()){
				acf::QtGuiInterface* guiPtr = m_slaveGuisCompIfPtr.interfacePtr(tabIndex);
				if (guiPtr != NULL){
					guiPtr->attachTo(tab);

					isAttached = true;
				}
			}
			
			if (m_iconsProviderCompIfPtr.isValid()){
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
		if (m_cornerGuiCompIfPtr.isValid()){
			QFrame* cornerFrame = new QFrame(m_widget);
			m_widget->setCornerWidget(cornerFrame);

			m_cornerGuiCompIfPtr->attachTo(cornerFrame);
		}
	}
}


void CTabContainerGuiComp::destroyGui()
{
}


} // namespace iqt


#endif // OLD_ACF_SUPPORT
