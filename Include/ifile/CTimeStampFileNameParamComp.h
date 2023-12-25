#pragma once


// ACF includes
#include <ifile/CRelativeFileNameParamComp.h>


namespace ifile
{


/**
	File path generation using time stamp.
*/
class CTimeStampFileNameParamComp: public ifile::CRelativeFileNameParamComp
{
public:
	typedef ifile::CRelativeFileNameParamComp BaseClass;

	I_BEGIN_COMPONENT(CTimeStampFileNameParamComp);
		I_ASSIGN(m_fileExtensionAttrPtr, "FileExtension", "Extension of file, if path type is set to file", false, "");
		I_ASSIGN(m_dateFormatAttrPtr, "DateFormat", "Format of timestamp string in name of file", true,"yyyyMMdd");
		I_ASSIGN(m_prefixAttrPtr, "PrefixText", "Timestamp prefix text in name of log file", false, "");
		I_ASSIGN(m_suffixAttrPtr, "SuffixText", "Timestamp suffix text in name of log file", false, "");
	I_END_COMPONENT;

	// reimplemented (ifile::IFileNameParam)
	virtual const QString& GetPath() const override;

private:
	I_ATTR(QString, m_fileExtensionAttrPtr);
	I_ATTR(QString, m_dateFormatAttrPtr);
	I_ATTR(QString, m_prefixAttrPtr);
	I_ATTR(QString, m_suffixAttrPtr);

	mutable QString m_generatedPath;
};


} // namespace ifile


