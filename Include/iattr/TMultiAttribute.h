#ifndef iattr_TMultiAttribute_included
#define iattr_TMultiAttribute_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/CClassInfo.h>
#include <iser/IArchive.h>
#include <iser/IObject.h>
#include <iser/CArchiveTag.h>
#include <iser/TCopySerializedWrap.h>
#include <iattr/IAttributeMetaInfo.h>


namespace iattr
{


/**
	Template implementation of multiple component attribute.
	\internal
	Don't use direct this class, use macros I_MULTIATTR and I_ASSIGN instead.
*/
template <typename Value>
class TMultiAttribute: public iser::TCopySerializedWrap<iser::IObject>
{
public:
	typedef Value ValueType;
	typedef iser::TCopySerializedWrap<iser::IObject> BaseClass;

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = IAttributeMetaInfo::AF_OBLIGATORY | IAttributeMetaInfo::AF_NULLABLE | IAttributeMetaInfo::AF_MULTIPLE | IAttributeMetaInfo::AF_VALUE,
		DAF_OPTIONAL = IAttributeMetaInfo::AF_NULLABLE | IAttributeMetaInfo::AF_MULTIPLE | IAttributeMetaInfo::AF_VALUE
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
	virtual int FindValue(const Value& value) const;

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// static methods
	static QByteArray GetTypeName();

protected:
	struct Wrap
	{
		Value value;

		bool operator == (const Wrap& object) const
		{
			return (object.value == value);
		}
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
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < GetValuesCount());

	return m_values[index].value;
}


template <typename Value>
void TMultiAttribute<Value>::SetValueAt(int index, const Value& value)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < GetValuesCount());

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


template <typename Value>
int TMultiAttribute<Value>::FindValue(const Value& value) const
{
	Wrap valueWrap;
	valueWrap.value = value;

	return m_values.indexOf(valueWrap);
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
	static iser::CArchiveTag valuesTag("Values", "List of attribute values", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag valueTag("Value", "Single Value", iser::CArchiveTag::TT_LEAF, &valuesTag);

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
		return "iattr::TMultiAttribute<QByteArray>";
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


} // namespace iattr


#endif // !iattr_TMultiAttribute_included


