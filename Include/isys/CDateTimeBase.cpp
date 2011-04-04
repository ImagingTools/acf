#include "isys/CDateTimeBase.h"


// STL includes
#include <sstream>
#include <iomanip>


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace isys
{


bool CDateTimeBase::SerializeComponents(iser::IArchive& archive, TimeComponent fromComponent, TimeComponent toComponent)
{
	bool retVal = true;

	if (archive.IsStoring()){
		for (int i = fromComponent; i <= toComponent; ++i){
			int componentValue = GetComponent(i);

			retVal = retVal && archive.BeginTag(s_archiveTags[i]);
			retVal = retVal && archive.Process(componentValue);
			retVal = retVal && archive.EndTag(s_archiveTags[i]);
		}
	}
	else{
		for (int i = fromComponent; i <= toComponent; ++i){
			int componentValue = 0;

			retVal = retVal && archive.BeginTag(s_archiveTags[i]);
			retVal = retVal && archive.Process(componentValue);
			retVal = retVal && archive.EndTag(s_archiveTags[i]);

			SetComponent(i, componentValue);
		}
	}

	return retVal;
}


// reimplemented (isys::IDateTime)
 
std::string CDateTimeBase::ToString(int fromComponent, int toComponent) const
{
	std::ostringstream stream;

	for (int i = fromComponent; i <= toComponent; ++i){
		stream << std::setfill('0') << std::setw(2) << GetComponent(i);

		if (i < toComponent){
			if (i < TC_HOUR){
				stream << '-';
			}
			else if (i < TC_MICROSECOND){
				stream << ':';
			}
			else{
				stream << '\'';
			}
		}
	}

	return stream.str();
}


bool CDateTimeBase::FromString(const std::string& text, int fromComponent, int toComponent)
{
	std::istringstream stream(text);

	for (int i = fromComponent; i <= toComponent; ++i){
		int value;
		stream >> value;

		SetComponent(i, value);

		if (i < toComponent){
			std::string dummy;
			stream >> dummy;
		}
	}

	return true;
}


// reimplemented (iser::ISerializable)

bool CDateTimeBase::Serialize(iser::IArchive& archive)
{
	return SerializeComponents(archive, TC_YEAR, TC_MICROSECOND);
}


// protected static attributes

iser::CArchiveTag CDateTimeBase::s_archiveTags[TC_LAST + 1] = {
	iser::CArchiveTag("Year", "Year"),
	iser::CArchiveTag("Month", "Month"),
	iser::CArchiveTag("Day", "Day"),
	iser::CArchiveTag("Hour", "Hour"),
	iser::CArchiveTag("Minute", "Minute"),
	iser::CArchiveTag("Second", "Second"),
	iser::CArchiveTag("Miliecond", "Miliecond")
};


} // namespace isys


