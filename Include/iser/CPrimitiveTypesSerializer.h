#ifndef iser_CPrimitiveTypesSerializer_included
#define iser_CPrimitiveTypesSerializer_included


// Qt includes
#include <QtCore/QDateTime>

// ACF includes
#include "istd/CRange.h"


namespace iser
{
	class IArchive;
}


namespace iser
{


/**
	Implementation range serializer.

	\ingroup Main
*/
class CPrimitiveTypesSerializer
{
public:
	static bool SerializeRange(iser::IArchive& archive, istd::CRange& range);
	static bool SerializeDateTime(iser::IArchive& archive, QDateTime& dateTime);
};

} // namespace iser


#endif // !iser_CPrimitiveTypesSerializer_included

