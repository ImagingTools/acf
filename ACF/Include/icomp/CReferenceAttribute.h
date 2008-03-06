#ifndef icomp_CReferenceAttribute_included
#define icomp_CReferenceAttribute_included


#include <string>

#include "iser/ISerializable.h"

#include "icomp/icomp.h"


namespace icomp{


class CReferenceAttribute: public iser::ISerializable
{
public:
	CReferenceAttribute(const ::std::string& componentId = "");

	const ::std::string& GetComponentId() const;
	void SetComponentId(const ::std::string& componentId);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	::std::string m_componentId;
};


// inline methods

inline const ::std::string& CReferenceAttribute::GetComponentId() const
{
	return m_componentId;
}


inline void CReferenceAttribute::SetComponentId(const ::std::string& componentId)
{
	m_componentId = componentId;
}


} // namespace icomp


#endif //!icomp_CReferenceAttribute_included


