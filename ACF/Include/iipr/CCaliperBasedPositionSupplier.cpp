#include "iipr/CCaliperBasedPositionSupplier.h"


#include "iipr/CHeaviestFeatureConsumer.h"


namespace iipr
{


// reimplemented (iipr::IVector2dSupplier)

const i2d::CVector2d* CCaliperBasedPositionSupplier::GetVector2d(I_DWORD objectId) const
{
	const WorkInfo* infoPtr = GetWorkInfo(objectId, true);
	if (infoPtr != NULL){
		return &infoPtr->product;
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CCaliperBasedPositionSupplier::Serialize(iser::IArchive& archive)
{
	if (m_paramsSetCompPtr.IsValid()){
		return m_paramsSetCompPtr->Serialize(archive);
	}

	return true;
}


I_DWORD CCaliperBasedPositionSupplier::GetMinimalVersion(int versionId) const
{
	if (m_paramsSetCompPtr.IsValid()){
		return m_paramsSetCompPtr->GetMinimalVersion(versionId);
	}

	return BaseClass::GetMinimalVersion(versionId);
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CCaliperBasedPositionSupplier::ProduceObject(I_DWORD objectId, i2d::CVector2d& result) const
{
	if (		m_bitmapSupplierCompPtr.IsValid() &&
				m_lineProjectionProcessorCompPtr.IsValid() &&
				m_caliperProcessorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapSupplierCompPtr->GetBitmap(objectId);
		if (bitmapPtr != NULL){
			iipr::CProjectionData projection;
			int projectionState = m_lineProjectionProcessorCompPtr->DoProcessing(
						m_paramsSetCompPtr.GetPtr(),
						bitmapPtr,
						&projection);

			if (projectionState != iipr::ILineProjectionProcessor::TS_OK){
				return WS_ERROR;
			}

			CHeaviestFeatureConsumer consumer;
			int caliperState = m_caliperProcessorCompPtr->DoProcessing(
							m_paramsSetCompPtr.GetPtr(),
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
						m_paramsSetCompPtr.GetPtr());
			if (position.GetElementsCount() < 2){
				return WS_CRITICAL;
			}

			result.SetX(position[0]);
			result.SetY(position[1]);
		}
	}

	return WS_CRITICAL;
}


} // namespace iipr


