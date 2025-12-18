#ifndef icomp_IComponentListProvider_included
#define icomp_IComponentListProvider_included


// ACF includes
#include <icomp/IComponentStaticInfo.h>
#include <icomp/CComponentAddress.h>


namespace icomp
{


class IComponentListProvider: virtual public istd::IChangeable
{
public:
	enum ComponentTypeFlag
	{
		CTF_REAL = 1 << IComponentStaticInfo::CT_REAL,
		CTF_COMPOSITE = 1 << IComponentStaticInfo::CT_COMPOSITE,
		CTF_ALL = -1
	};

	typedef QSet<CComponentAddress> ComponentAddresses;

	/**
		Get addresses of known components.
	*/
	virtual ComponentAddresses GetComponentAddresses(int typeFlag = CTF_ALL) const = 0;
};


} // namespace icomp


#endif // !icomp_IComponentListProvider_included


