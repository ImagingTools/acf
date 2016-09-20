#ifndef iprm_INameParam_included
#define iprm_INameParam_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <iser/ISerializable.h>


namespace iprm
{


/**
	Interface for a object containing simple text.
	\todo Rename it to ITextParam
*/
class INameParam: virtual public iser::ISerializable
{
public:
	/**
		Data model change notification flags.
	*/
	enum ChangeFlags
	{
		CF_RENAME = 0x15345e0
	};

	/**
		Get the object name.
	*/
	virtual const QString& GetName() const = 0;

	/**
		Set the object name.
	*/
	virtual void SetName(const QString& name) = 0;

	/**
		Return \c true, if the name setting is enabled.
	*/
	virtual bool IsNameFixed() const = 0;
};


} // namespace iprm


#endif // !iprm_INameParam_included
