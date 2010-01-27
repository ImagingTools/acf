#ifndef icomp_TAttribute_included
#define icomp_TAttribute_included


#include "istd/CClassInfo.h"
#include "iser/IArchive.h"
#include "iser/ISerializable.h"
#include "iser/IObject.h"
#include "iser/CArchiveTag.h"

#include "icomp/icomp.h"


namespace icomp
{


/**
	Template implementation of single component attribute.
	\internal
	Don't use direct this class, use macros \c I_ATTR and \c I_ASSIGN instead.
*/
template <typename Value>
class TAttribute: virtual public iser::IObject
{
public:
	typedef Value ValueType;

	TAttribute();
	TAttribute(const TAttribute& attribute);

	explicit TAttribute(const Value& value);

	virtual const Value& GetValue() const;
	virtual void SetValue(const Value& value);

	// reimplemented (iser::IObject)
	virtual const std::string& GetFactoryId() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	Value m_value;

	static const istd::CClassInfo s_classInfo;
};


// public methods

template <typename Value>
TAttribute<Value>::TAttribute()
{
}


template <typename Value>
TAttribute<Value>::TAttribute(const Value& value)
:	m_value(value)
{
}


template <typename Value>
TAttribute<Value>::TAttribute(const TAttribute& attribute)
:	m_value(attribute.GetValue())
{
}


template <typename Value>
const Value& TAttribute<Value>::GetValue() const
{
	return m_value;
}


template <typename Value>
void TAttribute<Value>::SetValue(const Value& value)
{
	m_value = value;
}


// reimplemented (iser::IObject)

template <typename Value>
const std::string& TAttribute<Value>::GetFactoryId() const
{
	return s_classInfo.GetName();
}


// reimplemented (ISerializable)

template <typename Value>
bool TAttribute<Value>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag valueTag("Value", "Value of attribute");
	retVal = retVal && archive.BeginTag(valueTag);

	retVal = retVal && archive.Process(m_value);

	retVal = retVal && archive.EndTag(valueTag);

	return retVal;
}


// static attributes

template <typename Value>
const istd::CClassInfo TAttribute<Value>::s_classInfo(typeid(TAttribute<Value>));


// typedefs

typedef TAttribute<double> CDoubleAttribute;
typedef TAttribute<bool> CBoolAttribute;
typedef TAttribute<int> CIntAttribute;
typedef TAttribute<istd::CString> CStringAttribute;


} // namespace icomp


#endif // !icomp_TAttribute_included


