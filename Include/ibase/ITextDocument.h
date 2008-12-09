#ifndef ibase_ITextDocument_included
#define ibase_ITextDocument_included


#include "istd/CString.h"

#include "iser/ISerializable.h"


namespace ibase
{

class ITextDocument: virtual public iser::ISerializable
{
public:
	virtual istd::CString GetText() const = 0;
	virtual void SetText(const istd::CString& text) = 0;
};


} // namespace ibase


#endif // !ibase_ITextDocument_included


