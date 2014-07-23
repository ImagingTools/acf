#ifndef icomp_TAttribute_included
#define icomp_TAttribute_included


// ACF includes
#include "istd/CChangeNotifier.h"
#include "istd/CClassInfo.h"
#include "iser/IArchive.h"
#include "iser/IObject.h"
#include "iser/CArchiveTag.h"
#include "iser/TCopySerializedWrap.h"
#include "icomp/IAttributeStaticInfo.h"


namespace icomp
{


/**
	Template implementation of single component attribute.
	\internal
	Don't use direct this class, use macros \c I_ATTR and \c I_ASSIGN instead.
*/
template <typename Value>
class TAttribute: virtual public iser::TCopySerializedWrap<iser::IObject>
{
public:
	typedef Value ValueType;
	typedef iser::TCopySerializedWrap<iser::IObject> BaseClass;

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = IAttributeStaticInfo::AF_OBLIGATORY | IAttributeStaticInfo::AF_NULLABLE | IAttributeStaticInfo::AF_SINGLE | IAttributeStaticInfo::AF_VALUE,
		DAF_OPTIONAL = IAttributeStaticInfo::AF_NULLABLE | IAttributeStaticInfo::AF_SINGLE | IAttributeStaticInfo::AF_VALUE
	};

	TAttribute();
	TAttribute(const TAttribute& attribute);

	explicit TAttribute(const Value& value);

	virtual const Value& GetValue() const;
	virtual void SetValue(const Value& value);

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// static methods
	static QByteArray GetTypeName();

protected:
	Value m_value;
};


// public methods

template <typename Value>
TAttribute<Value>::TAttribute()
	:BaseClass()
{
}


template <typename Value>
TAttribute<Value>::TAttribute(const Value& value)
	:BaseClass(),
	m_value(value)
{
}


template <typename Value>
TAttribute<Value>::TAttribute(const TAttribute& attribute)
	:BaseClass(),
	m_value(attribute.GetValue())
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
QByteArray TAttribute<Value>::GetFactoryId() const
{
	return GetTypeName();
}


// reimplemented (iser::ISerializable)

template <typename Value>
bool TAttribute<Value>::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag valueTag("Value", "Value of attribute", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(valueTag);
	retVal = retVal && archive.Process(m_value);
	retVal = retVal && archive.EndTag(valueTag);

	return retVal;
}


// static methods

template <typename Value>
QByteArray TAttribute<Value>::GetTypeName()
{
	return istd::CClassInfo::GetName<TAttribute<Value> >();
}


template <>
inline QByteArray TAttribute<int>::GetTypeName()
{
	return "Integer";
}


template <>
inline QByteArray TAttribute<double>::GetTypeName()
{
	return "Real";
}


template <>
inline QByteArray TAttribute<bool>::GetTypeName()
{
	return "Boolean";
}


template <>
inline QByteArray TAttribute<QString>::GetTypeName()
{
	return "String";
}


template <>
inline QByteArray TAttribute<QByteArray>::GetTypeName()
{
	return "Id";
}


// typedefs

typedef TAttribute<int> CIntegerAttribute;
typedef TAttribute<double> CRealAttribute;
typedef TAttribute<bool> CBooleanAttribute;
typedef TAttribute<QString> CStringAttribute;
typedef TAttribute<QByteArray> CIdAttribute;


} // namespace icomp


#endif // !icomp_TAttribute_included


