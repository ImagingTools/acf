#include <ifile/CTimeStampFileNameParamComp.h>


// Qt includes
#include <QtCore/QDate>


namespace ifile
{


// public methods

// reimplemented (ifile::IFileNameParam)

const QString& CTimeStampFileNameParamComp::GetPath() const
{
	const QString format = m_dateFormatAttrPtr.IsValid() ? *m_dateFormatAttrPtr : "yyyyMMdd";
	const QString prefix = m_prefixAttrPtr.IsValid() ? *m_prefixAttrPtr : "";
	const QString suffix = m_suffixAttrPtr.IsValid() ? *m_suffixAttrPtr : "";

	QString generatedPath = QDate::currentDate().toString(format);

	m_generatedPath = BaseClass::GetPath() + "/" + prefix + generatedPath + suffix;

	if (GetPathType() == PT_FILE){
		QString extension = m_fileExtensionAttrPtr.IsValid() ? *m_fileExtensionAttrPtr : "";
		m_generatedPath += extension;
	}

	return m_generatedPath;
}


} // namespace ifile


