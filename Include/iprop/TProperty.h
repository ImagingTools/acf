#ifndef iprop_TProperty_included
#define iprop_TProperty_included


#include "istd/TChangeNotifier.h"
#include "istd/CClassInfo.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "iser/TCopySerializedWrap.h"

#include "iprop/CPropertyBase.h"


namespace iprop
{


/**
	Template implementation of an atomic attribute.
*/
template <typename Value>
class TProperty: public iprop::CPropertyBase
{
public:
	typedef Value ValueType;
	typedef TProperty<Value> AttributeType;
	typedef iprop::CPropertyBase BaseClass;

	TProperty();

	TProperty(
				iprop::IPropertiesManager* attributeOwnerPtr,
				const std::string& attributeName,
				const std::string& attributeDescription,
				int attributeFlags,
				int changeFlags = 0,
				const ValueType& defaultValue = ValueType());

	const Value& GetValue() const;
	void SetValue(const Value& value);
	void ResetValue();

	// reimplemented (iser::IAttribute)
	virtual const iser::IObject* GetDefaultAttributePtr() const;

	// reimplemented (iser::IObject)
	virtual const std::string& GetFactoryId() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// static methods
	static const std::string& GetTypeName();

protected:
	Value m_value;

	mutable istd::TDelPtr<AttributeType> m_defaultAttributeValuePtr;

private:
	static const std::string s_typeName;
};


// public methods

template <typename Value>
TProperty<Value>::TProperty()
	:BaseClass()
{
}


template <typename Value>
TProperty<Value>::TProperty(
				iprop::IPropertiesManager* attributeOwnerPtr,
				const std::string& attributeName,
				const std::string& attributeDescription,
				int attributeFlags,
				int changeFlags,
				const ValueType& defaultValue)
	:BaseClass(attributeOwnerPtr, attributeName, attributeDescription, attributeFlags, changeFlags),
	m_value(defaultValue)
{
	if (!m_defaultAttributeValuePtr.IsValid()){
		m_defaultAttributeValuePtr.SetPtr(new AttributeType());

		m_defaultAttributeValuePtr->SetValue(defaultValue);
	}
}


template <typename Value>
const Value& TProperty<Value>::GetValue() const
{
	return m_value;
}


template <typename Value>
void TProperty<Value>::SetValue(const Value& value)
{
	if (m_value != value){
		istd::CChangeNotifier changePtr(m_attributeOwnerPtr, m_changeFlags);

		m_value = value;
	}
}


template <typename Value>
void  TProperty<Value>::ResetValue()
{
	if (m_defaultAttributeValuePtr.IsValid()){
		SetValue(m_defaultAttributeValuePtr->GetValue());
	}
}


// reimplemented (iser::IAttribute)

template <typename Value>
const iser::IObject* TProperty<Value>::GetDefaultAttributePtr() const
{
	return m_defaultAttributeValuePtr.GetPtr();
}


// reimplemented (iser::IObject)

template <typename Value>
const std::string& TProperty<Value>::GetFactoryId() const
{
	return s_typeName;
}


// reimplemented (ISerializable)

template <typename Value>
bool TProperty<Value>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag valueTag("Value", "Attribute value");
	retVal = retVal && archive.BeginTag(valueTag);

	retVal = retVal && archive.Process(m_value);

	retVal = retVal && archive.EndTag(valueTag);

	return retVal;
}


// static methods

template <typename Value>
const std::string& TProperty<Value>::GetTypeName()
{
	return s_typeName;
}


// private static attributes

template <typename Value>
const std::string TProperty<Value>::s_typeName(istd::CClassInfo::GetName<TProperty<Value> >());


// typedefs

typedef TProperty<double> CDoubleAttribute;
typedef TProperty<bool> CBoolAttribute;
typedef TProperty<int> CIntAttribute;
typedef TProperty<istd::CString> CStringAttribute;


} // namespace iprop


#endif // !iprop_TProperty_included


