#pragma once


// ACF includes
#include <ifile/IFileMetaInfoProvider.h>
#include <icomp/CComponentBase.h>


namespace ifile
{


/**
	\ingroup Persistence
*/
class CComposedFileMetaInfoProviderComp:
			public icomp::CComponentBase,
			virtual public ifile::IFileMetaInfoProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CComposedFileMetaInfoProviderComp);
		I_REGISTER_INTERFACE(ifile::IFileMetaInfoProvider);
		I_ASSIGN_MULTI_0(m_slaveInfoProvidersCompPtr, "SlaveInfoProviders", "List of slave meta info providers", true);
	I_END_COMPONENT;

	// reimplemented (ifile::IFileMetaInfoProvider)
	virtual MetaInfoPtr GetFileMetaInfo(const QString& filePath) const;
	
private:
	I_MULTIREF(ifile::IFileMetaInfoProvider, m_slaveInfoProvidersCompPtr);
};


} // namespace ifile


