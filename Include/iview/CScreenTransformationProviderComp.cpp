#include <iview/CScreenTransformationProviderComp.h>


namespace iview
{

CScreenTransformationProviderComp::CScreenTransformationProviderComp()
{
	m_transformPtr.SetPtr(new iview::CScreenTransform);
	m_transformPtr->Reset();
}

istd::TSmartPtr<iview::CScreenTransform> CScreenTransformationProviderComp::GetTransformation() const
{
	return m_transformPtr;
}


} // namespace iview


