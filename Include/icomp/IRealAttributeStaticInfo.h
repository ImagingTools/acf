#ifndef icomp_IRealAttributeStaticInfo_included
#define icomp_IRealAttributeStaticInfo_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include "icomp/IAttributeStaticInfo.h"


namespace icomp
{


/**
	Interface adding to attribute static info functionality existing only for real attributes.
*/
class IRealAttributeStaticInfo: virtual public IAttributeStaticInfo
{
public:
	/**
		Get ID of this attribute.
	 */
	virtual const QByteArray& GetAttributeId() const = 0;
	/**
		Add related interface to this attribute info.
		\param	relatedInterfaceInfo	specify interface.
		\param	flags					set of flags defined in \c icomp::IAttributeStaticInfo::AttributeFlags.
	*/
	virtual void AddRelatedMetaId(int metaGroupId, const QByteArray& id, int flags) = 0;
};


} // namespace icomp


#endif // !icomp_IRealAttributeStaticInfo_included


