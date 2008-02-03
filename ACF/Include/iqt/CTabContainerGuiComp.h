#ifndef iqt_CTabContainerGuiComp_h_included
#define iqt_CTabContainerGuiComp_h_included


#include "iqt.h"


#include <QTabWidget>

#include "QtAcf/QtGuiComponent.h"

#include "Comp/MultipleComponentDependency.h"
#include "Comp/Attribute.h"


namespace iqt
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


} // namespace iqt


#endif // !iqt_CTabContainerGuiComp_h_included

