#include "ifile/CTimeStampFileNameParamComp.h"


// Qt includes
#include <QtCore/QDate>


namespace ifile
{


// public methods

// reimplemented (ifile::IFileNameParam)

const QString& CTimeStampFileNameParamComp::GetPath() const
{
	QString generatedPath = QDate::currentDate().toString("yyyyMMdd");

	m_generatedPath = BaseClass::GetPath() + "/" + generatedPath;

	return m_generatedPath;
}


} // namespace ifile


