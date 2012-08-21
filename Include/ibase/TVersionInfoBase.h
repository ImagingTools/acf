#ifndef ibase_TVersionInfoBase_included
#define ibase_TVersionInfoBase_included


// ACF includes
#include "iser/IVersionInfo.h"
#include "ibase/TLoggerCompWrap.h"
#include "istd/AcfVersion.h"


namespace ibase
{


template <class BaseComponent>
class TVersionInfoBase: public BaseComponent, virtual public iser::IVersionInfo
{
public:
	typedef BaseComponent BaseClass;

	I_BEGIN_BASE_COMPONENT(TVersionInfoBase);
		I_REGISTER_INTERFACE(iser::IVersionInfo);
		I_ASSIGN(m_slaveVersionInfoCompPtr, "SlaveVersionInfo", "Slave version info, unknown version information requests will be delegated here", false, "SlaveVersionInfo");
	I_END_COMPONENT;

	// reimplemented (iser::IVersionInfo)
	virtual bool GetVersionNumber(int versionId, quint32& result) const;
	virtual QString GetVersionIdDescription(int versionId) const;
	virtual VersionIds GetVersionIds() const;
	virtual QString GetEncodedVersionName(int versionId, quint32 versionNumber) const;

private:
	I_REF(iser::IVersionInfo, m_slaveVersionInfoCompPtr);
};


typedef ibase::TVersionInfoBase<icomp::CComponentBase> CVersionInfoCompBase;
typedef ibase::TVersionInfoBase<ibase::CLoggerComponentBase> CVersionInfoLoggerCompBase;


// public methods

// reimplemented (iser::IVersionInfo)

template <class BaseComponent>
bool TVersionInfoBase<BaseComponent>::GetVersionNumber(int versionId, quint32& result) const
{
	if (m_slaveVersionInfoCompPtr.IsValid()){
		return m_slaveVersionInfoCompPtr->GetVersionNumber(versionId, result);
	}
	else if (versionId == iser::IVersionInfo::AcfVersionId){
		result = istd::RS_USE_VERSION;

		return true;
	}

	result = 0xffffffff;

	return false;
}


template <class BaseComponent>
QString TVersionInfoBase<BaseComponent>::GetVersionIdDescription(int versionId) const
{
	if (m_slaveVersionInfoCompPtr.IsValid()){
		return m_slaveVersionInfoCompPtr->GetVersionIdDescription(versionId);
	}

	return "";
}


template <class BaseComponent>
iser::IVersionInfo::VersionIds TVersionInfoBase<BaseComponent>::GetVersionIds() const
{
	VersionIds retVal;
	if (m_slaveVersionInfoCompPtr.IsValid()){
		retVal = m_slaveVersionInfoCompPtr->GetVersionIds();
	}

	return retVal;
}


template <class BaseComponent>
QString TVersionInfoBase<BaseComponent>::GetEncodedVersionName(int versionId, quint32 versionNumber) const
{
	QString retVal;

	if (m_slaveVersionInfoCompPtr.IsValid()){
		retVal = m_slaveVersionInfoCompPtr->GetEncodedVersionName(versionId, versionNumber);
	}
	else{
		retVal = QString("<") + QString("%1").arg(versionNumber) + ">";
	}

	return retVal;
}


} // namespace ibase


#endif // !ibase_TVersionInfoBase_included
