#include "CMeilhausSimpleComp.h"


#include <QTime>

#include "CChannelSelectionNode.h"


namespace imebase
{


CMeilhausSimpleComp::CMeilhausSimpleComp()
{
	meOpen(0);

	m_lastTaskId = -1;
}


// reimplemented (isig::ISamplesProcessor)

istd::CRange CMeilhausSimpleComp::GetValueRange(bool /*forInput*/, bool /*forOutput*/, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	return istd::CRange(-10, 9.996);
}


int CMeilhausSimpleComp::GetMaximalSamplesCount(bool /*forInput*/, bool /*forOutput*/, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	return 1024;
}


// reimplemented (iproc::TIAssyncProcessor)

int CMeilhausSimpleComp::GetProcessorState(const iprm::IParamsSet* /*paramsPtr*/) const
{
	return PS_READY;
}


void CMeilhausSimpleComp::ResetAllTasks()
{
	for (int index = 0; index < m_activeTaskList.size(); index++) {
		delete m_activeTaskList.at(index);
		m_activeTaskList.removeAt(index);
	}
}


bool CMeilhausSimpleComp::AreParamsAccepted(const iprm::IParamsSet* paramsPtr) const
{
	CMeAddr address;
	return		GetChannelAddress(paramsPtr, address) &&
				(GetSamplingParams(paramsPtr) != 0);
}


int CMeilhausSimpleComp::BeginTask(
			const iprm::IParamsSet* paramsPtr,
			const isig::ISamplesContainer* inputPtr,
			isig::ISamplesContainer* outputPtr)
{
	CMeAddr address;
	if (!GetChannelAddress(paramsPtr, address)){
		return -1;
	}

	const isig::ISamplingParams* samplingParamsPtr = GetSamplingParams(paramsPtr);
	if (samplingParamsPtr == NULL){
		return -1;
	}

	CMeContext* context = NULL;
	if (*m_isOutputAttrPtr){
		if (inputPtr == NULL){
			return -1;
		}

		context = new CMeContext(address, PullNextTaskId(), *m_isOutputAttrPtr, const_cast<isig::ISamplesContainer*>(inputPtr));

		context->CopyFromContainer();
	}
	else{
		if (outputPtr == NULL){
			return -1;
		}

		outputPtr->SetSamplesCount(1024);

		context = new CMeContext(address, PullNextTaskId(), *m_isOutputAttrPtr, outputPtr);
	}

	if (!context->Register(samplingParamsPtr->GetInterval())){
		delete context;
		return -1;
	}

	m_activeTaskList.append(context);

	return m_lastTaskId;

}


int CMeilhausSimpleComp::WaitTaskFinished(int taskId, double timeoutTime, bool killOnTimeout)
{
	int ret;
	if (taskId == -1){
		ret  = this->WaitAllTasksFinished(timeoutTime, killOnTimeout);
	}
	else
		ret = this->WaitSingleTaskFinished(taskId, timeoutTime, killOnTimeout);
	return ret;
}


int CMeilhausSimpleComp::WaitSingleTaskFinished(int taskId, double timeoutTime, bool killOnTimeout)
{
	for (int index = 0; index < m_activeTaskList.size(); index++) {
		CMeContext* entry = m_activeTaskList.at(index);
		if (entry->GetId() == taskId){
			if (timeoutTime < 0){
				timeoutTime = entry->GetInterval() + 1;
			}

			if (entry->Wait(timeoutTime)){
				if (!*m_isOutputAttrPtr){
					entry->CopyToContainer();
				}

				delete entry;
				m_activeTaskList.removeAt(index);

				return TS_OK;
			}
			else{
				if (killOnTimeout){
					delete entry;
					m_activeTaskList.removeAt(index);

					return TS_INVALID;
				}
				else{
					return TS_WAIT;
				}
			}

			break;
		}
	}

	I_CRITICAL();

	return TS_NONE;//Not on list!
}


int CMeilhausSimpleComp::WaitAllTasksFinished(double timeoutTime, bool killOnTimeout)
{
	if (m_activeTaskList.isEmpty()){
		return TS_OK;
	}

	int ret = TS_OK;
	double localTimeout = timeoutTime;
	QTime stopper;
	stopper.start();
	for (int index = 0; index < m_activeTaskList.size(); index++) {
		CMeContext* entry = m_activeTaskList.at(index);
		int lret = WaitSingleTaskFinished(entry->GetId(), localTimeout, killOnTimeout);
		if (TS_OK != lret){
			ret = lret;
		}
		localTimeout -= (double)stopper.elapsed() / 1000;
		if (localTimeout < 0)
			localTimeout = 0;
	}
	return ret;
}


int CMeilhausSimpleComp::GetReadyTask()
{
	for (int index = 0; index < m_activeTaskList.size(); index++) {
		CMeContext* entry = m_activeTaskList.at(index);
		if (entry->IsDone())
			return entry->GetId();
	}
	return -1;
}


int CMeilhausSimpleComp::GetTaskState(int taskId) const
{
	for (int index = 0; index < m_activeTaskList.size(); index++) {
		CMeContext* entry = m_activeTaskList.at(index);
		if (taskId == -1){
			if (!entry->IsDone())
				break;

		}
		else {
			if (entry->GetId() == taskId)
				if (entry->IsDone())
					return TS_OK;
				else
					break;
		}
	}
	return TS_WAIT;
}


void CMeilhausSimpleComp::InitProcessor(const iprm::IParamsSet* /*paramsPtr*/)
{
}


// reimplemented (imebase::IMeilhausAccess)

bool CMeilhausSimpleComp::CreateSelectionTree(CChannelSelectionNode& result) const
{
	int devicesCount;
	if (meQueryNumberDevices(&devicesCount) != 0){
		return false;
	}

	for (int deviceIndex = 0; deviceIndex < devicesCount; deviceIndex++){
		istd::TDelPtr<CChannelSelectionNode> deviceNodePtr(new CChannelSelectionNode);

		int  meType = (*m_isOutputAttrPtr)? ME_TYPE_AO: ME_TYPE_AI;

		for (		int subdeviceIndex = 0;
					meQuerySubdeviceByType(deviceIndex, subdeviceIndex, meType, ME_SUBTYPE_STREAMING, &subdeviceIndex) == 0;
					++subdeviceIndex){
			istd::TDelPtr<CChannelSelectionNode> subdeviceNodePtr(new CChannelSelectionNode);

			int channelsCount;
			meQueryNumberChannels(deviceIndex, subdeviceIndex, &channelsCount);

			for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex){
				istd::CString channelName = iqt::GetCString(QString::number(channelIndex));
				subdeviceNodePtr->InsertNode(channelName, channelIndex, NULL);
			}

			if (subdeviceNodePtr->GetOptionsCount() > 0){
				istd::CString subdeviceName = iqt::GetCString(QString::number(subdeviceIndex));
				deviceNodePtr->InsertNode(subdeviceName, subdeviceIndex, subdeviceNodePtr.PopPtr());
			}
		}

		if (deviceNodePtr->GetOptionsCount() > 0){
			istd::CString deviceName = iqt::GetCString(QString::number(deviceIndex));
			char buffer[256];
			if (meQueryNameDevice(deviceIndex, buffer, 255) == 0){
				deviceName = deviceName + ":" + buffer;
			}

			result.InsertNode(deviceName, deviceIndex, deviceNodePtr.PopPtr());
		}
	}

