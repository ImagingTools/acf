#include "iipr/CCaliperBasedPositionSupplierComp.h"


#include "iipr/CHeaviestFeatureConsumer.h"


namespace iipr
{


// reimplemented (iipr::IValueSupplier)

imath::CVarVector CCaliperBasedPositionSupplierComp::GetValue(I_DWORD objectId) const
{
	const WorkInfo* infoPtr = GetWorkInfo(objectId, true);
	if ((infoPtr != NULL) && (infoPtr->status == WS_OK)){
		return infoPtr->product;
	}

	return imath::CVarVector();
}


// reimplemented (iproc::ISupplier)

void CCaliperBasedPositionSupplierComp::BeginNextObject(I_DWORD objectId)
{
	if (!IsIdKnown(objectId)){
		BaseClass::BeginNextObject(objectId);

		if (m_bitmapSupplierCompPtr.IsValid()){
			m_bitmapSupplierCompPtr->BeginNextObject(objectId);
		}
	}
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CCaliperBasedPositionSupplierComp::ProduceObject(I_DWORD objectId, imath::CVarVector& result) const
{
	if (		m_bitmapSupplierCompPtr.IsValid() &&
				m_featuresMapperCompPtr.IsValid() &&
				m_caliperProcessorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapSupplierCompPtr->GetBitmap(objectId);
		if (bitmapPtr != NULL){
			iprm::IParamsSet* paramsSetPtr = GetParamsSet();

			CHeaviestFeatureConsumer consumer;
			int caliperState = m_caliperProcessorCompPtr->DoProcessing(
							paramsSetPtr,
							bitmapPtr,
							&consumer);

			if (caliperState != iproc::IProcessor::TS_OK){
				return WS_ERROR;
			}

			const IFeature* featurePtr = consumer.GetFeature();
			if (featurePtr == NULL){
				return WS_ERROR;
			}

			result = m_featuresMapperCompPtr->GetImagePosition(
						*featurePtr,
						paramsSetPtr);
			if (result.GetElementsCount() >= 2){
				return WS_OK;
			}
		}
	}

	return WS_CRITICAL;
}


} // namespace iipr


