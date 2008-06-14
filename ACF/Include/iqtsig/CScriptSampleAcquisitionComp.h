#ifndef iqtisig_CScriptSampleAcquisitionComp_included
#define iqtisig_CScriptSampleAcquisitionComp_included


#include <QtScript>

#include "icomp/CComponentBase.h"

#include "iproc/TSyncProcessorWrap.h"

#include "isig/ISamplesProcessor.h"

#include "iqtsig/iqtsig.h"


namespace iqtsig
{


class CScriptSampleAcquisitionComp: public icomp::CComponentBase, virtual public iproc::TSyncProcessorWrap<isig::ISamplesProcessor>
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CScriptSampleAcquisitionComp)
		I_REGISTER_INTERFACE(isig::ISamplesProcessor)
		I_ASSIGN(m_minValueAttrPtr, "MinValue", "Minimal value of sample", true, 0.0);
		I_ASSIGN(m_maxValueAttrPtr, "MaxValue", "Maximal value of sample", true, 100.0);
		I_ASSIGN(m_samplesCountAttrPtr, "SamplesCount", "Number of samples processed in single step", false, 100);
		I_ASSIGN(m_defaultScriptAttrPtr, "DefaultScript", "Default script used to calculate sample output in 'Calc' function", false, "minValue");
	I_END_COMPONENT

	CScriptSampleAcquisitionComp();

	// reimplemented (iproc::TSyncProcessorWrap<isig::ISamplesProcessor>)
	virtual int DoSyncProcess(
				const iprm::IParamsSet* paramsPtr,
				const isig::ISamplesContainer* inputPtr,
				isig::ISamplesContainer* outputPtr);

	// reimplemented (isig::ISamplesProcessor)
	virtual istd::CRange GetValueRange(bool forInput = true, bool forOutput = true, const iprm::IParamsSet* paramsSetPtr = 0) const;
	virtual int GetMaximalSamplesCount(bool forInput = true, bool forOutput = true, const iprm::IParamsSet* paramsSetPtr = 0) const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	QScriptEngine m_scriptEngine;
	int frameNumber;

	I_ATTR(double, m_minValueAttrPtr);
	I_ATTR(double, m_maxValueAttrPtr);
	I_ATTR(int, m_samplesCountAttrPtr);
	I_ATTR(istd::CString, m_defaultScriptAttrPtr);
};


} // namespace iqtsig


#endif // !iqtisig_CScriptSampleAcquisitionComp_included


