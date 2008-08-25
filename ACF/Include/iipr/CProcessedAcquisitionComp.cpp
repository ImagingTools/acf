#include "iipr/CProcessedAcquisitionComp.h"

#include "istd/TChangeNotifier.h"

#include "iimg/CGeneralBitmap.h"


namespace iipr
{


// reimplemented (icam::IBitmapAcquisition)

istd::CIndex2d CProcessedAcquisitionComp::GetBitmapSize(const iprm::IParamsSet* paramsPtr) const
{
	if (m_slaveAcquisitionCompPtr.IsValid()){
		return m_slaveAcquisitionCompPtr->GetBitmapSize(paramsPtr);
	}

	return istd::CIndex2d();
}


// reimplemented iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>

int CProcessedAcquisitionComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	istd::TChangeNotifier<iimg::IBitmap> outputBitmapPtr(dynamic_cast<iimg::IBitmap*>(outputPtr));

	if (!outputBitmapPtr.IsValid() || !m_slaveAcquisitionCompPtr.IsValid()){
		return TS_INVALID;
	}

	int retVal = TS_INVALID;

	retVal = m_slaveAcquisitionCompPtr->DoProcessing(paramsPtr, inputPtr, outputBitmapPtr.GetPtr());
	if (retVal == TS_OK){
		if (m_processorCompPtr.IsValid()){
			iimg::CGeneralBitmap bufferBitmap;
			retVal = m_processorCompPtr->DoProcessing(paramsPtr, outputBitmapPtr.GetPtr(), &bufferBitmap);
			if (retVal == TS_OK){
				if (!outputBitmapPtr->CopyImageFrom(bufferBitmap)){
					retVal = TS_INVALID;
				}
			}
		}
	}

	return retVal;
}


} // namespace iipr

