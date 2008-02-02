#ifndef iqtstd_CTabContainerGuiComp_h_included
#define iqtstd_CTabContainerGuiComp_h_included


#include <QTabWidget>


#include "iqtstd.h"

#include "QtAcf/QtGuiComponent.h"

#include "Comp/MultipleComponentDependency.h"
#include "Comp/Attribute.h"


namespace iqtstd
{


class CTabContainerGuiComp: public acf::QtGuiComponent<QTabWidget> 
{
public:
	typedef acf::QtGuiComponent<QTabWidget> BaseClass;

	CTabContainerGuiComp();
	virtual ~CTabContainerGuiComp();

protected:
	// reimplemented (acf::QtAbstractGuiComponent)
	virtual void initializeGui();
	virtual void destroyGui();

protected:
	acf::MultipleComponentDependency<acf::QtGuiInterface> m_slaveGuisCompIfPtr;
};


} // namespace iqtstd


#endif // !iqtstd_CTabContainerGuiComp_h_included

