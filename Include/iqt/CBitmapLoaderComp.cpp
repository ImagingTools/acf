#include "iqt/CBitmapLoaderComp.h"


// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QByteArray>
#include <QtGui/QImageReader>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/IQImageProvider.h"
#include "iqt/CBitmap.h"


namespace iqt
{


// reimplemented (iser::IFileLoader)

bool CBitmapLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const QString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iqt::IQImageProvider*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, tr("Object is not Qt image"));
		}

		return false;
	}

	if (filePathPtr != NULL){
		QFileInfo info(*filePathPtr);

		QStringList extensions;
		if (GetFileExtensions(extensions, flags)){
			QStringList extensionsList = (extensions);

			if (!extensionsList.contains(info.suffix(), Qt::CaseInsensitive)){
				if (!beQuiet){
					SendInfoMessage(MI_BAD_EXTENSION, tr("Bad image file extension %1").arg(info.suffix()));
				}

				return false;
			}
		}

		if ((flags & QF_LOAD) != 0){
			if (!info.exists()){
				if (!beQuiet){
					SendInfoMessage(MI_FILE_NOT_EXIST, tr("Image file %1 not exist").arg(*filePathPtr));
				}

				return false;
			}

			QByteArray format = QImageReader::imageFormat(info.filePath());

			if (format.isEmpty()){
				if (!beQuiet){
					SendInfoMessage(MI_BAD_FORMAT, tr("Bad image format"));
				}
				return false;
			}
		}
	}

	if ((flags & (QF_LOAD | QF_SAVE)) == 0){
		return false;
	}

	return ((flags & QF_FILE) != 0);
}


int CBitmapLoaderComp::LoadFromFile(istd::IChangeable& data, const QString& filePath) const
{
	istd::CChangeNotifier notifier(&data);

	QImage image;
	if (image.load(filePath)){
		bool isOk = false;

		CBitmap* qtBitmapPtr = dynamic_cast<CBitmap*>(&data);
		if (qtBitmapPtr != NULL){
			isOk = qtBitmapPtr->CopyImageFrom(image);
		}
		else{
			CBitmap tempQtBitmap(image);

			isOk = data.CopyFrom(tempQtBitmap);
		}

		if (isOk){
			return StateOk;
		}
		else{
			SendInfoMessage(MI_BAD_OBJECT_TYPE, tr("Object is not supported image"));
		}
	}
	else{
		SendInfoMessage(MI_CANNOT_LOAD, tr("Cannot load file %1").arg(filePath));
	}

	return StateFailed;
}


int CBitmapLoaderComp::SaveToFile(const istd::IChangeable& data, const QString& filePath) const
{
	const iqt::IQImageProvider* imageProviderPtr = dynamic_cast<const iqt::IQImageProvider*>(&data);

	CBitmap qtBitmap;
	if (imageProviderPtr == NULL){
		if (qtBitmap.CopyFrom(data)){
			imageProviderPtr = &qtBitmap;
		}
		else{
			SendInfoMessage(MI_BAD_OBJECT_TYPE, tr("Object is not supported image"));

			return StateFailed;
		}
	}

	I_ASSERT(imageProviderPtr != NULL);

	const QImage& image = imageProviderPtr->GetQImage();
	if (image.save(filePath)){
		return StateOk;
	}
	else{
		SendInfoMessage(MI_CANNOT_SAVE, tr("Cannot save file %1").arg(filePath));
	}

	return StateFailed;
}


// reimplemented (iser::IFileTypeInfo)

bool CBitmapLoaderComp::GetFileExtensions(QStringList& result, int /*flags*/, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	QList<QByteArray> formatList = QImageReader::supportedImageFormats();

	for (		QList<QByteArray>::const_iterator iter = formatList.begin();
				iter != formatList.end();
				++iter){
		const QByteArray& format = *iter;

		result.push_back(QString(format));
	}

	return true;
}


QString CBitmapLoaderComp::GetTypeDescription(const QString* extensionPtr) const
{
	if (		(extensionPtr == NULL) ||
				(extensionPtr->compare("bmp", Qt::CaseInsensitive) == 0) ||
				(extensionPtr->compare("png", Qt::CaseInsensitive) == 0) ||
				(extensionPtr->compare("jpeg", Qt::CaseInsensitive) == 0) ||
				(extensionPtr->compare("jpg", Qt::CaseInsensitive) == 0)){
		return tr("Bitmap");
	}

	return "";
}


} // namespace iqt


