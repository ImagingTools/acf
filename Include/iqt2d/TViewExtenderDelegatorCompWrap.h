#ifndef iqt2d_TViewExtenderDelegatorCompWrap_included
#define iqt2d_TViewExtenderDelegatorCompWrap_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "iqt2d/IViewExtender.h"


namespace iqt2d
{


template<class Base>
class TViewExtenderDelegatorCompWrap:
			public Base,
			virtual public iqt2d::IViewExtender
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(TViewExtenderDelegatorCompWrap);
		I_REGISTER_INTERFACE(iqt2d::IViewExtender);
		I_ASSIGN(m_slaveExtenderCompPtr, "SlaveSceneExtender", "Slave scene extender", false, "SlaveSceneExtender");
	I_END_COMPONENT;

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr);

private:
	I_REF(iqt2d::IViewExtender, m_slaveExtenderCompPtr);
};



// reimplemented (iqt2d::IViewExtender)

template<class Base>
void TViewExtenderDelegatorCompWrap<Base>::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	if (m_slaveExtenderCompPtr.IsValid()){
		m_slaveExtenderCompPtr->AddItemsToScene(providerPtr, flags);
	}
}


template<class Base>
void TViewExtenderDelegatorCompWrap<Base>::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	if (m_slaveExtenderCompPtr.IsValid()){
		m_slaveExtenderCompPtr->RemoveItemsFromScene(providerPtr);
	}
}


typedef TViewExtenderDelegatorCompWrap<icomp::CComponentBase> CViewExtenderDelegatorComp;


} // namespace iqt2d


#endif // !iqt2d_TViewExtenderDelegatorCompWrap_included


