#ifndef IComponentNote_included
#define IComponentNote_included


#include "iser/ISerializable.h"


#include "i2d/CVector2d.h"

#include "istd/CString.h"


class IComponentNote: virtual public iser::ISerializable
{
public:
	virtual istd::CString GetText() const;
	virtual i2d::CVector2d GetPosition() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
};


#endif // !IComponentNote_included

