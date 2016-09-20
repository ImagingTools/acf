#ifndef iimg_CMultiPageBitmapComp_included
#define iimg_CMultiPageBitmapComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <iimg/CMultiPageBitmapBase.h>


namespace iimg
{


/**
	Realization of the multi-page bitmap used the component factory for the instatiation of the page bitmap.
*/
class CMultiPageBitmapComp:
			public icomp::CComponentBase,
			public CMultiPageBitmapBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CMultiPageBitmapComp);
		I_REGISTER_INTERFACE(IMultiBitmapProvider);
		I_REGISTER_INTERFACE(IMultiPageBitmapController);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(iprm::IOptionsList);
		I_REGISTER_INTERFACE(idoc::IMultiPageDocument);
		I_ASSIGN(m_bitmapFactoryCompPtr, "BitmapFactory", "Factory used for creation of bitmap for a single document page", true, "BitmapFactory");
	I_END_COMPONENT;

protected:
	// reimplemented (CMultiPageBitmapBase)
	virtual IBitmap* CreateBitmap() const;

private:
	I_FACT(IBitmap, m_bitmapFactoryCompPtr);
};


} // namespace iimg


#endif // !iimg_CMultiPageBitmapComp_included


