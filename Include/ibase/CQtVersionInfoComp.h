#ifndef ibase_CQtVersionInfo_included
#define ibase_CQtVersionInfo_included


// ACF includes
#include "ibase/TVersionInfoBase.h"


namespace ibase
{


/**
	Component representation of the Qt module version.
*/
class CQtVersionInfoComp: public ibase::CVersionInfoCompBase
{
public:
	typedef ibase::CVersionInfoCompBase BaseClass;

	I_BEGIN_COMPONENT(CQtVersionInfoComp);
	I_END_COMPONENT;

	enum QtVersionIds
	{
		/**
			Version of Qt framework against the actual module was compiled.
		*/
		QVI_COMPILED = IVersionInfo::UserVersionId + 1,

		/**
			Version of Qt framework registered in the current runtime system.
		*/
		QVI_RUNTIME
	};

	// reimplemented (iser::IVersionInfo)
	virtual VersionIds GetVersionIds() const;
	virtual bool GetVersionNumber(int versionId, quint32& result) const;
	virtual QString GetVersionIdDescription(int versionId) const;
	virtual QString GetEncodedVersionName(int versionId, quint32 versionNumber) const;
};


} // namespace ibase


#endif // !ibase_CQtVersionInfo_included


