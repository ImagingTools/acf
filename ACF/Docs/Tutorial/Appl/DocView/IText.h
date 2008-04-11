#ifndef IText_included
#define IText_included


#include "istd/CString.h"

#include "iser/ISerializable.h"


class IText: public iser::ISerializable
{
public:
	virtual istd::CString GetText() const = 0;
	virtual void SetText(const istd::CString& text) = 0;
};


#endif // !IText_included


