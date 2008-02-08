#include "iser/CXmlDocumentInfoBase.h"


namespace iser
{


const istd::CString& CXmlDocumentInfoBase::GetDefaultRootName()
{
	return s_defaultRootName;
}


// static attributes

istd::CString CXmlDocumentInfoBase::s_defaultRootName("AcfDocument");


} // namespace iser


