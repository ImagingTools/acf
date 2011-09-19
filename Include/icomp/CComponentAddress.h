#ifndef icomp_CComponentAddress_included
#define icomp_CComponentAddress_included


// STL includes
#include <string>

#include "iser/ISerializable.h"


namespace icomp
{


/**
	Represents global address of component.
	The component address contains of global package ID and component ID inside this package.

	\ingroup ComponentConcept
*/
class CComponentAddress: public iser::ISerializable
{
public:
	CComponentAddress();
	CComponentAddress(const std::string& packageId, const std::string& componentId);

	/**
		Check if this address is valid.
	*/
	bool IsValid() const;

	const std::string& GetPackageId() const;
	void SetPackageId(const std::string& id);

	const std::string& GetComponentId() const;
	void SetComponentId(const std::string& id);

	istd::CString ToString() const;

	bool operator==(const CComponentAddress& address) const;
	bool operator!=(const CComponentAddress& address) const;
	bool operator<(const CComponentAddress& address) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	std::string m_packageId;
	std::string m_componentId;
};


// inline methods

inline const std::string& CComponentAddress::GetPackageId() const
{
	return m_packageId;
}


inline void CComponentAddress::SetPackageId(const std::string& id)
{
	m_packageId = id;
}


inline const std::string& CComponentAddress::GetComponentId() const
{
	return m_componentId;
}


inline void CComponentAddress::SetComponentId(const std::string& id)
{
	m_componentId = id;
}


inline bool CComponentAddress::operator==(const CComponentAddress& address) const
{
	return ((m_packageId == address.m_packageId) && (m_componentId == address.m_componentId));
}


inline bool CComponentAddress::operator!=(const CComponentAddress& address) const
{
	return ((m_packageId != address.m_packageId) || (m_componentId != address.m_componentId));
}


inline bool CComponentAddress::operator<(const CComponentAddress& address) const
{
	if (m_packageId == address.m_packageId){
		return m_componentId < address.m_componentId;
	}

	return m_packageId < address.m_packageId;
}


}//namespace icomp


#endif // !icomp_CComponentAddress_included


