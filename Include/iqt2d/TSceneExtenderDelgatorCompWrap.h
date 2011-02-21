#ifndef iqt2d_TSceneExtenderDelgatorCompWrap_included
#define iqt2d_TSceneExtenderDelgatorCompWrap_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "iqt2d/ISceneExtender.h"


namespace iqt2d
{


template<class Base>
class TSceneExtenderDelgatorCompWrap:
			public Base,
			virtual public iqt2d::ISceneExtender
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(TSceneExtenderDelgatorCompWrap)
		I_REGISTER_INTERFACE(iqt2d::ISceneExtender)
		I_ASSIGN(m_slaveExtenderCompPtr, "SlaveSceneExtender", "Slave scene extender", false, "SlaveSceneExtender");
	I_END_COMPONENT

	// reimplemented (iqt2d::ISceneExtender)
	virtual void AddItemsToScene(iqt2d::ISceneProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::ISceneProvider* providerPtr);

private:
	I_REF(iqt2d::ISceneExtender, m_slaveExtenderCompPtr);
};



// reimplemented (iqt2d::ISceneExtender)

template<class Base>
void TSceneExtenderDelgatorCompWrap<Base>::AddItemsToScene(iqt2d::ISceneProvider* providerPtr, int flags)
{
	if (m_slaveExtenderCompPtr.IsValid()){
		m_slaveExtenderCompPtr->AddItemsToScene(providerPtr, flags);
	}
}


template<class Base>
void TSceneExtenderDelgatorCompWrap<Base>::RemoveItemsFromScene(iqt2d::ISceneProvider* providerPtr)
{
	if (m_slaveExtenderCompPtr.IsValid()){
		m_slaveExtenderCompPtr->RemoveItemsFromScene(providerPtr);
	}
}


typedef TSceneExtenderDelgatorCompWrap<icomp::CComponentBase> CSceneExtenderDelgatorComp;


} // namespace iqt2d


#endif // !iqt2d_TSceneExtenderDelgatorCompWrap_included


