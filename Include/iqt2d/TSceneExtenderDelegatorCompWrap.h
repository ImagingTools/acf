#ifndef iqt2d_TSceneExtenderDelegatorCompWrap_included
#define iqt2d_TSceneExtenderDelegatorCompWrap_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "iqt2d/IViewExtender.h"


namespace iqt2d
{


template<class Base>
class TSceneExtenderDelegatorCompWrap:
			public Base,
			virtual public iqt2d::IViewExtender
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(TSceneExtenderDelegatorCompWrap);
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
void TSceneExtenderDelegatorCompWrap<Base>::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	if (m_slaveExtenderCompPtr.IsValid()){
		m_slaveExtenderCompPtr->AddItemsToScene(providerPtr, flags);
	}
}


template<class Base>
void TSceneExtenderDelegatorCompWrap<Base>::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	if (m_slaveExtenderCompPtr.IsValid()){
		m_slaveExtenderCompPtr->RemoveItemsFromScene(providerPtr);
	}
}


typedef TSceneExtenderDelegatorCompWrap<icomp::CComponentBase> CSceneExtenderDelegatorComp;


} // namespace iqt2d


#endif // !iqt2d_TSceneExtenderDelegatorCompWrap_included


