#ifndef iview_CScreenTransformationProviderComp_included
#define iview_CScreenTransformationProviderComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <iview/IScreenTransformationProvider.h>


namespace iview
{


class CScreenTransformationProviderComp : public icomp::CComponentBase, public IScreenTransformationProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CScreenTransformationProviderComp);
		I_REGISTER_INTERFACE(IScreenTransformationProvider);
	I_END_COMPONENT;

	CScreenTransformationProviderComp();

	virtual istd::TSmartPtr<iview::CScreenTransform> GetTransformation() const override;

private:
	istd::TSmartPtr<iview::CScreenTransform> m_transformPtr;
};




} // namespace iview


#endif // !iview_CColorSchemaComp_included


