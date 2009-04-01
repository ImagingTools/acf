#ifndef idoc_IHelpViewer_included
#define idoc_IHelpViewer_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"


namespace idoc
{


/**
	Allow to show help document to specified object or text.
*/
class IHelpViewer: virtual public istd::IPolymorphic
{
public:
	virtual void ShowHelp(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const = 0;
};


} // namespace idoc


#endif // !idoc_IHelpViewer_included