	return true;
}


istd::CRange CMeilhausSimpleComp::GetIntervalRange() const
{// Maximum speed is 500kHz, minimum 1Hz.
	return istd::CRange(0.000002, 1.000000);
}


bool CMeilhausSimpleComp::IsSamplingModeSupported(int mode) const
{
	if (*m_isOutputAttrPtr){
		return mode == isig::ISamplingParams::SM_PERIODIC;
	}
	else{
		return mode == isig::ISamplingParams::SM_SINGLE;
	}
}


int CMeilhausSimpleComp::PullNextTaskId()
{
	m_lastTaskId = (m_lastTaskId + 1) & 0x7fffffff;

	return m_lastTaskId;
}


bool CMeilhausSimpleComp::GetChannelAddress(const iprm::IParamsSet* paramsPtr, CMeAddr& result) const
{
	std::string selectionId = (*m_addressParamIdAttrPtr).ToString();
	const iprm::ISelectionParam* selectionPtr = dynamic_cast<const iprm::ISelectionParam*>(paramsPtr->GetParameter(selectionId));

	const CChannelSelectionNode* deviceNodePtr = dynamic_cast<const CChannelSelectionNode*>(selectionPtr);
	if (deviceNodePtr != NULL){
		result.device = deviceNodePtr->GetActivePhysicalAddress();
		const CChannelSelectionNode* subdeviceNodePtr = dynamic_cast<const CChannelSelectionNode*>(deviceNodePtr->GetActiveSubselection());
		if (subdeviceNodePtr != NULL){
			result.subdevice = subdeviceNodePtr->GetActivePhysicalAddress();
			const CChannelSelectionNode* channelNodePtr = dynamic_cast<const CChannelSelectionNode*>(subdeviceNodePtr->GetActiveSubselection());
			if (channelNodePtr != NULL){
				result.channel = channelNodePtr->GetActivePhysicalAddress();

				return true;
			}
		}
	}

	return false;
}


const isig::ISamplingParams* CMeilhausSimpleComp::GetSamplingParams(const iprm::IParamsSet* paramsPtr) const
{
	std::string samplingId = (*m_samplingParamsIdAttrPtr).ToString();

	return dynamic_cast<const isig::ISamplingParams*>(paramsPtr->GetParameter(samplingId));
}


} // namespace imebase


