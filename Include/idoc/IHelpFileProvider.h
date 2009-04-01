#ifndef idoc_IHelpFileProvider_included
#define idoc_IHelpFileProvider_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"


namespace idoc
{


/**
	Allow to show help document to specified object or text.
*/
class IHelpFileProvider: virtual public istd::IPolymorphic
{
public:
	virtual double GetHelpQuality(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const = 0;
	virtual istd::CString GetHelpFilePath(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const = 0;
};


} // namespace idoc


#endif // !idoc_IHelpFileProvider_included


