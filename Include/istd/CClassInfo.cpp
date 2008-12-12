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

	return retVal;
#else
	int nameSize = int(rawName.size());
	if (nameSize > 2){
		if (rawName[0] == 'N'){
			int namespacePos;
			int namespaceLength = ParseToNumber(
						&rawName[1],
						nameSize - 1,
						namespacePos);
			if (namespacePos + namespaceLength + 1 < nameSize){
				int classNamePos;
				int classNameLength = ParseToNumber(
							&rawName[namespacePos + namespaceLength + 1],
							nameSize - namespacePos - namespaceLength - 1,
							classNamePos);
				return rawName.substr(namespacePos + 1, namespaceLength) + "::" + rawName.substr(classNamePos + namespacePos + namespaceLength + 1, classNameLength);
			}
		}
		else{
			int classNamePos;
			int classNameLength = ParseToNumber(
						&rawName[0],
						nameSize,
						classNamePos);
			return rawName.substr(classNamePos, classNameLength);
		}
	}
	
	return rawName;
#endif
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


