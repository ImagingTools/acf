#ifndef iqt_CCopyProcessorComp_included
#define iqt_CCopyProcessorComp_included


// Qt includes
#include <QString>
#include <QDir>

#include "istd/CString.h"

#include "icomp/CComponentBase.h"

#include "ibase/IApplicationInfo.h"
#include "ibase/TLoggerCompWrap.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Copy files from one directory to another one.
	This component is part of installation concept. It is designed to use it as free component.
*/
class CCopyProcessorComp: public ibase::TLoggerCompWrap<icomp::CComponentBase>
{
public:
	typedef ibase::TLoggerCompWrap<icomp::CComponentBase> BaseClass;

	enum MessageId
	{
		MI_END_STATUS = 0xa830,
		MI_NO_INPUT,
		MI_NO_OUTPUT,
		MI_FILE_INFO,
		MI_INPUT_OPEN,
		MI_OUTPUT_OPEN,
		MI_LICENSE_OPEN,
		MI_BAD_VERSION,
		MI_BAD_TAG,
		MI_NO_VERSION_INFO,
	};

	I_BEGIN_COMPONENT(CCopyProcessorComp);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Provide information about versions for substitution", false, "VersionInfo");

		I_ASSIGN(m_inputPathAttrPtr, "InputDir", "Path of input file or directory", true, ".");
		I_ASSIGN(m_outputPathAttrPtr, "OutputDir", "Path of input file or directory", true, "./Copy");
		I_ASSIGN_MULTI_1(m_filtersAttrPtr, "Filters", "List of file filters will be copied", true, "*");
		I_ASSIGN_MULTI_0(m_excludeFiltersAttrPtr, "ExcludeFilters", "List of file filters will be exclude from copy", false);
		I_ASSIGN(m_recursionDepthAttrPtr, "RecursionDepth", "Depth of recursion", true, 0);
		I_ASSIGN(m_licensePathAttrPtr, "LicensePath", "Path of license file will be included at begin of copied file", false, "License.txt");
		I_ASSIGN(m_useSubstitutionAttrPtr, "UseSubstitution", "If enebled strings like $AcfVersion:n$ will be substituted", true, false);
	I_END_COMPONENT;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

protected:
	/**
		Check if in tree specified files exists.
		\param	inputDir		input directory.
		\param	filters			list of file filter will be copied.
		\param	recursionDepth	maximale recursion depth.
	*/
	bool CheckFileExistTree(
				const QDir& inputDir,
				const QStringList& filters,
				const QStringList& excludeFilters,
				int recursionDepth) const;
	/**
		Copy complete files tree for specified filter.
		\param	inputDir		input directory.
		\param	outputDir		output directory.
		\param	filters			list of file filter will be copied.
		\param	copyFunc		function used to copy single file.
		\param	recursionDepth	maximale recursion depth.
	*/
	bool CopyFileTree(
				const QDir& inputDir,
				const QDir& outputDir,
				const QStringList& filters,
				const QStringList& excludeFilters,
				int recursionDepth,
				int& counter) const;
	bool CopyFile(const QString& inputFileName, const QString& outputFileName) const;

	bool CheckIfExcluded(const QString& fileName, const QStringList& excludeFilters) const;

private:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);

	I_ATTR(istd::CString, m_inputPathAttrPtr);
	I_ATTR(istd::CString, m_outputPathAttrPtr);
	I_MULTIATTR(istd::CString, m_filtersAttrPtr);
	I_MULTIATTR(istd::CString, m_excludeFiltersAttrPtr);
	I_ATTR(int, m_recursionDepthAttrPtr);
	I_ATTR(istd::CString, m_licensePathAttrPtr);
	I_ATTR(bool, m_useSubstitutionAttrPtr);
};


} // namespace iqt


#endif // !iqt_CCopyProcessorComp_included


