#include "icomp/CComponentAddress.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace icomp
{


CComponentAddress::CComponentAddress()
{
}


CComponentAddress::CComponentAddress(const std::string& packageId, const std::string& componentId)
:	m_packageId(packageId),
	m_componentId(componentId)
{
}


bool CComponentAddress::IsValid() const
{
	return !m_packageId.empty() && !m_componentId.empty();
}


QString CComponentAddress::ToString() const
{
	return QString(QString::fromStdString(m_packageId) + "/" + QString::fromStdString(m_componentId));
}


// reimplemented (iser::ISerializable)

bool CComponentAddress::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag packageIdTag("PackageId", "ID of package");
	static iser::CArchiveTag componentIdTag("ComponentId", "ID of factory");

	bool retVal = true;

	retVal = retVal && archive.BeginTag(packageIdTag);
	retVal = retVal && archive.Process(m_packageId);
	retVal = retVal && archive.EndTag(packageIdTag);

	retVal = retVal && archive.BeginTag(componentIdTag);
	retVal = retVal && archive.Process(m_componentId);
	retVal = retVal && archive.EndTag(componentIdTag);

	return retVal;
}


} // namespace icomp


