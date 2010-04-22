#include "iqt/CBitmapLoaderComp.h"


// STL includes
#include <algorithm>

// Qt includes
#include <QFileInfo>
#include <QByteArray>
#include <QImageReader>

#include "istd/TChangeNotifier.h"

#include "iqt/IQImageProvider.h"
#include "iqt/CBitmap.h"


namespace iqt
{


// reimplemented (iser::IFileLoader)

bool CBitmapLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iqt::IQImageProvider*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, iqt::GetCString(QObject::tr("Object is not Qt image")));
		}

		return false;
	}

	if (filePathPtr != NULL){
		QString qtFilePath = iqt::GetQString(*filePathPtr);
		QFileInfo info(qtFilePath);

		istd::CStringList extensions;
		if (GetFileExtensions(extensions, flags)){
			QStringList extensionsList = iqt::GetQStringList(extensions);

			if (!extensionsList.contains(info.suffix(), Qt::CaseInsensitive)){
				if (!beQuiet){
					SendInfoMessage(MI_BAD_EXTENSION, iqt::GetCString(QObject::tr("Bad image file extension %1").arg(info.suffix())));
				}

				return false;
			}
		}

		if ((flags & QF_NO_SAVING) != 0){
			if (!info.exists()){
				if (!beQuiet){
					SendInfoMessage(MI_FILE_NOT_EXIST, iqt::GetCString(QObject::tr("Image file %1 not exist").arg(qtFilePath)));
				}

				return false;
			}

			QByteArray format = QImageReader::imageFormat(info.filePath());

			if (format.isEmpty()){
				if (!beQuiet){
					SendInfoMessage(MI_BAD_FORMAT, iqt::GetCString(QObject::tr("Bad image format")));
				}
				return false;
			}
		}
	}

	return ((flags & QF_ANONYMOUS_ONLY) == 0);
}


int CBitmapLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	istd::CChangeNotifier notifier(&data);

	QString qtFilePath = iqt::GetQString(filePath);

	CBitmap qtBitmap;
	QImage& image = qtBitmap.GetQImageRef();
	if (image.load(qtFilePath)){
		if (data.CopyFrom(qtBitmap)){
			return StateOk;
		}
		else{
			SendInfoMessage(MI_BAD_OBJECT_TYPE, iqt::GetCString(QObject::tr("Object is not supported image")));
		}
	}
	else{
		SendInfoMessage(MI_CANNOT_LOAD, iqt::GetCString(QObject::tr("Cannot load file %1").arg(qtFilePath)));
	}

	return StateFailed;
}


int CBitmapLoaderComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	const iqt::IQImageProvider* imageProviderPtr = dynamic_cast<const iqt::IQImageProvider*>(&data);

	CBitmap qtBitmap;
	if (imageProviderPtr == NULL){
		if (qtBitmap.CopyFrom(data)){
			imageProviderPtr = &qtBitmap;
		}
		else{
			SendInfoMessage(MI_BAD_OBJECT_TYPE, iqt::GetCString(QObject::tr("Object is not supported image")));

			return StateFailed;
		}
	}

	I_ASSERT(imageProviderPtr != NULL);

	QString qtFilePath = iqt::GetQString(filePath);

	const QImage& image = imageProviderPtr->GetQImage();
	if (image.save(qtFilePath)){
		return StateOk;
	}
	else{
		SendInfoMessage(MI_CANNOT_SAVE, iqt::GetCString(QObject::tr("Cannot save file %1").arg(qtFilePath)));
	}

	return StateFailed;
}


// reimplemented (iser::IFileTypeInfo)

bool CBitmapLoaderComp::GetFileExtensions(istd::CStringList& result, int /*flags*/, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	QList<QByteArray> formatList = QImageReader::supportedImageFormats();

	for (		QList<QByteArray>::const_iterator iter = formatList.begin();
				iter != formatList.end();
				++iter){
		const QByteArray& format = *iter;

		result.push_back(iqt::GetCString(QString(format)));
	}

	return true;
}


istd::CString CBitmapLoaderComp::GetTypeDescription(const istd::CString* extensionPtr) const
{
	if (		(extensionPtr == NULL) ||
				extensionPtr->IsEqualNoCase("bmp") ||
				extensionPtr->IsEqualNoCase("png") ||
				extensionPtr->IsEqualNoCase("jpeg") ||
				extensionPtr->IsEqualNoCase("jpg")){
		return iqt::GetCString(QObject::tr("Bitmap"));
	}

	return "";
}


} // namespace iqt


