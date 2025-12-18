#ifndef ibase_CQtVersionInfo_included
#define ibase_CQtVersionInfo_included


// ACF includes
#include <ibase/TVersionInfoBase.h>


namespace ibase
{


/**
	Component representation of the Qt module version.
*/
class CQtVersionInfoComp: public ibase::CVersionInfoLoggerCompBase
{
public:
	typedef ibase::CVersionInfoLoggerCompBase BaseClass;

	I_BEGIN_COMPONENT(CQtVersionInfoComp);
	I_END_COMPONENT;

	enum VersionId
	{
		/**
			Version of Qt framework against the actual module was compiled.
		*/
		VI_QT_VERSION = QtVersionId
	};

	// reimplemented (iser::IVersionInfo)
	virtual VersionIds GetVersionIds() const override;
	virtual bool GetVersionNumber(int versionId, quint32& result) const override;
	virtual QString GetVersionIdDescription(int versionId) const override;
	virtual QString GetEncodedVersionName(int versionId, quint32 versionNumber) const override;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	/**
		Get used runtime version of the Qt framework
	*/
	static quint32 GetRuntimeVersion();
};


} // namespace ibase


#endif // !ibase_CQtVersionInfo_included


