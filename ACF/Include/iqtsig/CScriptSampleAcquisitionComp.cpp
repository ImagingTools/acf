#include "iqtsig/CScriptSampleAcquisitionComp.h"


#include "istd/TChangeNotifier.h"


namespace iqtsig
{


CScriptSampleAcquisitionComp::CScriptSampleAcquisitionComp()
:	frameNumber(0)
{
}


// reimplemented (iproc::TSyncProcessorWrap<isig::ISamplesProcessor>)

int CScriptSampleAcquisitionComp::DoSyncProcess(
			const iprm::IParamsSet* /*paramsPtr*/,
			const isig::ISamplesContainer* /*inputPtr*/,
			isig::ISamplesContainer* outputPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	QScriptValue frameValue(&m_scriptEngine, ++frameNumber);
	m_scriptEngine.globalObject().setProperty("frame", frameValue);

	QString functionScript = iqt::GetQString(*m_defaultScriptAttrPtr);
	m_scriptEngine.evaluate(functionScript);

	istd::CChangeNotifier notifier(outputPtr);

	int samplesCount;
	if (m_samplesCountAttrPtr.IsValid()){
		samplesCount = *m_samplesCountAttrPtr;
		if (!outputPtr->SetSamplesCount(samplesCount)){
			return TS_INVALID;
		}
	}
	else{
		samplesCount = outputPtr->GetSamplesCount();
	}

	QScriptValue calcCtor = m_scriptEngine.evaluate("Calc");

	for (int i = 0; i < samplesCount; ++i){
		QScriptValueList arguments;
		QScriptValue xValue(&m_scriptEngine, i);
		arguments << xValue;

		double sample = calcCtor.call(m_scriptEngine.nullValue(), arguments).toNumber();

		if (m_scriptEngine.hasUncaughtException()) {
			return TS_INVALID;
		}

		outputPtr->SetSample(i, sample);
	}

	return TS_OK;
}


// reimplemented (isig::ISamplesProcessor)

istd::CRange CScriptSampleAcquisitionComp::GetValueRange(bool /*forInput*/, bool /*forOutput*/, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	return istd::CRange(*m_minValueAttrPtr, *m_maxValueAttrPtr);
}


int CScriptSampleAcquisitionComp::GetMaximalSamplesCount(bool /*forInput*/, bool /*forOutput*/, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	return -1;
}


// reimplemented (icomp::IComponent)

void CScriptSampleAcquisitionComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	QScriptValue minValue(&m_scriptEngine, *m_minValueAttrPtr);
	m_scriptEngine.globalObject().setProperty("minValue", minValue);
	QScriptValue maxValue(&m_scriptEngine, *m_maxValueAttrPtr);
	m_scriptEngine.globalObject().setProperty("maxValue", maxValue);
}


} // namespace iqtsig


