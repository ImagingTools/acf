#ifndef iipr_CIterativeProcessorComp_included
#define iipr_CIterativeProcessorComp_included


#include "iproc/TSyncProcessorWrap.h"

#include "icomp/CComponentBase.h"

#include "iimg/CGeneralBitmap.h"

#include "iipr/CIterativeProcessorParams.h"


namespace iipr
{
	

/**
	Processor wrapper for multiple executing of a slave processor.
	Input at k-iteration for the slave operator is processor output at (k-1)-iteration.
*/
class CIterativeProcessorComp:
			public icomp::CComponentBase,
			public iproc::CSyncProcessorBase
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef iproc::CSyncProcessorBase BaseClass2;

	I_BEGIN_COMPONENT(CIterativeProcessorComp)
		I_REGISTER_INTERFACE(iproc::IProcessor)
		I_ASSIGN(m_paramsIdAttrPtr, "ParamsId", "ID of processor parameter", true, "ParamsId");
		I_ASSIGN(m_slaveProcessorCompPtr, "SlaveProcessor", "Slave image processor", true, "SlaveProcessor");
	I_END_COMPONENT

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr);

protected:
	int ProcessSlave(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap* inputPtr,
				iimg::IBitmap* outputPtr);

private:
	I_ATTR(istd::CString, m_paramsIdAttrPtr);
	I_REF(iproc::IProcessor, m_slaveProcessorCompPtr);
};


} // namespace iipr


#endif // !iipr_CIterativeProcessorComp_included


