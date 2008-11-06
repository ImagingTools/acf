#ifndef CCopyProcessor_included
#define CCopyProcessor_included


// Qt includes
#include <QString>
#include <QDir>

#include "istd/CString.h"


class CCopyProcessor
{
public:
	/**
		Copy all files as described in config.
	*/
	bool ProcessConfigFile(const istd::CString configFilePath);

protected:
	enum{
		MAX_RECURSION_DEPTH = 100
	};

	/**
		Copy single file from \c inputFilePath to \c outputFilePath.
	*/
	typedef bool (CCopyProcessor::*CopyFileFunc)(const QString& inputFilePath, const QString& outputFilePath) const;

	/**
		Check if in tree specified files exists.
		\param	inputDir		input directory.
		\param	filters			list of file filter will be copied.
		\param	recursionDepth	maximale recursion depth.
	*/
	bool CheckFileExistTree(
				const QDir& inputDir,
				const QStringList& filters,
				int recursionDepth = MAX_RECURSION_DEPTH) const;
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
				CopyFileFunc copyFunc,
				int recursionDepth,
				int& counter) const;
	bool CopyBinFile(const QString& inputFileName, const QString& outputFileName) const;
	bool CopySourceFile(const QString& inputFileName, const QString& outputFileName) const;
	bool CheckIfExcluded(const QString& fileName, const QStringList& excludeFilters) const;

private:
	istd::CString m_licenseFileName;
};


#endif // CCopyProcessor_included

