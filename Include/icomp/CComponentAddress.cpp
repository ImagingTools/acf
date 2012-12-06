#include "icomp/CComponentAddress.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CIdManipBase.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace icomp
{


CComponentAddress::CComponentAddress()
{
}


CComponentAddress::CComponentAddress(const QByteArray& packageId, const QByteArray& componentId)
:	m_packageId(packageId),
	m_componentId(componentId)
{
}


bool CComponentAddress::IsValid() const
{
	return !m_packageId.isEmpty() && !m_componentId.isEmpty();
}


QString CComponentAddress::ToString() const
{
	return m_packageId + "/" + m_componentId;
}


bool CComponentAddress::FromString(const QString& address)
{
	QByteArray packageId;
	QByteArray componentId;
	if (istd::CIdManipBase::SplitId(address.toUtf8(), packageId, componentId)){
		m_packageId = packageId;
		m_componentId = componentId;

		return true;
	}

	return false;
}


// reimplemented (iser::ISerializable)

bool CComponentAddress::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag packageIdTag("PackageId", "ID of package");
	static iser::CArchiveTag componentIdTag("ComponentId", "ID of factory");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

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


