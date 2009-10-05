#ifndef iqt_CFileInfoCopyComp_included
#define iqt_CFileInfoCopyComp_included


// Qt includes
#include <QString>


// ACF includes
#include "istd/CString.h"

#include "ibase/IApplicationInfo.h"
#include "ibase/IFileConvertCopy.h"
#include "ibase/TLoggerCompWrap.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Copy of text files with some tags substitution and optional license attachment.
*/
class CFileInfoCopyComp:
			public ibase::CLoggerComponentBase,
			virtual public ibase::IFileConvertCopy
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_FILE_INFO = 0xa810,
		MI_INPUT_OPEN,
		MI_OUTPUT_OPEN,
		MI_LICENSE_OPEN,
		MI_BAD_TAG
	};

	I_BEGIN_COMPONENT(CFileInfoCopyComp);
		I_REGISTER_INTERFACE(ibase::IFileConvertCopy);

		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Provide information about versions for substitution", false, "VersionInfo");
		I_ASSIGN(m_licensePathAttrPtr, "LicensePath", "Path of license file will be included at begin of copied file", false, "License.txt");
		I_ASSIGN(m_useSubstitutionAttrPtr, "UseSubstitution", "If enabled strings like $AcfVersion:n$ will be substituted", true, false);
		I_ASSIGN_MULTI_0(m_userSubstitutionTagsAttrPtr, "UserSubstitutionTags", "List of user defined substitution tags will be replaced with specified values", false);
		I_ASSIGN_MULTI_0(m_userSubstitutionValuesAttrPtr, "UserSubstitutionValues", "List of user substitution values according to specified user tags", false);
	I_END_COMPONENT;

	// reimplemented (ibase::IFileConvertCopy)
	virtual bool CopyFile(const istd::CString& inputFilePath, const istd::CString& outputFilePath) const;

protected:
	bool ProcessSubstitutionTag(const QString& tag, QString& result) const;

private:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);

	I_ATTR(istd::CString, m_licensePathAttrPtr);
	I_ATTR(bool, m_useSubstitutionAttrPtr);
	I_MULTIATTR(istd::CString, m_userSubstitutionTagsAttrPtr);
	I_MULTIATTR(istd::CString, m_userSubstitutionValuesAttrPtr);
};


} // namespace iqt


#endif // !iqt_CFileInfoCopyComp_included


