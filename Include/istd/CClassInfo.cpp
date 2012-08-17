#include "istd/CClassInfo.h"


#ifndef _MSC_VER
#include <cxxabi.h>
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
	int status = 0;
	char* demangledName = abi::__cxa_demangle(rawName.constData(), NULL, NULL, &status);
	QByteArray retVal(demangledName);

	::free(demangledName);
	
	return retVal;
#endif
}


} // namespace istd


