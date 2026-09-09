// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <idoc/CDocumentStateComparator.h>


// ACF includes
#include <iser/CMemoryReadArchive.h>
#include <istd/CChangeNotifier.h>


namespace idoc
{


CDocumentStateComparator::CDocumentStateComparator()
:	m_hasStoredDocumentState(false),
	m_versionInfoPtr(NULL),
	m_stateChangedFlag(DCF_UNKNOWN),
	m_isStateChangedFlagValid(false)
{
}


// reimplemented (idoc::IDocumentStateComparator)

bool CDocumentStateComparator::HasStoredDocumentState() const
{
	return m_hasStoredDocumentState;
}


bool CDocumentStateComparator::StoreDocumentState()
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	iser::CMemoryWriteArchive& storedStateArchive = GetStoredStateArchive();
	storedStateArchive.Reset();

	iser::ISerializable* serializablePtr = GetObservedObject();
	if ((serializablePtr != NULL) && serializablePtr->Serialize(storedStateArchive)){
		m_stateChangedFlag = DCF_EQUAL;
		m_hasStoredDocumentState = true;
	}
	else{
		m_stateChangedFlag = DCF_UNKNOWN;
		m_hasStoredDocumentState = false;
	}

	m_isStateChangedFlagValid = true;

	return m_hasStoredDocumentState;
}


bool CDocumentStateComparator::RestoreDocumentState()
{
	if (m_hasStoredDocumentState){
		iser::CMemoryReadArchive restoreArchive(GetStoredStateArchive());
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		iser::ISerializable* objectPtr = GetObservedObject();
		if (objectPtr != NULL && objectPtr->Serialize(restoreArchive)){
			m_stateChangedFlag = DCF_EQUAL;
			m_isStateChangedFlagValid = true;

			return true;
		}

		m_stateChangedFlag = DCF_UNKNOWN;
		m_isStateChangedFlagValid = true;
	}

	return false;
}


CDocumentStateComparator::DocumentChangeFlag CDocumentStateComparator::GetDocumentChangeFlag() const
{
	if (!m_isStateChangedFlagValid){
		m_stateChangedFlag = DCF_UNKNOWN;

		if (m_hasStoredDocumentState){
			iser::CMemoryWriteArchive compareArchive(m_versionInfoPtr);

			iser::ISerializable* serializablePtr = GetObservedObject();
			if ((serializablePtr != NULL) && const_cast<iser::ISerializable*>(serializablePtr)->Serialize(compareArchive)){
				m_stateChangedFlag = (compareArchive != GetStoredStateArchive())? DCF_DIFFERENT: DCF_EQUAL;
			}
		}

		m_isStateChangedFlagValid = true;
	}

	return m_stateChangedFlag;
}


// reimplemented (imod::IObserver)

bool CDocumentStateComparator::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	if (BaseClass::OnModelAttached(modelPtr, changeMask)){
		m_hasStoredDocumentState = false;
		m_storedStateArchivePtr.Reset();
		m_stateChangedFlag = DCF_UNKNOWN;
		m_isStateChangedFlagValid = false;

		return true;
	}

	return false;
}


bool CDocumentStateComparator::OnModelDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnModelDetached(modelPtr)){
		m_hasStoredDocumentState = false;
		m_storedStateArchivePtr.Reset();
		m_stateChangedFlag = DCF_UNKNOWN;
		m_isStateChangedFlagValid = false;

		return true;
	}

	return false;
}


// protected methods

void CDocumentStateComparator::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	m_isStateChangedFlagValid = false;
	BaseClass::AfterUpdate(modelPtr, changeSet);
}


iser::ISerializable* CDocumentStateComparator::CastFromModel(imod::IModel* modelPtr) const
{
	return dynamic_cast<iser::ISerializable*>(modelPtr);
}


void CDocumentStateComparator::SetVersionInfo(const iser::IVersionInfo* versionInfoPtr)
{
	if (m_versionInfoPtr == versionInfoPtr){
		return;
	}

	m_versionInfoPtr = versionInfoPtr;

	m_storedStateArchivePtr.Reset();
	m_hasStoredDocumentState = false;
	m_stateChangedFlag = DCF_UNKNOWN;
	m_isStateChangedFlagValid = false;
}


const iser::IVersionInfo* CDocumentStateComparator::GetVersionInfo() const
{
	return m_versionInfoPtr;
}


// private methods

iser::CMemoryWriteArchive& CDocumentStateComparator::GetStoredStateArchive() const
{
	if (!m_storedStateArchivePtr.IsValid()){
		m_storedStateArchivePtr.SetPtr(new iser::CMemoryWriteArchive(m_versionInfoPtr));
	}

	return *m_storedStateArchivePtr;
}


} // namespace idoc
