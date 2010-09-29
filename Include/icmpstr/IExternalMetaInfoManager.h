#ifndef icmpstr_IExternalMetaInfoManager_included
#define icmpstr_IExternalMetaInfoManager_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "icomp/CComponentAddress.h"


namespace icmpstr
{


class IExternalMetaInfoManager: virtual public istd::IPolymorphic
{
public:
	/**
		Get path of package info directory.
	*/
	virtual istd::CString GetPackageInfoPath(const std::string& packageId) const = 0;

	/**
		Get path of component info directory.
	*/
	virtual istd::CString GetComponentInfoPath(const icomp::CComponentAddress& address) const = 0;
};


} // namespace icmpstr


#endif // !icmpstr_IExternalMetaInfoManager_included


