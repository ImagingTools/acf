#ifndef istd_IInformationProvider_included
#define istd_IInformationProvider_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QDateTime>

// ACF includes
#include "istd/IChangeable.h"


namespace istd
{


/**
	Common interface for class providing some basic information object.
*/
class IInformationProvider: virtual public IChangeable
{
public:
	/**
		Category of information.
	*/
	enum InformationCategory
	{
		IC_INFO,
		IC_WARNING,
		IC_ERROR,
		IC_CRITICAL
	};

	/**
		Additionaly information flags.
	*/
	enum InformationTypeFlags
	{
		ITF_DEBUG = 0x1,
		ITF_SYSTEM = 0x2,
		ITF_USER = 0x4
	};

	/**
		Get optional time stamp of the information.
		\return time stamp associated with this information, or null QDateTime object, if there is no associated time stamp.
	*/
	virtual QDateTime GetInformationTimeStamp() const = 0;

	/**
		Get category of the information.
		\sa InformationCategory
	*/
	virtual InformationCategory GetInformationCategory() const = 0;

	/**
		Get binary ID of the information using to automatical identification of this information type.
		If there is no information ID it returns negative value.
	*/
	virtual int GetInformationId() const = 0;

	/**
		Get the text description of the information.
		If there is no description provided it contains empty string.
	*/
	virtual QString GetInformationDescription() const = 0;

	/**
		Get the source of the information.
		If there is no source information provided it contains empty string.
	*/
	virtual QString GetInformationSource() const = 0;

	/**
		Get flags of the information.
		\sa InformationFlags
	*/
	virtual int GetInformationFlags() const = 0;
};


} // namespace istd


#endif // !istd_IInformationProvider_included


