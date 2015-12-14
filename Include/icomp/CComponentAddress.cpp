#include "icomp/CComponentAddress.h"


// ACF includes
#include "istd/CChangeNotifier.h"
#include "istd/CIdManipBase.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace icomp
{


// static constants
static const iser::CArchiveTag s_packageIdTag("PackageId", "ID of package", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_componentIdTag("ComponentId", "ID of factory", iser::CArchiveTag::TT_LEAF);


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
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(s_packageIdTag);
	retVal = retVal && archive.Process(m_packageId);
	retVal = retVal && archive.EndTag(s_packageIdTag);

	retVal = retVal && archive.BeginTag(s_componentIdTag);
	retVal = retVal && archive.Process(m_componentId);
	retVal = retVal && archive.EndTag(s_componentIdTag);

	return retVal;
}


} // namespace icomp


