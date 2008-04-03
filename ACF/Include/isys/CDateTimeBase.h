#ifndef isys_CDateTimeBase_included
#define isys_CDateTimeBase_included


#include "iser/CArchiveTag.h"

#include "isys/IDateTime.h"


namespace isys
{


class CDateTimeBase: public IDateTime
{
public:
	bool SerializeComponents(iser::IArchive& archive, TimeComponent fromComponent, TimeComponent toComponent);

	// reimplemented (isys::IDateTime)
	virtual std::string ToString(int fromComponent = TC_YEAR, int toComponent = TC_MICROSECOND) const;
	virtual bool FromString(const std::string& dateTime, int fromComponent = TC_YEAR, int toComponent = TC_MICROSECOND);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	static iser::CArchiveTag s_archiveTags[TC_LAST + 1];
};


} // namespace isys


#endif // !isys_CDateTimeBase_included


