#ifndef iqt2d_TMultiViewExtenderDelegatorCompWrap_included
#define iqt2d_TMultiViewExtenderDelegatorCompWrap_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <iqt2d/IViewExtender.h>


namespace iqt2d
{


/**
	Iterates over the list of the scene extenders to show their shapes on the same view provider.
*/
template<class Base>
class TMultiViewExtenderDelegatorCompWrap:
			public Base,
			virtual public iqt2d::IViewExtender
{
public:
	typedef Base BaseClass;

	I_BEGIN_COMPONENT(TMultiViewExtenderDelegatorCompWrap);
		I_REGISTER_INTERFACE(iqt2d::IViewExtender);
		I_ASSIGN_MULTI_0(m_slaveExtendersCompPtr, "SlaveSceneExtenders", "List of the slave scene extenders", false);
	I_END_COMPONENT;

	// reimplemented (iqt2d::IViewExtender)
	virtual void AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr);

private:
	I_MULTIREF(iqt2d::IViewExtender, m_slaveExtendersCompPtr);
};


// reimplemented (iqt2d::IViewExtender)

template<class Base>
void TMultiViewExtenderDelegatorCompWrap<Base>::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	if (m_slaveExtendersCompPtr.IsValid()){
		int count = m_slaveExtendersCompPtr.GetCount();
		for (int i = 0; i < count; i++){
			if (m_slaveExtendersCompPtr[i] != NULL){
				m_slaveExtendersCompPtr[i]->AddItemsToScene(providerPtr, flags);
			}
		}
	}
}


template<class Base>
void TMultiViewExtenderDelegatorCompWrap<Base>::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	if (m_slaveExtendersCompPtr.IsValid()){
		int count = m_slaveExtendersCompPtr.GetCount();
		for (int i = 0; i < count; i++){
			if (m_slaveExtendersCompPtr[i] != NULL){
				m_slaveExtendersCompPtr[i]->RemoveItemsFromScene(providerPtr);
			}
		}
	}
}


typedef TMultiViewExtenderDelegatorCompWrap<icomp::CComponentBase> CMultiViewExtenderDelegatorComp;


} // namespace iqt2d


#endif // !iqt2d_TMultiViewExtenderDelegatorCompWrap_included

