#include <iimg/CComposedBitmapProviderComp.h>


namespace iimg
{


CComposedBitmapProviderComp::CComposedBitmapProviderComp()
:	imod::CMultiModelBridgeBase(this)
{
}


// reimplemented (iimg::IMultiBitmapProvider)

const iprm::IOptionsList* CComposedBitmapProviderComp::GetBitmapListInfo() const
{
	return this;
}


int CComposedBitmapProviderComp::GetBitmapsCount() const
{
	if (m_bitmapProvidersCompPtr.IsValid()){
		return m_bitmapProvidersCompPtr.GetCount();
	}

	return 0;
}


const iimg::IBitmap* CComposedBitmapProviderComp::GetBitmap(int bitmapIndex) const
{
	Q_ASSERT(bitmapIndex >= 0 && bitmapIndex < GetBitmapsCount());

	if (bitmapIndex >= 0 && bitmapIndex < GetBitmapsCount()){
		return m_bitmapProvidersCompPtr[bitmapIndex]->GetBitmap();
	}

	return NULL;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CComposedBitmapProviderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int bitmapsCount = m_bitmapProviderModelsCompPtr.GetCount();
	int labelsCount = m_bitmapLabelsAttrPtr.GetCount();
	int descriptionsCount = m_bitmapDescriptionsAttrPtr.GetCount();

	for (int bitmapIndex = 0; bitmapIndex < bitmapsCount; bitmapIndex++){
		m_bitmapProviderModelsCompPtr[bitmapIndex]->AttachObserver(this);

		QString optionName = QString("Bitmap %1").arg(bitmapIndex + 1);

		InsertOption(optionName, optionName.toUtf8());
	}

	int optionsCount = GetOptionsCount();
	for (int optionIndex = 0; optionIndex < optionsCount; ++optionIndex){
		if (optionIndex < labelsCount){
			SetOptionName(optionIndex, m_bitmapLabelsAttrPtr[optionIndex]);
		}

		if (optionIndex < descriptionsCount){
			SetOptionName(optionIndex, m_bitmapDescriptionsAttrPtr[optionIndex]);
		}
	}
}


void CComposedBitmapProviderComp::OnComponentDestroyed()
{
	EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace iimg


