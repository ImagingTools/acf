#ifndef icomp_CRelatedInfoRegistrator_included
#define icomp_CRelatedInfoRegistrator_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <icomp/IRealAttributeStaticInfo.h>


namespace icomp
{


class CRelatedInfoRegistrator
{
public:
	CRelatedInfoRegistrator(
				IRealAttributeStaticInfo& baseAttributeInfo,
				int metaGroupId,
				const QByteArray& id,
				int flags);
};


} // namespace icomp


#endif // !icomp_CRelatedInfoRegistrator_included


