#ifndef iipr_TImageProcessorCompBase_included
#define iipr_TImageProcessorCompBase_included


#include "iproc/TSyncProcessorWrap.h"

#include "icomp/CComponentBase.h"

#include "iimg/CGeneralBitmap.h"

#include "iipr/IBitmapProcessor.h"


namespace iipr
{


/**
	Basic implementation for a simple bitmap processor.
*/
template <class ParameterType>
class TImageProcessorCompBase:	public icomp::CComponentBase,
								public iproc::TSyncProcessorWrap<iipr::IBitmapProcessor>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef iproc::TSyncProcessorWrap<iipr::IBitmapProcessor> BaseClass2;

	I_BEGIN_BASE_COMPONENT(TImageProcessorCompBase)
		I_REGISTER_INTERFACE(iipr::IBitmapProcessor)
		I_ASSIGN(m_paramsIdAttrPtr, "ParamsId", "ID of processor parameter", true, "ParamsId");
		I_ASSIGN(m_slaveProcessorPtr, "SlaveProcessor", "Slave image processor", false, "SlaveProcessor");
	I_END_COMPONENT

protected:
	// reimplemented (iproc::TSyncProcessorWrap<iipr::IBitmapProcessor>)
	virtual int DoSyncProcess(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap* inputPtr,
				iimg::IBitmap* outputPtr);

	// abstract methods
	virtual bool ProcessImage(
				const ParameterType& parameter, 
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage) = 0;

private:
	I_ATTR(istd::CString, m_paramsIdAttrPtr);
	I_REF(iipr::IBitmapProcessor, m_slaveProcessorPtr);
};


// protected methods

template <class ParameterType>
int TImageProcessorCompBase<ParameterType>::DoSyncProcess(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap* inputPtr,
			iimg::IBitmap* outputPtr)
{
	if (inputPtr == NULL || outputPtr == NULL){
		return BaseClass2::TS_INVALID;
	}

	if (!m_paramsIdAttrPtr.IsValid()){
		return BaseClass2::TS_INVALID;
	}

	iimg::CGeneralBitmap outputBitmap;

	if (m_slaveProcessorPtr.IsValid()){
		int taskId = m_slaveProcessorPtr->BeginTask(paramsPtr, inputPtr, &outputBitmap);
		if (taskId >= 0){
			int retVal = m_slaveProcessorPtr->WaitTaskFinished(taskId);
			if (retVal == TS_OK){
				inputPtr = &outputBitmap;
			}
		}
	}

	const ParameterType* processorParamsPtr = 
		dynamic_cast<const ParameterType*>(paramsPtr->GetParameter(m_paramsIdAttrPtr->GetValue().ToString()));
	if (processorParamsPtr == NULL){
		return BaseClass2::TS_INVALID;
	}

	// create output image:
	outputPtr->CopyImageFrom(*inputPtr);

	// do image processing:
	if (!ProcessImage(*processorParamsPtr, *inputPtr, *outputPtr)){
		return BaseClass2::TS_INVALID;
	}
	
	return BaseClass2::TS_OK;
}


} // namespace iipr


#endif // !iipr_TImageProcessorCompBase_included

