#include <iprm/CIdParam.h>


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <istd/TDelPtr.h>
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace iprm
{


// reimplemented (iprm::IIdParam)

QByteArray CIdParam::GetId() const
{
	return m_id;
}


void CIdParam::SetId(const QByteArray& id)
{
	if (m_id != id){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		m_id = id;
	}
}


// reimplemented (iser::ISerializable)

bool CIdParam::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag idTag("ID", "Object-ID", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);
	Q_UNUSED(notifier);

	bool retVal = archive.BeginTag(idTag);
	retVal = retVal && archive.Process(m_id);
	retVal = retVal && archive.EndTag(idTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

int CIdParam::GetSupportedOperations() const
{
	return SO_CLONE | SO_COPY | SO_COMPARE | SO_RESET;
}


bool CIdParam::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const IIdParam* sourcePtr = dynamic_cast<const IIdParam*>(&object);
	if (sourcePtr != NULL){
		SetId(sourcePtr->GetId());

		return true;
	}

	return false;
}


bool CIdParam::IsEqual(const IChangeable& object) const
{
	const IIdParam* sourcePtr = dynamic_cast<const IIdParam*>(&object);
	if (sourcePtr != NULL){
		return sourcePtr->GetId() == m_id;
	}

	return false;
}


istd::IChangeable* CIdParam::CloneMe(istd::IChangeable::CompatibilityMode mode) const
{
	istd::TDelPtr<CIdParam> clonePtr(new CIdParam);
	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


bool CIdParam::ResetData(CompatibilityMode /*mode*/)
{
	if (!m_id.isEmpty()){
		istd::CChangeNotifier changeNotifier(this);

		m_id.clear();
	}

	return true;
}


} // namespace iprm


