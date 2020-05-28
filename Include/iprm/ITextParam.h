#ifndef iprm_ITextParam_included
#define iprm_ITextParam_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <iser/ISerializable.h>


namespace iprm
{


/**
	Interface for a object containing simple text.
*/
class ITextParam: virtual public iser::ISerializable
{
public:
	/**
		Get the text.
	*/
	virtual QString GetText() const = 0;

	/**
		Set the text.
	*/
	virtual void SetText(const QString& text) = 0;

	/**
		Return \c true, if the text is readonly.
	*/
	virtual bool IsReadOnly() const = 0;
};


} // namespace iprm


#endif // !iprm_ITextParam_included
