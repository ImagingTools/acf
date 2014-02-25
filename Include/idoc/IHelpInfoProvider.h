#ifndef idoc_IHelpInfoProvider_included
#define idoc_IHelpInfoProvider_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "istd/IPolymorphic.h"


namespace idoc
{


/**
	Allow to show help document to specified object or text.
*/
class IHelpInfoProvider: virtual public istd::IPolymorphic
{
public:
	/**
		Get quality of help information for specified context and/or object.
		\return	quality of help. It represented as value in range [0, 1].
				0 means there is no usable help, 1 means good context specified help is available.
	*/
	virtual double GetHelpQuality(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const = 0;
};


} // namespace idoc


#endif // !idoc_IHelpInfoProvider_included


