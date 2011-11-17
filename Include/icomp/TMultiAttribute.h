#ifndef icomp_TMultiAttribute_included
#define icomp_TMultiAttribute_included


// STL includes
#include <vector>


// ACF includes
#include "istd/CClassInfo.h"

#include "iser/IArchive.h"
#include "iser/IObject.h"
#include "iser/CArchiveTag.h"
#include "iser/TCopySerializedWrap.h"

#include "icomp/IAttributeStaticInfo.h"


namespace icomp
{


/**
	Template implementation of multiple component attribute.
	\internal
	Don't use direct this class, use macros I_MULTIATTR and I_ASSIGN instead.
*/
template <typename Value>
class TMultiAttribute: virtual public iser::TCopySerializedWrap<iser::IObject>
{
public:
	typedef Value ValueType;

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = IAttributeStaticInfo::AF_OBLIGATORY | IAttributeStaticInfo::AF_NULLABLE,
		DAF_OPTIONAL = IAttributeStaticInfo::AF_NULLABLE
	};

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
	virtual void Reset();

	// reimplemented (iser::IObject)
	std::string GetFactoryId() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// static methods
	static std::string GetTypeName();

protected:
	struct Wrap
	{
		Value value;
	};
	std::vector<Wrap> m_values;

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
		Wrap wrap;
		wrap.value = valuesPtr[i];
		m_values.push_back(wrap);
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

	return m_values[index].value;
}


template <typename Value>
void TMultiAttribute<Value>::SetValueAt(int index, const Value& value)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetValuesCount());

	m_values[index].value = value;
}


template <typename Value>
void TMultiAttribute<Value>::InsertValue(const Value& value)
{
	Wrap wrap;
	wrap.value = value;
	m_values.push_back(wrap);
}


template <typename Value>
void TMultiAttribute<Value>::Reset()
{
	m_values.clear();
}


// reimplemented (iser::IObject)

template <typename Value>
std::string TMultiAttribute<Value>::GetFactoryId() const
{
	return GetTypeName();
}


// reimplemented (iser::ISerializable)

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
		retVal = retVal && archive.Process(m_values[i].value);
		retVal = retVal && archive.EndTag(valueTag);
	}

	retVal = retVal && archive.EndTag(valuesTag);

	return retVal;
}


// static methods

template <typename Value>
std::string TMultiAttribute<Value>::GetTypeName()
{
	if (typeid(Value) == typeid(std::string)){
		return "icomp::TMultiAttribute<std::string>";
	}

	return istd::CClassInfo::GetName<TMultiAttribute<Value> >();
}


// typedefs

typedef TMultiAttribute<double> CMultiDoubleAttribute;
typedef TMultiAttribute<bool> CMultiBoolAttribute;
typedef TMultiAttribute<int> CMultiIntAttribute;
typedef TMultiAttribute<std::string> CMultiStdStringAttribute;
typedef TMultiAttribute<istd::CString> CMultiStringAttribute;


} // namespace icomp


#endif // !icomp_TMultiAttribute_included


