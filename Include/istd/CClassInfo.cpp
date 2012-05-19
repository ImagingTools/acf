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


QByteArray ParseRecursive(const QByteArray& rawName, QByteArray ns, int& position)
{
	QByteArray retVal;
	
	QByteArray separator;

	position = position;

	int nameSize = int(rawName.size());
	while (position < nameSize - 2){
		char code = rawName[position];
		if ((code >= '0') && (code <= '9')){
			int substrPos;
			int substrLength = ParseToNumber(
                        rawName.constData() + position,
						nameSize - position,
						substrPos);
			if (substrLength > nameSize - position - substrPos){
				substrLength = nameSize - position - substrPos;
			}

            retVal += rawName.mid(position + substrPos, substrLength);

			position += substrPos + substrLength;

			separator = "";
		}
		else if (code == 'N'){
			++position;

			int substrPos;
			int substrLength = ParseToNumber(
                        rawName.constData() + position,
						nameSize - position,
						substrPos);
			if (substrLength > nameSize - position - substrPos){
				substrLength = nameSize - position - substrPos;
			}

            QByteArray ns = rawName.mid(position + substrPos, substrLength);
			
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

			if (!ns.isEmpty()){
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


bool CClassInfo::IsConst() const
{
	return (m_name.left(6) == "const ");
}


CClassInfo CClassInfo::GetConstCasted(bool enableConst) const
{
	if (enableConst){
		if (!IsConst()){
			return CClassInfo("const " + m_name);
		}
	}
	else{
		if (IsConst()){
			return CClassInfo(m_name.mid(6));
		}
	}

	return *this;
}


bool CClassInfo::ConstCast(bool enableConst)
{
	if (enableConst){
		if (!IsConst()){
			m_name = "const " + m_name;

			return true;
		}
	}
	else{
		if (IsConst()){
			m_name = m_name.mid(6);

			return true;
		}
	}

	return false;
}


bool CClassInfo::IsTemplateClass() const
{
	int ltPos = m_name.indexOf('<');
	if (ltPos >= 0){
		int gtPos = m_name.lastIndexOf('>');
		if ((gtPos >= 0) && (ltPos < gtPos)){
			return true;
		}
	}

	return false;
}


int CClassInfo::GetTemplateParamsCount() const
{
	int beginPos = m_name.indexOf('<');
	if (beginPos >= 0){
		int gtPos = m_name.lastIndexOf('>');
		if ((gtPos >= 0) && (beginPos < gtPos)){
			int counter;
			for (counter = 0; (beginPos = m_name.indexOf(',', beginPos + 1)) >= 0; ++counter);

			return counter;
		}
	}

	return -1;
}


CClassInfo CClassInfo::GetTemplateParam(int paramIndex) const
{
	int beginPos = m_name.indexOf('<');
	if (beginPos >= 0){
		int gtPos = m_name.lastIndexOf('>');
		if ((gtPos >= 0) && (beginPos < gtPos)){
			int counter;
			for (counter = 0; counter < paramIndex; ++counter){
				beginPos = m_name.indexOf(',', beginPos + 1);

				if ((beginPos >= 0) || (beginPos < gtPos)){
					return CClassInfo();
				}
			}

			return CClassInfo(m_name.mid(beginPos + 1, gtPos - beginPos - 1));
		}
	}

	return CClassInfo();
}


QByteArray CClassInfo::GetName(const std::type_info& info)
{
	return GetUndecoratedName(info.name());
}


QByteArray CClassInfo::GetName(const istd::IPolymorphic& object)
{
	return GetUndecoratedName(typeid(object).name());
}


// protected methods

QByteArray CClassInfo::GetUndecoratedName(const QByteArray& rawName)
{
#ifdef _MSC_VER
	QByteArray retVal = rawName;
	int position;
	while ((position = retVal.indexOf("class ")) >= 0){
		retVal = retVal.left(position) + retVal.mid(position + 6);
	}

	return retVal;
#else
	int position = 0;
	
	return ParseRecursive(rawName, "", position);
#endif
}


} // namespace istd


