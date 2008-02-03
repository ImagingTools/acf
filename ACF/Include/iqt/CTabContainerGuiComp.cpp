#include <QGridLayout>

#include "QtAcf/QtAcf.h"


#include "iqt/CTabContainerGuiComp.h"


namespace iqt
{


CTabContainerGuiComp::CTabContainerGuiComp()
	:BaseClass(), 
	m_slaveGuisCompIfPtr(this, "SlaveGuiComponents")
{

}


CTabContainerGuiComp::~CTabContainerGuiComp()
{

}


// reimplemented (acf::QtAbstractGuiComponent)

void CTabContainerGuiComp::initializeGui()
{
	if (m_widget != NULL){
		int tabCount = m_slaveGuisCompIfPtr.dependencyCount();
		for (int tabIndex = 0; tabIndex < tabCount; tabIndex++){
			acf::QtGuiInterface* guiPtr = m_slaveGuisCompIfPtr.interfacePtr(tabIndex);
			if (guiPtr != NULL){
				QWidget* tab = new QWidget(m_widget);

				guiPtr->attachTo(tab);
			}
		}
	}
}


void CTabContainerGuiComp::destroyGui()
{

}


} // namespace iqt
