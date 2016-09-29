#ifndef icomp_CComponentAddress_included
#define icomp_CComponentAddress_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <iser/ISerializable.h>


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
	CComponentAddress(const QByteArray& packageId, const QByteArray& componentId);

	/**
		Check if this address is valid.
	*/
	bool IsValid() const;

	const QByteArray& GetPackageId() const;
	void SetPackageId(const QByteArray& id);

	const QByteArray& GetComponentId() const;
	void SetComponentId(const QByteArray& id);

	QString ToString() const;
	bool FromString(const QString& address);

	bool operator==(const CComponentAddress& address) const;
	bool operator!=(const CComponentAddress& address) const;
	bool operator<(const CComponentAddress& address) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	friend uint qHash(const CComponentAddress& address, uint seed);

private:
	QByteArray m_packageId;
	QByteArray m_componentId;
};


// inline methods

inline const QByteArray& CComponentAddress::GetPackageId() const
{
	return m_packageId;
}


inline void CComponentAddress::SetPackageId(const QByteArray& id)
{
	m_packageId = id;
}


inline const QByteArray& CComponentAddress::GetComponentId() const
{
	return m_componentId;
}


inline void CComponentAddress::SetComponentId(const QByteArray& id)
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


// related global functions

inline uint qHash(const CComponentAddress& address, uint seed = 0)
{
	Q_UNUSED(seed);

	return qHash(address.m_packageId) + qHash(address.m_packageId);
}


} // namespace icomp


#endif // !icomp_CComponentAddress_included


