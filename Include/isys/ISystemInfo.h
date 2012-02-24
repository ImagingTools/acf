#ifndef isys_ISystemInfo_included
#define isys_ISystemInfo_included


// Qt includes
#include <QString>


// ACF includes
#include "isys/isys.h"

#include "iser/ISerializable.h"


namespace isys
{


/**
	Common interface to get the information about the system resources.

	\ingroup System
*/
class ISystemInfo: virtual public iser::ISerializable
{
public:
	/**
		Get computer/workstation name.
	*/
	virtual QString GetComputerName() const = 0;
	/**
		Get number of processors installed.
	*/
	virtual int GetProcessorsCount() const = 0;

	/**
		Get ID of the processor with index \c processorIndex.
	*/
	virtual int GetProcessorId(int processorIndex) const = 0;

	/**
		Returns \c true if there are network adapter(s) installed.
	*/
	virtual bool IsNetworkAdapterInstalled(int* adapterCountPtr = NULL) const = 0;

	/**
		Get MAC address of the network adapter.
	*/
	virtual std::string GetMacAdress(int adapterIndex) const = 0;
};


} // namespace isys


#endif // isys_ISystemInfo_included


