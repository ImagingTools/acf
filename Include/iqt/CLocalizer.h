#ifndef iqt_CLocalizer_included
#define iqt_CLocalizer_included


// Qt includes
#include <QTime>


// ACF includes
#include "istd/ILocalizer.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	An timer implementation for Qt library.
*/
class CLocalizer: virtual public istd::ILocalizer
{
public:
	// reimplemented (istd::ILocalizer)
	virtual QString GetTranslatedText(
				const std::string& context,
				const QString& text,
				const std::string& disambiguation = std::string()) const;
};


} // namespace iqt


#endif // !iqt_CLocalizer_included
