#ifndef iqt_CFileInfoCopyComp_included
#define iqt_CFileInfoCopyComp_included


// Qt includes
#include <QString>


// ACF includes
#include <QString>

#include "ibase/IApplicationInfo.h"
#include "ibase/IFileConvertCopy.h"
#include "ibase/TLoggerCompWrap.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Copy of text files with some tags substitution and optional license attachment.
	Tags parameter must be separated using \c : character.
	Tags (inclusive parameter) must be enclosed in \c $ character, for example $AcfCompanyName$ or $AcfVersion:0$.
	Known tasks:
	\li	\c AcfCompanyName - gets name of company
	\li	\c AcfProductName - gets name of product
	\li	\c AcfApplicationName - gets name of application
	\li	\c AcfApplicationSubname - gets sub-name of product
	\li	\c AcfApplicationType - gets type information text
	\li	\c AcfLegalCopyright - gets legal copyright information text
	\li	\c AcfVersion - gets version of application, or its module, if parameter with module ID is specified
	\li	\c AcfRawVersion - gets internal version number of application, or its module, if parameter with module ID is specified
	\li	\c AcfRcVersion - gets version number (in RC compiler compatible format) of application, or its module, if parameter with module ID is specified
	\li	\c AcfTimestamp - gets current time
*/
class CFileInfoCopyComp:
			public QObject,
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
	virtual bool ConvertFile(
				const QString& inputFilePath,
				const QString& outputFilePath,
				const iprm::IParamsSet* paramsPtr = NULL) const;

protected:
	bool ProcessSubstitutionTag(const QString& tag, QString& result) const;

private:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);

	I_ATTR(QString, m_licensePathAttrPtr);
	I_ATTR(bool, m_useSubstitutionAttrPtr);
	I_MULTIATTR(QString, m_userSubstitutionTagsAttrPtr);
	I_MULTIATTR(QString, m_userSubstitutionValuesAttrPtr);
};


} // namespace iqt


#endif // !iqt_CFileInfoCopyComp_included


