#ifndef icomp_TSimpleAttribute_included
#define icomp_TSimpleAttribute_included


#include "istd/CString.h"

#include "iser/IArchive.h"
#include "iser/ISerializable.h"
#include "iser/CArchiveTag.h"

#include "icomp/icomp.h"


namespace icomp
{


/**
	Template implementation of single component attribute.
*/
template <typename ValueType> 
class TSingleAttribute: virtual public iser::ISerializable
{
public:
	TSingleAttribute();
	explicit TSingleAttribute(const ValueType& value);
	TSingleAttribute(const TSingleAttribute& attribute);

	virtual const ValueType& GetValue() const;
	virtual void SetValue(const ValueType& value);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	ValueType m_value;
};


// public methods

template <typename ValueType> 
TSingleAttribute<ValueType>::TSingleAttribute()
{
}


template <typename ValueType> 
TSingleAttribute<ValueType>::TSingleAttribute(const ValueType& value)
:	m_value(value)
{
}


template <typename ValueType> 
TSingleAttribute<ValueType>::TSingleAttribute(const TSingleAttribute& attribute)
:	m_value(attribute.GetValue())
{
}


template <typename ValueType> 
const ValueType& TSingleAttribute<ValueType>::GetValue() const
{
	return m_value;
}


template <typename ValueType> 
void TSingleAttribute<ValueType>::SetValue(const ValueType& value)
{
	m_value = value;
}


// reimplemented (ISerializable)

template <typename ValueType> 
bool TSingleAttribute<ValueType>::Serialize(iser::IArchive& archive)
{
	bool result = true;

	static iser::CArchiveTag valueTag("Value", "Value of attribute");
	result = result && archive.BeginTag(valueTag);

	result = result && archive.Process(m_value);

	result = result && archive.EndTag(valueTag);

	return result;
}


} // namespace icomp


#endif // icomp_TSimpleAttribute_included


