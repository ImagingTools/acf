#include "istd/CClassInfo.h"


#ifndef _MSC_VER
// static help functions

static int ParseToNumber(const char* buffer, int maxLength, int& nextPosition)
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


std::string ParseRecursive(const std::string& rawName, std::string ns, int& position)
{
	std::string retVal;
	
	std::string separator;

	position = position;

	int nameSize = int(rawName.size());
	while (position < nameSize - 2){
		char code = rawName[position];
		if ((code >= '0') && (code <= '9')){
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

			separator = "";
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

			std::string ns = rawName.substr(position + substrPos, substrLength);
			
			position += substrPos + substrLength;

			retVal += ns + "::";

			separator = "";
		}
		else if (code == 'E'){
			++position;

			break;
		}
		else if (code == 'I'){
			++position;

			retVal += "<" + ParseRecursive(rawName, ns, position) + ">";

			separator = "";
		}
		else if (code == 'S'){
			position += 2;

			if (!ns.empty()){
				retVal += ns + "::";
			}

			separator = "";
		}
		else if (code == 'v'){
			++position;

			retVal += separator + "void";

			separator = ",";
		}
		else if (code == 'b'){
			++position;

			retVal += separator + "bool";

			separator = ",";
		}
		else if (code == 'c'){
			++position;

			retVal += separator + "char";

			separator = ",";
		}
		else if (code == 'a'){
			++position;

			retVal += separator + "signed char";

			separator = ",";
		}
		else if (code == 'h'){
			++position;

			retVal += separator + "unsigned char";

			separator = ",";
		}
		else if (code == 's'){
			++position;

			retVal += separator + "short";

			separator = ",";
		}
		else if (code == 't'){
			++position;

			retVal += separator + "unsigned short";

			separator = ",";
		}
		else if (code == 'i'){
			++position;

			retVal += separator + "int";

			separator = ",";
		}
		else if (code == 'j'){
			++position;

			retVal += separator + "unsigned int";

			separator = ",";
		}
		else if (code == 'l'){
			++position;

			retVal += separator + "long";

			separator = ",";
		}
		else if (code == 'm'){
			++position;

			retVal += separator + "unsigned long";

			separator = ",";
		}
		else if (code == 'x'){
			++position;

			retVal += separator + "long long";

			separator = ",";
		}
		else if (code == 'y'){
			++position;

			retVal += separator + "unsigned long long";

			separator = ",";
		}
		else if (code == 'f'){
			++position;

			retVal += separator + "float";

			separator = ",";
		}
		else if (code == 'd'){
			++position;

			retVal += separator + "double";

			separator = ",";
		}
		else if (code == 'e'){
			++position;

			retVal += separator + "long double";

			separator = ",";
		}
		else if (code == 'K'){
			++position;

			retVal += separator + "const ";

			separator = "";
		}
		else if (code == 'V'){
			++position;

			retVal += separator + "volatile ";

			separator = "";
		}
		else{
			++position;
		}
	}
	
	return retVal;
}


#endif //!_MSC_VER


namespace istd
{


bool CClassInfo::IsTemplateClass() const
{
	std::string::size_type ltPos = m_name.find_first_of('<');
	if (ltPos != std::string::npos){
		std::string::size_type gtPos = m_name.find_last_of('>');
		if ((gtPos != std::string::npos) && (ltPos < gtPos)){
			return true;
		}
	}

	return false;
}


int CClassInfo::GetTemplateParamsCount() const
{
	std::string::size_type beginPos = m_name.find_first_of('<');
	if (beginPos != std::string::npos){
		std::string::size_type gtPos = m_name.find_last_of('>');
		if ((gtPos != std::string::npos) && (beginPos < gtPos)){
			int counter;
			for (counter = 0; (beginPos = m_name.find_first_of(',', beginPos)) != std::string::npos; ++counter);

			return counter;
		}
	}

	return -1;
}


CClassInfo CClassInfo::GetTemplateParam(int paramIndex) const
{
	std::string::size_type beginPos = m_name.find_first_of('<');
	if (beginPos != std::string::npos){
		std::string::size_type gtPos = m_name.find_last_of('>');
		if ((gtPos != std::string::npos) && (beginPos < gtPos)){
			int counter;
			for (counter = 0; counter < paramIndex; ++counter){
				beginPos = m_name.find_first_of(',', beginPos);

				if ((beginPos == std::string::npos) || (beginPos < gtPos)){
					return CClassInfo();
				}
			}

			return CClassInfo(m_name.substr(beginPos + 1, gtPos - beginPos - 1));
		}
	}

	return CClassInfo();
}


std::string CClassInfo::GetName(const std::type_info& info)
{
	return GetUndecoratedName(info.name());
}


std::string CClassInfo::GetName(const istd::IPolymorphic& object)
{
	return GetUndecoratedName(typeid(object).name());
}


// protected methods

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
	int position = 0;
	
	return ParseRecursive(rawName, "", position);
#endif
}


// static attributes

istd::CClassInfo CClassInfo::s_voidType = istd::CClassInfo::GetInfo<void>();


} // namespace istd


