#include "isys/CDateTimeBase.h"


// STL includes
#include <sstream>
#include <iomanip>


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace isys
{


// public static members

const IDateTime::TimeSeparator IDateTime::TS_ISO = {'-',' ',':', '.'};
const IDateTime::TimeSeparator IDateTime::TS_FILENAME = {'-',' ','.','.'};


// public methods

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
 
QString CDateTimeBase::ToString(int fromComponent, int toComponent, const TimeSeparator& separator) const
{
	std::ostringstream stream;

	for (int i = fromComponent; i <= toComponent; ++i){
		stream << std::setfill('0') << std::setw(2) << GetComponent(i);

		if (i < toComponent){
			switch (i){
				case TC_YEAR:
				case TC_MONTH:
					stream << separator[0];
					break;

				case TC_DAY:
					stream << separator[1];
					break;

				case TC_HOUR:
				case TC_MINUTE:
					stream << separator[2];
					break;

				default:
					stream << separator[3];
			}
		}
	}

	return QString(stream.str().c_str());
}


bool CDateTimeBase::FromString(const std::string& text, int fromComponent, int toComponent)
{
	ResetAllComponents();

	std::stringstream stream(text);

	for (int i = fromComponent; i <= toComponent; ++i){
		int value;
		stream >> value;

		SetComponent(i, value);

		if (i < toComponent){
			char dummy = ' ';

			// skip all the non-digits
			while (!stream.eof()){
				stream >> dummy;

				if (isdigit(dummy)){
					// return the digit back
					stream.putback(dummy);
					break;
				}
			}
		}
	}

	return true;
}


// reimplemented (iser::ISerializable)

bool CDateTimeBase::Serialize(iser::IArchive& archive)
{
	return SerializeComponents(archive, TC_YEAR, TC_MILLISECOND);
}


// protected methods

void CDateTimeBase::ResetAllComponents()
{
	for (int i = 0; i <= TC_LAST; ++i){
		SetComponent(i, 0);
	}
}


// protected static attributes

iser::CArchiveTag CDateTimeBase::s_archiveTags[TC_LAST + 1] = {
	iser::CArchiveTag("Year", "Year"),
	iser::CArchiveTag("Month", "Month"),
	iser::CArchiveTag("Day", "Day"),
	iser::CArchiveTag("Hour", "Hour"),
	iser::CArchiveTag("Minute", "Minute"),
	iser::CArchiveTag("Second", "Second"),
	iser::CArchiveTag("Millisecond", "Millisecond")
};


} // namespace isys


