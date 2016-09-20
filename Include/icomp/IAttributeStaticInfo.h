#ifndef icomp_IAttributeStaticInfo_included
#define icomp_IAttributeStaticInfo_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <istd/IPolymorphic.h>
#include <iser/IObject.h>
#include <iattr/IAttributeMetaInfo.h>
#include <icomp/IElementStaticInfo.h>


namespace icomp
{


/**
	Describe component attribute static information.
*/
class IAttributeStaticInfo: virtual public iattr::IAttributeMetaInfo
{
public:
	/**
		Flags signalizing set of attribute properties.
		Extension of \c iattr::IAttributeMetaInfo::AttributeFlags.
	*/
	enum AttributeFlags
	{
		/**
			Attribute is reference to another component.
		*/
		AF_REFERENCE = 1 << 5,
		/**
			Attribute is factory of another component.
		*/
		AF_FACTORY = 1 << 6
	};

	/**
		Get related meta information.
		This information in normally used by references and factories to provide information about related interfaces.

		\param	metaGroupId	ID of data group of asked meta ID's. @sa icomp::IComponentStaticInfo::MetaGroupId
		\param	flags		flag value of element. \sa iattr::IAttributeMetaInfo::AttributeFlags.
		\param	flagsMask	mask value of element.
							Each element has flags. Only elements where element_flag & flagsMask == flags will be returned.
	*/
	virtual IElementStaticInfo::Ids GetRelatedMetaIds(int metaGroupId, int flags, int flagsMask) const = 0;
};


} // namespace icomp


#endif // !icomp_IAttributeStaticInfo_included


