#ifndef icmpstr_IComponentNote_included
#define icmpstr_IComponentNote_included


#include "iser/IObject.h"

#include "i2d/CVector2d.h"

#include "istd/CString.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


class IComponentNote: virtual public iser::IObject
{
public:
	virtual istd::CString GetText() const = 0;
	virtual void SetText(const istd::CString& noteText) = 0;
	virtual i2d::CVector2d GetPosition() const = 0;
	virtual void SetPosition(const i2d::CVector2d& position) = 0;
};


} // namespace icmpstr


#endif // !IComponentNote_included

