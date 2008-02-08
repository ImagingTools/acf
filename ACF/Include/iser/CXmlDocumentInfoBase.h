#ifndef iser_CXmlDocumentInfoBase_included
#define iser_CXmlDocumentInfoBase_included


#include "istd/CString.h"

#include "iser/iser.h"


namespace iser
{


/**
	Base class for all XML implementations for ACF documents.
*/
class CXmlDocumentInfoBase
{
public:
	static const istd::CString& GetDefaultRootName();

private:
	static istd::CString s_defaultRootName;
};


} // namespace iser


#endif // !iser_CXmlDocumentInfoBase_included


