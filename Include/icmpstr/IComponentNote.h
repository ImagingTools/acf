#ifndef IComponentNote_included
#define IComponentNote_included


#include "iser/ISerializable.h"


#include "i2d/CVector2d.h"

#include "istd/CString.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


class IComponentNote: virtual public iser::ISerializable
{
public:
	virtual istd::CString GetText() const = 0;
	virtual i2d::CVector2d GetPosition() const = 0;
};


} // namespace icmpstr


#endif // !IComponentNote_included

