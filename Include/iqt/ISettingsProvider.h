#ifndef iqt_ISettingsProvider_included
#define iqt_ISettingsProvider_included


// Qt includes
#include <QtCore/QSettings>

// ACF includes
#include <istd/IPolymorphic.h>
#include <iqt/iqt.h>


namespace iqt
{


/**
	Interface for access the application settings object.
*/
class ISettingsProvider: virtual public istd::IPolymorphic
{
public:
	/**
		Get the instance of the Qt settings.
	*/
	virtual QSettings& GetSettings() const = 0;
};


} // namespace iqt


#endif // !iqt_ISettingsProvider_included


