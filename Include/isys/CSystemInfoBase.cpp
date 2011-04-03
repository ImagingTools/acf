#include "isys/CSystemInfoBase.h"


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace isys
{


// public methods

// reimplemented (isys::ISystemInfo)

istd::CString CSystemInfoBase::GetComputerName() const
{
	return m_computerName;
}


int CSystemInfoBase::GetProcessorsCount() const
{
	return m_processorIds.size();
}


int CSystemInfoBase::GetProcessorId(int processorIndex) const
{
	I_ASSERT(processorIndex < int(m_processorIds.size()));
	I_ASSERT(processorIndex >= 0);

	return m_processorIds[processorIndex];
}


bool CSystemInfoBase::IsNetworkAdapterInstalled(int* adapterCountPtr) const
{
	if (adapterCountPtr != NULL){
		*adapterCountPtr = int(m_networkAdapters.size());
	}

	return (!m_networkAdapters.empty());
}


std::string CSystemInfoBase::GetMacAdress(int adapterIndex) const
{
	I_ASSERT(adapterIndex < int(m_networkAdapters.size()));
	I_ASSERT(adapterIndex >= 0);

	return m_networkAdapters[adapterIndex];
}


// reimplemented (iser::ISerializable)

bool CSystemInfoBase::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag computerNameTag("ComputerName", "ComputerName");
	retVal = retVal && archive.BeginTag(computerNameTag);
	retVal = retVal && archive.Process(m_computerName);
	retVal = retVal && archive.EndTag(computerNameTag);

	static iser::CArchiveTag processorIdsTag("ProcessorIds", "ProcessorIds");
	static iser::CArchiveTag processorIdTag("ProcessorId", "ProcessorId");
	int processorsCount = m_processorIds.size();

	retVal = retVal && archive.BeginMultiTag(processorIdsTag, processorIdTag, processorsCount);
	if (!archive.IsStoring()){
		m_processorIds.resize(processorsCount);
	}

	for (int processorIndex = 0; processorIndex < processorsCount; processorIndex++){
		retVal = retVal && archive.BeginTag(processorIdTag);
		retVal = retVal && archive.Process(m_processorIds[processorIndex]);
		retVal = retVal && archive.EndTag(processorIdTag);
	}

	retVal = retVal && archive.EndTag(processorIdsTag);

	static iser::CArchiveTag networkAdaptersTag("NetworkAdapters", "NetworkAdapters");
	static iser::CArchiveTag networkAdapterTag("NetworkAdapter", "NetworkAdapter");
	int networkAdaptersCount = m_networkAdapters.size();

	retVal = retVal && archive.BeginMultiTag(networkAdaptersTag, networkAdapterTag, networkAdaptersCount);
	if (!archive.IsStoring()){
		m_networkAdapters.resize(networkAdaptersCount);
	}

	for (int processorIndex = 0; processorIndex < processorsCount; processorIndex++){
		retVal = retVal && archive.BeginTag(networkAdapterTag);
		retVal = retVal && archive.Process(m_networkAdapters[processorIndex]);
		retVal = retVal && archive.EndTag(networkAdapterTag);
	}

	retVal = retVal && archive.EndTag(networkAdaptersTag);

	return retVal;
}


} // namespace isys


