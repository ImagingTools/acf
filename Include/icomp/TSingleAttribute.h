#ifndef icomp_TSingleAttribute_included
#define icomp_TSingleAttribute_included


#include "iser/IArchive.h"
#include "iser/ISerializable.h"
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
class TSingleAttribute: virtual public iser::ISerializable
{
public:
	typedef Value ValueType;

	TSingleAttribute();
	TSingleAttribute(const TSingleAttribute& attribute);

	explicit TSingleAttribute(const Value& value);

	virtual const Value& GetValue() const;
	virtual void SetValue(const Value& value);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	Value m_value;
};


// public methods

template <typename Value>
TSingleAttribute<Value>::TSingleAttribute()
{
}


template <typename Value>
TSingleAttribute<Value>::TSingleAttribute(const Value& value)
:	m_value(value)
{
}


template <typename Value>
TSingleAttribute<Value>::TSingleAttribute(const TSingleAttribute& attribute)
:	m_value(attribute.GetValue())
{
}


template <typename Value>
const Value& TSingleAttribute<Value>::GetValue() const
{
	return m_value;
}


template <typename Value>
void TSingleAttribute<Value>::SetValue(const Value& value)
{
	m_value = value;
}


// reimplemented (ISerializable)

template <typename Value>
bool TSingleAttribute<Value>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag valueTag("Value", "Value of attribute");
	retVal = retVal && archive.BeginTag(valueTag);

	retVal = retVal && archive.Process(m_value);

	retVal = retVal && archive.EndTag(valueTag);

	return retVal;
}


typedef TSingleAttribute<double> CDoubleAttribute;
typedef TSingleAttribute<bool> CBoolAttribute;
typedef TSingleAttribute<int> CIntAttribute;
typedef TSingleAttribute<istd::CString> CStringAttribute;


} // namespace icomp


#endif // !icomp_TSingleAttribute_included


