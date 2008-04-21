#include "iqt/CBitmapLoaderComp.h"


// Qt includes
#include <QImage>
#include <QFileInfo>
#include <QByteArray>
#include <QImageReader>


namespace iqt
{


// reimplemented (iser::IFileLoader)

bool CBitmapLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			bool forLoading,
			bool forSaving) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const QImage*>(dataObjectPtr) == NULL)){
		return false;
	}

	if (filePathPtr != NULL){
		QFileInfo info(iqt::GetQString(*filePathPtr));
		if (forLoading && !forSaving){
			if (!info.exists()){
				return false;
			}

			QByteArray format = QImageReader::imageFormat(info.filePath());

			return !format.isEmpty();
		}
	}

	return true;
}


int CBitmapLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	QImage* imagePtr = dynamic_cast<QImage*>(&data);

	if ((imagePtr != NULL) && imagePtr->load(iqt::GetQString(filePath))){
		m_lastLoadFileName = filePath;

		return StateOk;
	}

	return StateFailed;
}


int CBitmapLoaderComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	const QImage* imagePtr = dynamic_cast<const QImage*>(&data);

	if ((imagePtr != NULL) && imagePtr->save(iqt::GetQString(filePath))){
		m_lastSaveFileName = filePath;

		return StateOk;
	}

	return StateFailed;
}


const istd::CString& CBitmapLoaderComp::GetLastLoadFileName() const
{
	return m_lastLoadFileName;
}


const istd::CString& CBitmapLoaderComp::GetLastSaveFileName() const
{
	return m_lastSaveFileName;
}


} // namespace iqt


