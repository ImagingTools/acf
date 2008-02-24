#ifndef ibase_IObject_included
#define ibase_IObject_included


#include "ibase/ibase.h"


#include "istd/IChangeable.h"

#include "iser/ISerializable.h"


namespace ibase
{


class IObject: virtual public IChangeable, virtual public ISerializable
{
public:
	virtual istd::CString& GetFactoryId() const;

	static istd::CString& GetStaticFactoryId() const;
};


// public methods 

inline istd::CString& IObject::GetFactoryId() const
{
	return GetStaticFactoryId();
}


// public static methods 

inline istd::CString& IObject::GetStaticFactoryId() const
{
	static istd::CString emptyId;

	return emptyId;
}


} // namespace ibase


#endif // !ibase_IObject_included

