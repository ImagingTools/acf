#ifndef idoc_IHelpFileProvider_included
#define idoc_IHelpFileProvider_included


// Qt includes
#include <QString>


// ACF includes
#include "istd/IPolymorphic.h"

#include "idoc/IHelpInfoProvider.h"


namespace idoc
{


/**
	Allow to show help document to specified object or text.
*/
class IHelpFileProvider: virtual public IHelpInfoProvider
{
public:
	/**
		Get access to help file path for specified help information.
	*/
	virtual QString GetHelpFilePath(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const = 0;
};


} // namespace idoc


#endif // !idoc_IHelpFileProvider_included


