#ifndef ibase_IApplicationInfo_included
#define ibase_IApplicationInfo_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IPolymorphic.h>
#include <iser/IVersionInfo.h>
#include <ibase/ibase.h>


namespace ibase
{


/**
	This interface provides some information about the application.

	\ingroup Main
*/
class IApplicationInfo: virtual public istd::IPolymorphic
{
public:
	enum ApplicationAttribute
	{
		AA_COMPANY_NAME,
		AA_PRODUCT_NAME,
		AA_APPLICATION_NAME,
		AA_APPLICATION_SUBNAME,
		AA_APPLICATION_TYPE,
		AA_APPLICATION_PATH,
		AA_LEGAL_COPYRIGHT,
		AA_MAIN_VERSION,
		AA_USER = 100
	};

	/**
		Get value of application attribute.
		\param	attributeId	ID of application attribute \sa ApplicationAttribute.
	*/
	virtual QString GetApplicationAttribute(int attributeId, bool allowTranslation = true) const = 0;

	/**
		Get ID of main module version ID for \c iser::IVersionInfo.
		This ID is used to identify which module is main application module.
		The meaning is the same as meaning of \c versionId parameters in interface \c iser::IVersionInfo.
		\return	ID of main module, or negative value if it is undefined.
	*/
	virtual int GetMainVersionId() const = 0;

	/**
		Get access to application version info.
	*/
	virtual const iser::IVersionInfo& GetVersionInfo() const = 0;
};


} // namespace ibase


#endif // !ibase_IApplicationInfo_included


