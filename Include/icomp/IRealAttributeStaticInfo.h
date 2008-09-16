#ifndef icomp_IRealAttributeStaticInfo_included
#define icomp_IRealAttributeStaticInfo_included


#include <string>

#include "istd/IPolymorphic.h"

#include "iser/ISerializable.h"

#include "icomp/icomp.h"


namespace icomp
{


class IRealAttributeStaticInfo: virtual public IAttributeStaticInfo
{
public:
	/**
		Get ID of this attribute.
	 */
	virtual const std::string& GetAttributeId() const = 0;
};


}//namespace icomp


#endif // !icomp_IRealAttributeStaticInfo_included


