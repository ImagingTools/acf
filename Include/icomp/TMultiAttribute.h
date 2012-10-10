#ifndef icomp_TMultiAttribute_included
#define icomp_TMultiAttribute_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/TChangeNotifier.h"
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
	typedef iser::TCopySerializedWrap<iser::IObject> BaseClass;

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = IAttributeStaticInfo::AF_OBLIGATORY | IAttributeStaticInfo::AF_NULLABLE | IAttributeStaticInfo::AF_MULTIPLE | IAttributeStaticInfo::AF_VALUE,
		DAF_OPTIONAL = IAttributeStaticInfo::AF_NULLABLE | IAttributeStaticInfo::AF_MULTIPLE | IAttributeStaticInfo::AF_VALUE
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
	QByteArray GetFactoryId() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// static methods
	static QByteArray GetTypeName();

protected:
	struct Wrap
	{
		Value value;
	};
	QVector<Wrap> m_values;

};


// public methods

template <typename Value>
TMultiAttribute<Value>::TMultiAttribute()
	:BaseClass()
{
}


template <typename Value>
TMultiAttribute<Value>::TMultiAttribute(const TMultiAttribute& attribute)
	:BaseClass(),
	m_values(attribute.m_values)
{
}


template <typename Value>
TMultiAttribute<Value>::TMultiAttribute(int elementsCount, Value* valuesPtr)
	:BaseClass()
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
QByteArray TMultiAttribute<Value>::GetFactoryId() const
{
	return GetTypeName();
}


// reimplemented (iser::ISerializable)

template <typename Value>
bool TMultiAttribute<Value>::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag valuesTag("Values", "List of attribute values");
	static iser::CArchiveTag valueTag("Value", "Single Value");

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	bool retVal = true;

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
QByteArray TMultiAttribute<Value>::GetTypeName()
{
	if (typeid(Value) == typeid(QByteArray)){
		return "icomp::TMultiAttribute<QByteArray>";
	}

	return istd::CClassInfo::GetName<TMultiAttribute<Value> >();
}


template <>
inline QByteArray TMultiAttribute<int>::GetTypeName()
{
	return "Integer[]";
}


template <>
inline QByteArray TMultiAttribute<double>::GetTypeName()
{
	return "Real[]";
}


template <>
inline QByteArray TMultiAttribute<bool>::GetTypeName()
{
	return "Boolean[]";
}


template <>
inline QByteArray TMultiAttribute<QString>::GetTypeName()
{
	return "String[]";
}


template <>
inline QByteArray TMultiAttribute<QByteArray>::GetTypeName()
{
	return "Id[]";
}


// typedefs

typedef TMultiAttribute<int> CIntegerListAttribute;
typedef TMultiAttribute<double> CRealListAttribute;
typedef TMultiAttribute<bool> CBooleanListAttribute;
typedef TMultiAttribute<QString> CStringListAttribute;
typedef TMultiAttribute<QByteArray> CIdListAttribute;


} // namespace icomp


#endif // !icomp_TMultiAttribute_included


