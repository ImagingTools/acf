#include "istd/CClassInfo.h"


namespace istd
{


std::string CClassInfo::GetName(const std::type_info& info)
{
	return GetUndecoratedName(info.name());
}


std::string CClassInfo::GetName(const istd::IPolymorphic& object)
{
	return GetUndecoratedName(typeid(object).name());
}


std::string CClassInfo::GetUndecoratedName(const std::string& rawName)
{
#ifdef _MSC_VER
	std::string retVal = rawName;
	std::string::size_type position;
	while ((position = retVal.find("class ")) != std::string::npos){
		retVal = retVal.substr(0, position) + retVal.substr(position + 6);
	}
#else
	std::string retVal;
	int position = 0;

	int nameSize = int(rawName.size());
	while (position < nameSize - 2){
		char code = rawName[position];
		if (code >= '0') && (code <= '9'){
			int substrPos;
			int substrLength = ParseToNumber(
						&rawName[position],
						nameSize - position,
						substrPos);
			if (substrLength > nameSize - position - substrPos){
				substrLength = nameSize - position - substrPos;
			}

			retVal += rawName.substr(position + substrPos, substrLength);

			position += substrPos + substrLength;
		}
		else if (code == 'N'){
			++position;

			int substrPos;
			int substrLength = ParseToNumber(
						&rawName[position],
						nameSize - position,
						substrPos);
			if (substrLength > nameSize - position - substrPos){
				substrLength = nameSize - position - substrPos;
			}

			retVal += rawName.substr(position + substrPos, substrLength) + "::";

			position += substrPos + substrLength;
		}
		else if (code == 'E'){
			break;
		}
		else{
			I_CRITICAL();	// This type cannot be parsed, please extend the rules!
		}
	}
#endif

	return retVal;
}


// private static methods

int CClassInfo::ParseToNumber(const char* buffer, int maxLength, int& nextPosition)
{
	int retVal = 0;

	for (		nextPosition = 0;
				(nextPosition < maxLength) && (buffer[nextPosition] >= '0') && (buffer[nextPosition] <= '9');
				++nextPosition){
		retVal *= 10;
		retVal += int(buffer[nextPosition] - '0');
	}

	return retVal;
}


} // namespace istd


