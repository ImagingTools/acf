#include "iipr/CCaliperBasedPositionSupplierComp.h"


#include "iipr/CHeaviestFeatureConsumer.h"


namespace iipr
{


// reimplemented (iipr::IVector2dSupplier)

const i2d::CVector2d* CCaliperBasedPositionSupplierComp::GetVector2d(I_DWORD objectId) const
{
	const WorkInfo* infoPtr = GetWorkInfo(objectId, true);
	if ((infoPtr != NULL) && (infoPtr->status == WS_OK)){
		return &infoPtr->product;
	}

	return NULL;
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

int CCaliperBasedPositionSupplierComp::ProduceObject(I_DWORD objectId, i2d::CVector2d& result) const
{
	if (		m_bitmapSupplierCompPtr.IsValid() &&
				m_lineProjectionProcessorCompPtr.IsValid() &&
				m_caliperProcessorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapSupplierCompPtr->GetBitmap(objectId);
		if (bitmapPtr != NULL){
			iprm::IParamsSet* paramsSetPtr = GetParamsSet();

			iipr::CProjectionData projection;
			int projectionState = m_lineProjectionProcessorCompPtr->DoProcessing(
						paramsSetPtr,
						bitmapPtr,
						&projection);

			if (projectionState != iipr::ILineProjectionProcessor::TS_OK){
				return WS_ERROR;
			}

			CHeaviestFeatureConsumer consumer;
			int caliperState = m_caliperProcessorCompPtr->DoProcessing(
							paramsSetPtr,
							&projection,
							&consumer);

			if (caliperState != iipr::ILineProjectionProcessor::TS_OK){
				return WS_ERROR;
			}

			const IFeature* featurePtr = consumer.GetFeature();
			if (featurePtr == NULL){
				return WS_ERROR;
			}

			imath::CVarVector position = m_lineProjectionProcessorCompPtr->GetBitmapPosition(
						featurePtr->GetPosition(),
						paramsSetPtr);
			if (position.GetElementsCount() < 2){
				return WS_CRITICAL;
			}

			result.SetX(position[0]);
			result.SetY(position[1]);

			return WS_OK;
		}
	}

	return WS_CRITICAL;
}


} // namespace iipr


