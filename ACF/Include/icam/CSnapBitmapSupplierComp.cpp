#include "icam/CSnapBitmapSupplierComp.h"


namespace icam
{


// reimplemented (icam::IBitmapSupplier)

const iimg::IBitmap* CSnapBitmapSupplierComp::GetBitmap(I_DWORD objectId) const
{
	const WorkInfo* infoPtr = GetWorkInfo(objectId, true);
	if (infoPtr != NULL){
		return infoPtr->product.GetPtr();
	}

	return NULL;
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CSnapBitmapSupplierComp::ProduceObject(I_DWORD objectId, istd::TDelPtr<iimg::IBitmap>& result) const
{
	if (m_bitmapCompFact.IsValid() && !result.IsValid()){
		result.SetPtr(m_bitmapCompFact.CreateInstance());
	}

	if (result.IsValid() && m_bitmapAcquisitionCompPtr.IsValid()){
		int status = m_bitmapAcquisitionCompPtr->DoProcessing(GetModelParametersSet(), NULL, result.GetPtr());
		switch (status){
		case IBitmapAcquisition::TS_OK:
			return WS_OK;

		case IBitmapAcquisition::TS_CANCELED:
			return WS_CANCELED;

		default:
			return WS_ERROR;
		}
	}

	return WS_CRITICAL;
}


} // namespace icam

