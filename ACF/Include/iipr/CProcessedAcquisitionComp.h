#ifndef iipr_CProcessedAcquisitionComp_included
#define iipr_CProcessedAcquisitionComp_included


#include "iproc/TSyncProcessorCompBase.h"

#include "icam/IBitmapAcquisition.h"


namespace iipr
{


class CProcessedAcquisitionComp: public iproc::TSyncProcessorCompBase<icam::IBitmapAcquisition>
{
public:
	typedef iproc::TSyncProcessorCompBase<icam::IBitmapAcquisition> BaseClass;

	I_BEGIN_COMPONENT(CProcessedAcquisitionComp);
		I_ASSIGN(m_slaveAcquisitionCompPtr, "SlaveCamera", "Camera for real image acquisition", true, "SlaveCamera");
		I_ASSIGN(m_processorCompPtr, "ImageProcessor", "Image processor", false, "ImageProcessor");
	I_END_COMPONENT

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented iproc::TSyncProcessorWrap<icam::IBitmapAcquisition>
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr);

private:
	I_REF(icam::IBitmapAcquisition, m_slaveAcquisitionCompPtr);
	I_REF(iproc::IProcessor, m_processorCompPtr);
};


} // namespace iipr


#endif // !iipr_CProcessedAcquisitionComp_included


