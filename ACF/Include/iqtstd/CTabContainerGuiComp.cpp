#include <QGridLayout>

#include "QtAcf/QtAcf.h"


#include "iqtstd/CTabContainerGuiComp.h"


namespace iqtstd
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
	BaseClass::initializeGui();

	if (m_widget != NULL){
		
	}
}


void CTabContainerGuiComp::destroyGui()
{
	BaseClass::destroyGui();
}



} // namespace iqtstd
