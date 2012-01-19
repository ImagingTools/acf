#ifndef ibase_CRangeSerializer_included
#define ibase_CRangeSerializer_included


#include "istd/CRange.h"

#include "ibase/ibase.h"


namespace iser
{
	class IArchive;
}


namespace ibase
{


/**
	Implementation range serializer.

	\ingroup Main
*/
class CRangeSerializer
{
public:
	static bool SerializeRange(iser::IArchive& archive, istd::CRange& range);
};

} // namespace ibase


#endif // !ibase_CRangeSerializer_included

