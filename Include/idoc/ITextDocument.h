#ifndef idoc_ITextDocument_included
#define idoc_ITextDocument_included


#include <QtCore/QString>

#include <iser/ISerializable.h>


namespace idoc
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


} // namespace idoc


#endif // !idoc_ITextDocument_included


