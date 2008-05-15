#ifndef icomp_TMultiAttribute_included
#define icomp_TMultiAttribute_included


// STL includes
#include <vector>

#include "iser/IArchive.h"
#include "iser/ISerializable.h"
#include "iser/CArchiveTag.h"

#include "icomp/icomp.h"


namespace icomp
{


/**
	Template implementation of multiple component attribute.
	\internal
	Don't use direct this class, use macros I_MULTIATTR and I_ASSIGN instead.
*/
template <typename Value>
class TMultiAttribute: virtual public iser::ISerializable
{
public:
	typedef Value ValueType;

	TMultiAttribute();
	TMultiAttribute(const TMultiAttribute& attribute);

	/**
		Constructor.
		\param	elementsCount	number of elements stored at \c valuesPtr.
		\param	valuesPtr		pointer to array of elements.
	*/
	TMultiAttribute(int elementsCount, Value* valuesPtr);

	virtual int GetValuesCount() const;
	virtual const Value& GetValueAt(int index) const;
	virtual void SetValueAt(int index, const Value& value);
	virtual void InsertValue(const Value& value);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	std::vector<Value> m_values;
};


// public methods

template <typename Value>
TMultiAttribute<Value>::TMultiAttribute()
{
}


template <typename Value>
TMultiAttribute<Value>::TMultiAttribute(const TMultiAttribute& attribute)
:	m_values(attribute.m_values)
{
}


template <typename Value>
TMultiAttribute<Value>::TMultiAttribute(int elementsCount, Value* valuesPtr)
{
	for (int i = 0; i < elementsCount; ++i){
		m_values.push_back(valuesPtr[i]);
	}
}


template <typename Value>
int TMultiAttribute<Value>::GetValuesCount() const
{
	return int(m_values.size());
}


template <typename Value>
const Value& TMultiAttribute<Value>::GetValueAt(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetValuesCount());

	return m_values[index];
}


template <typename Value>
void TMultiAttribute<Value>::SetValueAt(int index, const Value& value)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetValuesCount());

	m_values[index] = value;
}


template <typename Value>
void TMultiAttribute<Value>::InsertValue(const Value& value)
{
	m_values.push_back(value);
}


// reimplemented (ISerializable)

template <typename Value>
bool TMultiAttribute<Value>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	bool isStoring = archive.IsStoring();

	static iser::CArchiveTag valuesTag("Values", "List of attribute values");
	static iser::CArchiveTag valueTag("Value", "Single Value");

	int valuesCount = 0;

	if (isStoring){
		valuesCount = int(m_values.size());
	}

	retVal = retVal && archive.BeginMultiTag(valuesTag, valueTag, valuesCount);

	if (!isStoring){
		if (!retVal){
			return false;
		}

		m_values.resize(valuesCount);
	}

	for (int i = 0; i < valuesCount; ++i){
		retVal = retVal && archive.BeginTag(valueTag);
		retVal = retVal && archive.Process(m_values[i]);
		retVal = retVal && archive.EndTag(valueTag);
	}

	retVal = retVal && archive.EndTag(valuesTag);

	return retVal;
}


typedef TMultiAttribute<double> CMultiDoubleAttribute;
typedef TMultiAttribute<bool> CMultiBoolAttribute;
typedef TMultiAttribute<int> CMultiIntAttribute;
typedef TMultiAttribute<istd::CString> CMultiStringAttribute;


} // namespace icomp


#endif // !icomp_TMultiAttribute_included


