#ifndef icomp_CRelatedInfoRegistrator_included
#define icomp_CRelatedInfoRegistrator_included


// STL includes
#include <string>

#include "icomp/IRealAttributeStaticInfo.h"


namespace icomp
{


class CRelatedInfoRegistrator
{
public:
	CRelatedInfoRegistrator(
				IRealAttributeStaticInfo& baseAttributeInfo,
				int metaGroupId,
				const std::string& id,
				int flags);
};


// public inline methods

inline CRelatedInfoRegistrator::CRelatedInfoRegistrator(
			IRealAttributeStaticInfo& baseAttributeInfo,
			int metaGroupId,
			const std::string& id,
			int flags)
{
	baseAttributeInfo.AddRelatedMetaId(metaGroupId, id, flags);
}


}//namespace icomp


#endif // !icomp_CRelatedInfoRegistrator_included


