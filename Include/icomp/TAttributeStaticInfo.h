#ifndef icomp_TAttributeStaticInfo_included
#define icomp_TAttributeStaticInfo_included


#include "icomp/IRealAttributeStaticInfo.h"


namespace icomp
{


template <class Attribute>
class TAttributeStaticInfo: virtual public IRealAttributeStaticInfo
{
public:
	TAttributeStaticInfo(
				icomp::IComponentStaticInfo& staticInfo,
				const std::string& id,
				const std::string& description,
				const Attribute* defaultValuePtr,
				bool isObligatory,
				const std::type_info* relatedInterfaceTypePtr);

	// reimplemented (icomp::IRealAttributeStaticInfo)
	virtual const std::string& GetAttributeId() const;

	// reimplemented (icomp::IAttributeStaticInfo)
	virtual iser::ISerializable* CreateAttribute() const;
	virtual const std::string& GetAttributeDescription() const;
	virtual const iser::ISerializable* GetAttributeDefaultValue() const;
	virtual const std::type_info& GetAttributeType() const;
	virtual const std::type_info& GetRelatedInterfaceType() const;
	virtual bool IsObligatory() const;

private:
	std::string m_id;
	std::string m_description;
	const Attribute* m_defaultValuePtr;
	const std::type_info& m_relatedInterfaceType;
	bool m_isObligatory;
};


// public methods

template <class Attribute>
TAttributeStaticInfo<Attribute>::TAttributeStaticInfo(
			icomp::IComponentStaticInfo& staticInfo,
			const std::string& id,
			const std::string& description,
			const Attribute* defaultValuePtr,
			bool isObligatory,
			const std::type_info* relatedInterfaceTypePtr)
:	m_id(id),
	m_description(description),
	m_defaultValuePtr(defaultValuePtr),
	m_isObligatory(isObligatory),
	m_relatedInterfaceType(*relatedInterfaceTypePtr)
{
	staticInfo.RegisterAttributeInfo(id, this);
}


// reimplemented (icomp::IRealAttributeStaticInfo)

template <class Attribute>
const std::string& TAttributeStaticInfo<Attribute>::GetAttributeId() const
{
	return m_id;
}


// reimplemented (icomp::IAttributeStaticInfo)

template <class Attribute>
iser::ISerializable* TAttributeStaticInfo<Attribute>::CreateAttribute() const
{
	if (m_defaultValuePtr != NULL){
		return new Attribute(*m_defaultValuePtr);
	}
	else{
		return new Attribute();
	}
}

template <class Attribute>
const std::string& TAttributeStaticInfo<Attribute>::GetAttributeDescription() const
{
	return m_description;
}


template <class Attribute>
const iser::ISerializable* TAttributeStaticInfo<Attribute>::GetAttributeDefaultValue() const
{
	return m_defaultValuePtr;
}


template <class Attribute>
const std::type_info& TAttributeStaticInfo<Attribute>::GetAttributeType() const
{
	return typeid(Attribute);
}


template <class Attribute>
const std::type_info& TAttributeStaticInfo<Attribute>::GetRelatedInterfaceType() const
{
	return m_relatedInterfaceType;
}


template <class Attribute>
bool TAttributeStaticInfo<Attribute>::IsObligatory() const
{
	return m_isObligatory;
}


}//namespace icomp


#endif // !icomp_TAttributeStaticInfo_included


