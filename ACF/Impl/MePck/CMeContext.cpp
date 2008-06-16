#include "CMeContext.h"


#include "istd/TChangeNotifier.h"


namespace imebase
{


CMeContext::CMeContext(const CMeAddr& address, int id, bool isOutput, isig::ISamplesContainer* containerPtr)
:	m_address(address),
	m_isOutput(isOutput),
	m_id(id),
	m_samplesContainer(*containerPtr)
{
	I_ASSERT(containerPtr != NULL);

	m_hwBuffer.resize(m_samplesContainer.GetSamplesCount());

	m_bufferCount = 0;
}


CMeContext::~CMeContext()
{
	Unregister();
}


bool CMeContext::Register(double interval)
{
	m_interval = interval;

	meIOResetSubdevice(m_address.device, m_address.subdevice, 0);

	meIOStreamCB_t func;

	if (m_isOutput){
		if (!ConfigOutputStream()){
			return false;
		}

		func = cbAOFunc;
		int aCount = int(m_hwBuffer.size());
		meIOStreamWrite(m_address.device, m_address.subdevice, ME_WRITE_MODE_PRELOAD, &m_hwBuffer[0], &aCount, 0);
		m_bufferCount += aCount;
	}
	else{
		if (!ConfigInputStream()){
			return false;
		}

		func = cbAIFunc;
	}

	bool retVal = true;
	if (!IsDone()){
		int errCode = meIOStreamSetCallbacks(m_address.device, m_address.subdevice, NULL, NULL, func, this, func, this, 0);

		retVal = (errCode == 0);
	}

	retVal = retVal && StartStream();

	return retVal;
}


void CMeContext::Unregister()
{
	meIOStreamStop_t stopList;
	stopList.iDevice = m_address.device;
	stopList.iSubdevice = m_address.subdevice;
	stopList.iStopMode = ME_STOP_MODE_IMMEDIATE;
	stopList.iFlags = 0;

	meIOStreamStop(&stopList, 1, 0);
	meIOStreamSetCallbacks(m_address.device, m_address.subdevice, NULL, NULL, NULL, NULL, NULL, NULL, 0);
}


int CMeContext::GetId() const
{
	return m_id;
}


int CMeContext::GetCount() const
{
	return m_bufferCount;
}


bool CMeContext::IsDone()
{
	return int(m_hwBuffer.size()) == m_bufferCount;
}


double CMeContext::GetInterval() const
{
	return m_interval;
}


bool CMeContext::Wait(double timeout)
{
	bool retVal = true;

	m_activeTaskMutex.lock();
	if (!IsDone()){
		retVal = m_dataReadyCondition.wait(&m_activeTaskMutex, I_DWORD(timeout * 1000));
	}

	m_activeTaskMutex.unlock();

	return retVal;
}


void CMeContext::CopyToContainer()
{
	int unit;
	double minVoltage, maxVoltage;
	int maxData;

	meQueryRangeInfo(m_address.device, m_address.subdevice, 0, &unit, &minVoltage, &maxVoltage, &maxData);

	int samplesCount = m_samplesContainer.GetSamplesCount();
	I_ASSERT(samplesCount == int(m_hwBuffer.size()));

	istd::CChangeNotifier notifier(&m_samplesContainer);

	for (int index=0; index < samplesCount; index++){
		double value;
		meUtilityDigitalToPhysical(minVoltage, maxVoltage, maxData, m_hwBuffer[index], ME_EXTENSION_TYPE_NONE, 0, &value);

		m_samplesContainer.SetSample(index, value);
	}

}


void CMeContext::CopyFromContainer()
{
	int unit;
	double minVoltage;
	double maxVoltage;
	int maxData;

	meQueryRangeInfo(m_address.device, m_address.subdevice, 0, &unit, &minVoltage, &maxVoltage, &maxData);

	int samplesCount = m_samplesContainer.GetSamplesCount();
	I_ASSERT(samplesCount == int(m_hwBuffer.size()));

	for (int index = 0; index < samplesCount; index++){
		double sample = m_samplesContainer.GetSample(index);

		meUtilityPhysicalToDigital(minVoltage, maxVoltage, maxData, sample, &m_hwBuffer[index]);
	}
}


// protected methods

bool CMeContext::ConfigInputStream()
{
	int interval_high;
	int interval_low;

	double sampleInterval = m_interval / m_samplesContainer.GetSamplesCount();

	if (meIOStreamTimeToTicks(
				m_address.device,
				m_address.subdevice,
				ME_TIMER_CONV_START,
				&sampleInterval,
				&interval_low,
				&interval_high,
				ME_VALUE_NOT_USED) != 0){
		return false;
	}

	meIOStreamConfig_t configList;
	configList.iChannel = m_address.channel;
	configList.iStreamConfig = 0;
	configList.iRef = ME_REF_AI_GROUND;
	configList.iFlags = ME_IO_STREAM_CONFIG_NO_FLAGS;

	meIOStreamTrigger_t trigger;
	trigger.iAcqStartTrigType = ME_TRIG_TYPE_SW;
	trigger.iAcqStartTrigEdge = ME_VALUE_NOT_USED;
	trigger.iAcqStartTrigChan = ME_TRIG_CHAN_DEFAULT;
	trigger.iAcqStartTicksLow = 66;
	trigger.iAcqStartTicksHigh = 0;
	trigger.iScanStartTrigType = ME_TRIG_TYPE_FOLLOW;
	trigger.iScanStartTicksLow = 0;
	trigger.iScanStartTicksHigh = 0;
	trigger.iConvStartTrigType = ME_TRIG_TYPE_TIMER;
	trigger.iConvStartTicksLow = interval_low;
	trigger.iConvStartTicksHigh = interval_high;
	trigger.iScanStopTrigType = ME_TRIG_TYPE_COUNT;
	trigger.iScanStopCount = 1024;
	trigger.iAcqStopTrigType = ME_TRIG_TYPE_FOLLOW;
	trigger.iAcqStopCount = 0;
	trigger.iFlags = ME_VALUE_NOT_USED;

	int errCode = meIOStreamConfig(m_address.device, m_address.subdevice, &configList, 1, &trigger, 1024, 0);

	return (errCode == 0);
}


bool CMeContext::ConfigOutputStream()
{
	int interval_high;
	int interval_low;

	double sampleInterval = m_interval / m_samplesContainer.GetSamplesCount();

	if (meIOStreamTimeToTicks(
				m_address.device,
				m_address.subdevice,
				ME_TIMER_CONV_START,
				&sampleInterval,
				&interval_low,
				&interval_high,
				ME_VALUE_NOT_USED) != 0){
		return false;
	}

	meIOStreamConfig_t configList;
	configList.iChannel = m_address.channel;
	configList.iStreamConfig = 0;
	configList.iRef = ME_REF_AO_GROUND;
	configList.iFlags = ME_IO_STREAM_CONFIG_NO_FLAGS;

	meIOStreamTrigger_t trigger;
	trigger.iAcqStartTrigType = ME_TRIG_TYPE_SW;
	trigger.iAcqStartTrigEdge = ME_VALUE_NOT_USED;
	trigger.iAcqStartTrigChan = ME_TRIG_CHAN_DEFAULT;
	trigger.iAcqStartTicksLow = 0;
	trigger.iAcqStartTicksHigh = 0;
	trigger.iScanStartTrigType = ME_TRIG_TYPE_FOLLOW;
	trigger.iScanStartTicksLow = 0;
	trigger.iScanStartTicksHigh = 0;
	trigger.iConvStartTrigType = ME_TRIG_TYPE_TIMER;
	trigger.iConvStartTicksLow = interval_low;
	trigger.iConvStartTicksHigh = interval_high;
	trigger.iScanStopTrigType = ME_TRIG_TYPE_NONE;
	trigger.iScanStopCount = 0;
	trigger.iAcqStopTrigType = ME_TRIG_TYPE_NONE;
	trigger.iAcqStopCount = 0;
	trigger.iFlags = ME_IO_STREAM_CONFIG_WRAPAROUND;

	return (meIOStreamConfig(m_address.device, m_address.subdevice, &configList, 1, &trigger, 0, 0) == 0);
}


bool CMeContext::StartStream()
{
	meIOStreamStart_t startList;
	startList.iDevice = m_address.device;
	startList.iSubdevice = m_address.subdevice;
	startList.iStartMode = ME_START_MODE_NONBLOCKING;
	startList.iTimeOut = 0;
	startList.iFlags = 0;

	return (meIOStreamStart(&startList, 1, 0) == 0);
}


// protected static methods

int CMeContext::cbAIFunc(int device, int subdevice, int count, void* context, int /*error*/)
{
	CMeContext* itself = (CMeContext*)context;
	int aCount = int(itself->m_hwBuffer.size()) - itself->m_bufferCount;
	if (aCount > count){
		aCount = count;
	}

	if (aCount){
		meIOStreamRead(device, subdevice, ME_READ_MODE_NONBLOCKING, &itself->m_hwBuffer[itself->m_bufferCount], &aCount, 0);
		itself->m_bufferCount += aCount;
	}

	if (itself->IsDone()){
		itself->m_dataReadyCondition.wakeAll();

		return 1;
	}

	return 0;
}


int CMeContext::cbAOFunc(int device, int subdevice, int /*count*/, void* context, int /*error*/)
{
	CMeContext* itself = (CMeContext *)context;
	if (itself->IsDone()){
		itself->m_dataReadyCondition.wakeAll();

		return 1;
	}

	int aCount = int(itself->m_hwBuffer.size()) - itself->m_bufferCount;

	if (aCount){
		meIOStreamWrite(device, subdevice, ME_WRITE_MODE_NONBLOCKING, &itself->m_hwBuffer[itself->m_bufferCount], &aCount, 0);
		itself->m_bufferCount += aCount;
	}

	if (itself->IsDone()){
		itself->m_dataReadyCondition.wakeAll();

		return 1;
	}

	return 0;
}


} // namespace imebase


