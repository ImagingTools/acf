#include <ifile/CFileReadSecureArchive.h>


namespace ifile
{


CFileReadSecureArchive::CFileReadSecureArchive(
			const QString& filePath,
			bool supportTagSkipping,
			bool serializeHeader)
	:BaseClass(filePath, supportTagSkipping, serializeHeader)
{
}


// reimplemented (ifile::CFileReadArchive)

bool CFileReadSecureArchive::ProcessData(void* data, int size)
{
	if (size <= 0){
		return true;
	}

	quint8* dataPtr = (quint8*)data;

	QVector<quint8> buffer(size);

	return BaseClass::ProcessData(&buffer[0], size) && Decode(&buffer[0], dataPtr, size);
}


}	// namespace ifile


