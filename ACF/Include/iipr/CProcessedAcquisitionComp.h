#ifndef iipr_CProcessedAcquisitionComp_included
#define iipr_CProcessedAcquisitionComp_included


#include "icomp/CComponentBase.h"

#include "iproc/TSyncProcessorWrap.h"

#include "icam/IBitmapAcquisition.h"
#include "iipr/IBitmapProcessor.h"


namespace iipr
{


class CProcessedAcquisitionComp:
			public icomp::CComponentBase,
			virtual public iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CProcessedAcquisitionComp);
		I_REGISTER_INTERFACE(icam::IBitmapAcquisition);
		I_ASSIGN(m_slaveAcquisitionCompPtr, "SlaveCamera", "Camera for real image acquisition", true, "SlaveCamera");
		I_ASSIGN(m_processorCompPtr, "ImageProcessor", "Image processor", false, "ImageProcessor");
	I_END_COMPONENT

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

protected:
	// reimplemented iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const isys::ITimer* inputPtr,
				iimg::IBitmap* outputPtr);

private:
	I_REF(icam::IBitmapAcquisition, m_slaveAcquisitionCompPtr);
	I_REF(iipr::IBitmapProcessor, m_processorCompPtr);
};


} // namespace iipr


#endif // !iipr_CProcessedAcquisitionComp_included


