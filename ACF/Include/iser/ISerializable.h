#ifndef iser_ISerializable_included
#define iser_ISerializable_included


#include "istd/IPolymorphic.h"


namespace iser
{


class IArchive;



/**
	Common class for all classes which objects can be archived or restored from archive.
 */
class ISerializable: virtual public istd::IPolymorphic
{
public:
	/**	Load or store state of this object as a archive stream.
	 *		Type of operation is depending on archive type.
	 *		@sa iser::IArchive
	 */
	virtual bool Serialize(IArchive& archive) = 0;
};



} // namespace iser



#endif // iser_ISerializable_included



