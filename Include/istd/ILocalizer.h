#ifndef istd_ILocalizer_included
#define istd_ILocalizer_included


// STL includes
#include <string>


// ACF includes
#include "istd/IPolymorphic.h"


class QString;


namespace istd
{


/**
	Common interface for a simple string localization.
*/
class ILocalizer: virtual public istd::IPolymorphic
{
public:
	/**
		Translate text \c text in the context \c context to defined language.
	*/
	virtual QString GetTranslatedText(
				const std::string& context,
				const QString& text,
				const std::string& disambiguation = std::string()) const = 0;
};


} // namespace istd


#endif // !istd_ILocalizer_included


