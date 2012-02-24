#ifndef ibase_ITextDocument_included
#define ibase_ITextDocument_included


#include <QString>

#include "iser/ISerializable.h"


namespace ibase
{


/**
	Simple interface for a text document.
*/
class ITextDocument: virtual public iser::ISerializable
{
public:
	/**
		Get document text.
	*/
	virtual QString GetText() const = 0;

	/**
		Set document text.
	*/
	virtual void SetText(const QString& text) = 0;
};


} // namespace ibase


#endif // !ibase_ITextDocument_included


