#ifndef icomp_IElementStaticInfo_included
#define icomp_IElementStaticInfo_included


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QSet>

// ACF includes
#include "istd/IPolymorphic.h"


namespace icomp
{


/**
	This interface provide static information about element meta info.
	Elements are defined as weaker abstraction of components, without attributes etc.
	\ingroup ComponentConcept
*/
class IElementStaticInfo: virtual public istd::IPolymorphic
{
public:
	enum MetaGroupId
	{
		/**
			ID of meta group storing list of supported interfaces.
		*/
		MGI_INTERFACES = 0,
		/**
			ID of group for component subelements.
		*/
		MGI_SUBELEMENTS,
		/**
			Last used ID.
		*/
		MGI_LAST = MGI_SUBELEMENTS
	};

	typedef QSet<QByteArray> Ids;

	/**
		Return number of interfaces for specific slot.
		\param	subcomponentId	ID of subcomponent.
								\sa	GetMetaIds() and MGI_SUBELEMENTS.
	*/
	virtual const IElementStaticInfo* GetSubelementInfo(const QByteArray& subcomponentId) const = 0;

	/**
		Get list of meta IDs associated with some meta key.
		\sa MetaGroupId
	*/
	virtual Ids GetMetaIds(int metaGroupId) const = 0;
};


} // namespace icomp


#endif // !icomp_IElementStaticInfo_included


