#include "icomp/CReferenceAttribute.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace icomp{


CReferenceAttribute::CReferenceAttribute(const std::string& componentId)
:	m_componentId(componentId)
{
}


// reimplemented (iser::ISerializable)

bool CReferenceAttribute::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag componentIdTag("ComponentId", "ID of component");
	bool retVal = true;

	retVal = retVal && archive.BeginTag(componentIdTag);
	retVal = retVal && archive.Process(m_componentId);
	retVal = retVal && archive.EndTag(componentIdTag);

	return retVal;
}


} // namespace icomp


