#ifndef idoc_IMultiPageDocumentProvider_included
#define idoc_IMultiPageDocumentProvider_included


// ACF includes
#include <istd/IChangeable.h>


namespace idoc
{


class IMultiPageDocument;


/**
	Common interface for a provider of a multi-page document.
*/
class IMultiPageDocumentProvider: virtual public istd::IChangeable
{
public:
	/**
		Get multi-page document instance.
	*/
	virtual const IMultiPageDocument* GetDocument() const = 0;
};


} // namespace idoc


#endif // !idoc_IMultiPageDocumentProvider_included


