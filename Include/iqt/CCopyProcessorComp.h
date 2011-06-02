#ifndef iqt_CCopyProcessorComp_included
#define iqt_CCopyProcessorComp_included


// Qt includes
#include <QString>
#include <QDir>


// ACF includes
#include "istd/CString.h"

#include "icomp/CComponentBase.h"

#include "ibase/IFileConvertCopy.h"
#include "ibase/TLoggerCompWrap.h"


namespace iqt
{


/**
	Copy files from one directory to another one.
	This component is part of installation concept. It is designed to use it as free component.
*/
class CCopyProcessorComp:
	public QObject,
	public ibase::CLoggerComponentBase
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_END_STATUS = 0xa830,
		MI_NO_INPUT,
		MI_NO_OUTPUT
	};

	I_BEGIN_COMPONENT(CCopyProcessorComp);
		I_ASSIGN(m_fileCopyCompPtr, "FileCopy", "Provide copy of single file", true, "FileCopy");
		I_ASSIGN(m_inputPathAttrPtr, "InputDir", "Path of input file or directory", true, ".");
		I_ASSIGN(m_outputPathAttrPtr, "OutputDir", "Path of input file or directory", true, "./Copy");
		I_ASSIGN_MULTI_1(m_filtersAttrPtr, "Filters", "List of file filters will be copied", true, "*");
		I_ASSIGN_MULTI_0(m_excludeFiltersAttrPtr, "ExcludeFilters", "List of file filters will be exclude from copy", false);
		I_ASSIGN(m_recursionDepthAttrPtr, "RecursionDepth", "Depth of recursion", true, 0);
	I_END_COMPONENT;

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

	/**
		Check if some file should be excluded from processing.
	*/
	bool CheckIfExcluded(const QString& fileName, const QStringList& excludeFilters) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(ibase::IFileConvertCopy, m_fileCopyCompPtr);
	I_ATTR(istd::CString, m_inputPathAttrPtr);
	I_ATTR(istd::CString, m_outputPathAttrPtr);
	I_MULTIATTR(istd::CString, m_filtersAttrPtr);
	I_MULTIATTR(istd::CString, m_excludeFiltersAttrPtr);
	I_ATTR(int, m_recursionDepthAttrPtr);
};


} // namespace iqt


#endif // !iqt_CCopyProcessorComp_included


