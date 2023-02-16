#include <ifile/CJsonFileReadArchive.h>


// Qt inclides
#include <QtCore/QFile>


namespace ifile
{


// public methods

CJsonFileReadArchive::CJsonFileReadArchive(const QString& filePath, bool serializeHeader)
	:BaseClass()
{
	OpenFile(filePath, serializeHeader);
}


bool CJsonFileReadArchive::OpenFile(const QString &filePath, bool serializeHeader)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly)){
		return false;
	}

	QByteArray inputString = file.readAll();

	return InitArchive(inputString, serializeHeader);
}


} // namespace iser


