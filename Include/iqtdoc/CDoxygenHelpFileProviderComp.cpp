#include "iqtdoc/CDoxygenHelpFileProviderComp.h"


// Qt includes
#include <QFileInfo>

#include "istd/CClassInfo.h"


namespace iqtdoc
{


// reimplemented (idoc::IHelpFileProvider)

double CDoxygenHelpFileProviderComp::GetHelpQuality(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const
{
	QFileInfo fileInfo(iqt::GetQString(CalcFilePath(CalcClassInfo(contextText, contextObjectPtr))));

	if (fileInfo.exists()){
		return 1.0;
	}
	else if (m_slaveProvider.IsValid()){
		return m_slaveProvider->GetHelpQuality(contextText, contextObjectPtr);
	}
	else{
		return 0.0;
	}
}


istd::CString CDoxygenHelpFileProviderComp::GetHelpFilePath(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const
{
	QFileInfo fileInfo(iqt::GetQString(CalcFilePath(CalcClassInfo(contextText, contextObjectPtr))));

	if (fileInfo.exists()){
		return iqt::GetCString(fileInfo.absoluteFilePath());
	}
	else if (m_slaveProvider.IsValid()){
		return m_slaveProvider->GetHelpFilePath(contextText, contextObjectPtr);
	}
	else{
		return "";
	}
}


// protected methods

istd::CClassInfo CDoxygenHelpFileProviderComp::CalcClassInfo(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const
{
	if (contextObjectPtr != NULL){
		const istd::CClassInfo* classInfoPtr = dynamic_cast<const istd::CClassInfo*>(contextObjectPtr);
		if (classInfoPtr != NULL){
			return *classInfoPtr;
		}
		else{
			return istd::CClassInfo(*contextObjectPtr);
		}
	}
	else{
		return istd::CClassInfo(contextText.ToString());
	}
}


istd::CString CDoxygenHelpFileProviderComp::CalcFilePath(const istd::CClassInfo& classInfo) const
{
	istd::CString retVal = *m_doxygenDirectoryAttrPtr + "/class";

	std::string className = classInfo.GetName();
	for (		std::string::const_iterator iter = className.begin();
				iter != className.end();
				++iter){
		char c = *iter;
		if (c == ':'){
			retVal += "_1";
		}
		else if ((c >= 'A') && (c <= 'Z')){
			retVal += "_";
			retVal.push_back(c - 'A' + 'a');
		}
		else{
			retVal.push_back(c);
		}
	}

	retVal += ".html";

	return retVal;
}


} // namespace iqtdoc


