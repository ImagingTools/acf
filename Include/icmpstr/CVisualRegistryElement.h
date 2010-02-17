#ifndef icmpstr_CVisualRegistryElement_included
#define icmpstr_CVisualRegistryElement_included


// Qt includes
#include <QIcon>


// ACF includes
#include "i2d/IObject2d.h"

#include "icomp/IRegistry.h"
#include "icomp/CComponentAddress.h"
#include "icomp/CRegistryElement.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


class CVisualRegistryElement: public icomp::CRegistryElement, virtual public i2d::IObject2d
{
public:
	typedef icomp::CRegistryElement BaseClass;

	CVisualRegistryElement();

	const icomp::IRegistry* GetRegistry() const;
	const icomp::CComponentAddress& GetAddress() const;
	const std::string& GetName() const;
	void SetName(const std::string& name);
	const QIcon& GetIcon() const;
	void SetIcon(const QIcon& icon);

	// overloaded (icomp::CRegistryElement)
	void Initialize(
				const icomp::IRegistry* parentPtr,
				const icomp::CComponentAddress& address);

	// reimplemented (IObject2d)
	virtual i2d::CVector2d GetCenter() const;
	virtual void MoveTo(const i2d::CVector2d& position);

private:
	const icomp::IRegistry* m_registryPtr;
	icomp::CComponentAddress m_addess;
	i2d::CVector2d m_center;
	std::string m_name;
	QIcon m_icon;
};


// inline methods

inline const icomp::IRegistry* CVisualRegistryElement::GetRegistry() const
{
	return m_registryPtr;
}


inline const icomp::CComponentAddress& CVisualRegistryElement::GetAddress() const
{
	return m_addess;
}


inline const std::string& CVisualRegistryElement::GetName() const
{
	return m_name;
}


inline const QIcon& CVisualRegistryElement::GetIcon() const
{
	return m_icon;
}


} // namespace icmpstr


#endif //!icmpstr_CVisualRegistryElement_included


