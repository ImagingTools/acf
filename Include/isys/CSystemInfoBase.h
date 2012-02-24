#ifndef isys_CSystemInfoBase_included
#define isys_CSystemInfoBase_included


#include "isys/ISystemInfo.h"

#include <vector>


namespace isys
{


/**
	Basic implementation for system info interface.
*/
class CSystemInfoBase: virtual public isys::ISystemInfo
{
public:
	// reimplemented (isys::ISystemInfo)
	virtual QString GetComputerName() const;
	virtual int GetProcessorsCount() const;
	virtual int GetProcessorId(int processorIndex) const;
	virtual bool IsNetworkAdapterInstalled(int* adapterCountPtr = NULL) const;
	virtual std::string GetMacAdress(int adapterIndex) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	typedef std::vector<I_DWORD> ProcessorIds;
	typedef std::vector<std::string> NetworkAdapters;

	QString m_computerName;
	ProcessorIds m_processorIds;
	NetworkAdapters m_networkAdapters;
};


} // namespace isys


#endif // isys_CSystemInfoBase_included


