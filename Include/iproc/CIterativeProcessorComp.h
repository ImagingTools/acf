#ifndef iproc_CIterativeProcessorComp_included
#define iproc_CIterativeProcessorComp_included


#include "ibase/TLoggerCompWrap.h"

#include "iproc/CIterativeProcessorParams.h"
#include "iproc/TSyncProcessorWrap.h"


namespace iproc
{
	

/**
	Processor wrapper for multiple executing of a slave processor.
	Input at k-iteration for the slave operator is processor output at (k-1)-iteration.
*/
class CIterativeProcessorComp:
			public ibase::CLoggerComponentBase,
			public iproc::CSyncProcessorBase
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef iproc::CSyncProcessorBase BaseClass2;

	enum MessageId
	{
		MI_BAD_BUFFER = 0x6b30
	};

	I_BEGIN_COMPONENT(CIterativeProcessorComp);
		I_REGISTER_INTERFACE(iproc::IProcessor);
		I_ASSIGN(m_paramsIdAttrPtr, "ParamsId", "ID of processor parameter", true, "ParamsId");
		I_ASSIGN(m_bufferObjectCompPtr, "BufferObject", "Object used as buffer between single processing steps", true, "BufferObject");
		I_ASSIGN(m_slaveProcessorCompPtr, "SlaveProcessor", "Slave image processor", true, "SlaveProcessor");
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				IProgressManager* progressManagerPtr = NULL);

protected:
	int ProcessSlave(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr);

private:
	I_ATTR(istd::CString, m_paramsIdAttrPtr);
	I_REF(istd::IChangeable, m_bufferObjectCompPtr);
	I_REF(iproc::IProcessor, m_slaveProcessorCompPtr);
};


} // namespace iproc


#endif // !iproc_CIterativeProcessorComp_included


