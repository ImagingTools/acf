#ifndef iimg_CComposedBitmapProviderComp_included
#define iimg_CComposedBitmapProviderComp_included


// ACF includes
#include <iimg/IBitmapProvider.h>
#include <iimg/IMultiBitmapProvider.h>
#include <icomp/CComponentBase.h>
#include <imod/CModelUpdateBridge.h>
#include <iprm/COptionsManager.h>


namespace iimg
{


/**
	Component for composition/grouping of more single bitmap providers.
*/
class CComposedBitmapProviderComp: 
			public icomp::CComponentBase,
			virtual public iimg::IMultiBitmapProvider,
			protected iprm::COptionsManager
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CComposedBitmapProviderComp);
		I_REGISTER_INTERFACE(iimg::IMultiBitmapProvider);
		I_ASSIGN_MULTI_0(m_bitmapProvidersCompPtr, "BitmapProviders", "List of input bitmap providers", true);
		I_ASSIGN_TO(m_bitmapProviderModelsCompPtr, m_bitmapProvidersCompPtr, false);
		I_ASSIGN_MULTI_0(m_bitmapLabelsAttrPtr, "BitmapLabels", "List of bitmap labels", false);
		I_ASSIGN_MULTI_0(m_bitmapDescriptionsAttrPtr, "BitmapDescriptions", "List of bitmap descriptions", false);
	I_END_COMPONENT;

	CComposedBitmapProviderComp();

	// reimplemented (iimg::IMultiBitmapProvider)
	virtual const iprm::IOptionsList* GetBitmapListInfo() const;
	virtual int GetBitmapsCount() const;
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_MULTIREF(iimg::IBitmapProvider, m_bitmapProvidersCompPtr);
	I_MULTIREF(imod::IModel, m_bitmapProviderModelsCompPtr);
	I_MULTITEXTATTR(m_bitmapLabelsAttrPtr);
	I_MULTITEXTATTR(m_bitmapDescriptionsAttrPtr);

	imod::CModelUpdateBridge m_updateBridge;
};


} // namespace iimg


#endif // !iimg_CComposedBitmapProviderComp_included


