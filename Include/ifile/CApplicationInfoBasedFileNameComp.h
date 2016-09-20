#ifndef ifile_CApplicationInfoBasedFileNameComp_included
#define ifile_CApplicationInfoBasedFileNameComp_included


// ACF includes
#include <ifile/CRelativeFileNameParamComp.h>
#include <ibase/IApplicationInfo.h>

namespace ifile
{

class CApplicationInfoBasedFileNameComp: public ifile::CRelativeFileNameParamComp
{
public:
	typedef ifile::CRelativeFileNameParamComp BaseClass;

	I_BEGIN_COMPONENT(CApplicationInfoBasedFileNameComp);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info", false, "ApplicationInfo");
	I_END_COMPONENT;

	// reimplemented (ifile::IFileNameParam)
	virtual const QString& GetPath() const;

private:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);

	mutable QString m_calculatedPath;

	static QString s_processIdVariable;
	static QString s_companyNameVariable;
	static QString s_productNameVariable;
	static QString s_applicationNameVariable;
	static QString s_applicationSubnameVariable;
	static QString s_applicationTypeVariable;
	static QString s_mainVersionVariable;
	static QString s_userVariable;
};


} // namespace ifile


#endif // !ifile_CApplicationInfoBasedFileNameComp_included


