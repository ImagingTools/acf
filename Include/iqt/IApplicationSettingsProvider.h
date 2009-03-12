#ifndef iqt_IApplicationSettingsProvider_included
#define iqt_IApplicationSettingsProvider_included


#include "iqt/iqt.h"


#include <QSettings>

#include "istd/IPolymorphic.h"



namespace iqt
{

/**
	Interface for access the application settings object.
*/
class IApplicationSettingsProvider: virtual public istd::IPolymorphic
{
public:
	virtual QSettings& GetApplicationSettings() const = 0;
};


} // namespace iqt


#endif // !iqt_IApplicationSettingsProvider_included
