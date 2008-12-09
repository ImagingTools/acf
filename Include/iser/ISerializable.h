#ifndef iser_ISerializable_included
#define iser_ISerializable_included


#include "istd/IChangeable.h"

#include "iser/IVersionInfo.h"


namespace iser
{


class IArchive;


/**
	Common class for all classes which objects can be archived or restored from archive.
 */
class ISerializable: virtual public istd::IChangeable
{
public:
	/**
		Load or store state of this object as a archive stream.
		Type of operation is depending on archive type.
		\sa iser::IArchive
	*/
	virtual bool Serialize(IArchive& archive) = 0;
	/**
		Get minimal needed version to correct storing of this data.
		In implementation shold be calculated maximum version of this and composed component.
	*/
	virtual I_DWORD GetMinimalVersion(int versionId = IVersionInfo::UserVersionId) const;
};


// inline methods

inline I_DWORD ISerializable::GetMinimalVersion(int /*versionId*/) const
{
	return 0;
}


} // namespace iser


#endif // !iser_ISerializable_included


