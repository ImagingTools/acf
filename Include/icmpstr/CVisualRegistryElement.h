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
	const QString& GetNote() const;
	void SetNote(const QString& note);

	// overloaded (icomp::CRegistryElement)
	void Initialize(
				const icomp::IRegistry* parentPtr,
				const icomp::CComponentAddress& address);

	// reimplemented (i2d::IObject2d)
	virtual i2d::CVector2d GetCenter() const;
	virtual void MoveCenterTo(const i2d::CVector2d& position);
	virtual bool Transform(
				const i2d::ITransformation2d& transformation,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);
	virtual bool InvTransform(
				const i2d::ITransformation2d& transformation,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);
	virtual bool GetTransformed(
				const i2d::ITransformation2d& transformation,
				i2d::IObject2d& result,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;
	virtual bool GetInvTransformed(
				const i2d::ITransformation2d& transformation,
				i2d::IObject2d& result,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;

private:
	i2d::CVector2d m_center;
	const icomp::IRegistry* m_registryPtr;
	icomp::CComponentAddress m_addess;
	std::string m_name;
	QString m_note;
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


} // namespace icmpstr


#endif //!icmpstr_CVisualRegistryElement_included


