#ifndef icomp_TAttributeStaticInfo_included
#define icomp_TAttributeStaticInfo_included


#include "istd/CClassInfo.h"

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
				const istd::CClassInfo& relatedInterfaceInfo);

	// reimplemented (icomp::IRealAttributeStaticInfo)
	virtual const std::string& GetAttributeId() const;

	// reimplemented (icomp::IAttributeStaticInfo)
	virtual iser::IObject* CreateAttribute() const;
	virtual const std::string& GetAttributeDescription() const;
	virtual const iser::IObject* GetAttributeDefaultValue() const;
	virtual const istd::CClassInfo& GetAttributeType() const;
	virtual const istd::CClassInfo& GetRelatedInterfaceType() const;
	virtual bool IsObligatory() const;

private:
	std::string m_id;
	std::string m_description;
	const Attribute* m_defaultValuePtr;
	bool m_isObligatory;
	istd::CClassInfo m_relatedInterfaceType;
};


// public methods

template <class Attribute>
TAttributeStaticInfo<Attribute>::TAttributeStaticInfo(
			icomp::IComponentStaticInfo& staticInfo,
			const std::string& id,
			const std::string& description,
			const Attribute* defaultValuePtr,
			bool isObligatory,
			const istd::CClassInfo& relatedInterfaceInfo)
:	m_id(id),
	m_description(description),
	m_defaultValuePtr(defaultValuePtr),
	m_isObligatory(isObligatory),
	m_relatedInterfaceType(relatedInterfaceInfo)
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
iser::IObject* TAttributeStaticInfo<Attribute>::CreateAttribute() const
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
const iser::IObject* TAttributeStaticInfo<Attribute>::GetAttributeDefaultValue() const
{
	return m_defaultValuePtr;
}


template <class Attribute>
const istd::CClassInfo& TAttributeStaticInfo<Attribute>::GetAttributeType() const
{
	static istd::CClassInfo info = istd::CClassInfo::GetInfo<Attribute>();

	return info;
}


template <class Attribute>
const istd::CClassInfo& TAttributeStaticInfo<Attribute>::GetRelatedInterfaceType() const
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


