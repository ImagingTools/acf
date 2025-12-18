#include <ifile/CFileWriteSecureArchive.h>


namespace ifile
{


CFileWriteSecureArchive::CFileWriteSecureArchive(
			const QString& filePath,
			const iser::IVersionInfo* versionInfoPtr,
			bool supportTagSkipping ,
			bool serializeHeader)
:	BaseClass(filePath, versionInfoPtr, supportTagSkipping, serializeHeader)
{
}


// reimplemented (ifile::CFileWriteArchive)

bool CFileWriteSecureArchive::ProcessData(void* data, int size)
{
	if (size == 0){
		return true;
	}

	quint8* dataPtr = (quint8*)data;

	QVector<quint8> buffer(size);

	return Encode(dataPtr, &buffer[0], size) && BaseClass::ProcessData(&buffer[0], size);
}


}	// namespace ifile


