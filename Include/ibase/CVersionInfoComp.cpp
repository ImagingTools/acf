#include <ibase/CVersionInfoComp.h>


#include <istd/AcfVersion.h>


namespace ibase
{


// public methods

// reimplemented (iser::IVersionInfo)

bool CVersionInfoComp::GetVersionNumber(int versionId, quint32& result) const
{
	if (m_versionIdAttrPtr.IsValid() && (versionId == *m_versionIdAttrPtr)){
		result = *m_versionNumberAttrPtr;

		return true;
	}
	else if (m_slaveVersionInfoCompPtr.IsValid()){
		return m_slaveVersionInfoCompPtr->GetVersionNumber(versionId, result);
	}
	else if (versionId == iser::IVersionInfo::AcfVersionId){
		result = istd::RS_USE_VERSION;

		return true;
	}

	result = 0xffffffff;

	return false;
}


QString CVersionInfoComp::GetVersionIdDescription(int versionId) const
{
	if (m_versionIdAttrPtr.IsValid() && (versionId == *m_versionIdAttrPtr)){
		return *m_versionIdDescriptionAttrPtr;
	}
	else if (m_slaveVersionInfoCompPtr.IsValid()){
		return m_slaveVersionInfoCompPtr->GetVersionIdDescription(versionId);
	}
	else{
		return "";
	}
}


iser::IVersionInfo::VersionIds CVersionInfoComp::GetVersionIds() const
{
	VersionIds retVal;
	if (m_slaveVersionInfoCompPtr.IsValid()){
		retVal = m_slaveVersionInfoCompPtr->GetVersionIds();
	}

	if (m_versionIdAttrPtr.IsValid()){
		retVal.insert(*m_versionIdAttrPtr);
	}

	return retVal;
}


QString CVersionInfoComp::GetEncodedVersionName(int versionId, quint32 versionNumber) const
{
	QString retVal;

	if (m_versionIdAttrPtr.IsValid() && (versionId == *m_versionIdAttrPtr)){
		quint32 lastBellowNumber = 0;
		int knownVersionsCount = qMin(m_knownVersionsAttrPtr.GetCount(), m_knownVersionNamesAttrPtr.GetCount());
		for (int i = 0; i < knownVersionsCount; ++i){
			quint32 knownNumber = quint32(m_knownVersionsAttrPtr[i]);

			if ((knownNumber <= versionNumber) && (knownNumber >= lastBellowNumber)){
				lastBellowNumber = knownNumber;
				retVal = m_knownVersionNamesAttrPtr[i];
			}
		}

		if (m_isExtensionUsedAttrPtr.IsValid() && *m_isExtensionUsedAttrPtr){
			retVal += QString(".") + QString("%1").arg(int(versionNumber - lastBellowNumber));
		}
	}
	else if (m_slaveVersionInfoCompPtr.IsValid()){
		retVal = m_slaveVersionInfoCompPtr->GetEncodedVersionName(versionId, versionNumber);
	}
	else{
		retVal = QString("<") + QString("%1").arg(versionNumber) + ">";
	}

	return retVal;
}


} // namespace ibase


