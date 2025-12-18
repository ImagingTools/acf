#include <iimg/CBitmapLoaderComp.h>


// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QByteArray>
#include <QtGui/QImageReader>
#include <QtGui/QImageWriter>


// ACF includes
#include <istd/CChangeNotifier.h>

#include <iimg/IQImageProvider.h>
#include <iimg/CBitmap.h>


namespace iimg
{


// reimplemented (ifile::IFilePersistence)

bool CBitmapLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const QString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iimg::IQImageProvider*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendErrorMessage(MI_BAD_OBJECT_TYPE, QT_TR_NOOP("Object is not Qt image"));
		}

		return false;
	}

	if (filePathPtr != NULL){
		QFileInfo info(*filePathPtr);

		QStringList extensions;
		if (GetFileExtensions(extensions, dataObjectPtr, flags)){
			QStringList extensionsList = (extensions);

			if (!extensionsList.contains(info.suffix(), Qt::CaseInsensitive)){
				if (!beQuiet){
					SendErrorMessage(MI_BAD_EXTENSION, QT_TR_NOOP(QString("Bad image file extension %1").arg(info.suffix())));
				}

				return false;
			}
		}

		if ((flags & QF_LOAD) != 0){
			if (!info.exists()){
				if (!beQuiet){
					SendErrorMessage(MI_FILE_NOT_EXIST, QT_TR_NOOP(QString("Image file %1 not exist").arg(*filePathPtr)));
				}

				return false;
			}

			QByteArray format = QImageReader::imageFormat(info.filePath());

			if (format.isEmpty()){
				if (!beQuiet){
					SendErrorMessage(MI_BAD_FORMAT, QT_TR_NOOP("Bad image format"));
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


ifile::IFilePersistence::OperationState CBitmapLoaderComp::LoadFromFile(
			istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	istd::CChangeNotifier notifier(&data);

	QImageReader reader(filePath);
	reader.setAutoTransform(true);
	QImage image = reader.read();
	if (image.isNull()) {
		SendErrorMessage(MI_CANNOT_LOAD, QT_TR_NOOP(QString("Cannot load file %1: %2").arg(filePath).arg(reader.errorString())));
		return OS_FAILED;
	}

	bool isOk = false;

	if (CBitmap* qtBitmapPtr = dynamic_cast<CBitmap*>(&data)){
		isOk = qtBitmapPtr->CopyImageFrom(image);
	}
	else{
		CBitmap tempQtBitmap(image);

		isOk = data.CopyFrom(tempQtBitmap);
	}

	if (isOk) return OS_OK;
	SendErrorMessage(MI_BAD_OBJECT_TYPE, QT_TR_NOOP("Cannot set the loaded data to the end-point object"));
	return OS_FAILED;
}


ifile::IFilePersistence::OperationState CBitmapLoaderComp::SaveToFile(
			const istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	const iimg::IQImageProvider* imageProviderPtr = dynamic_cast<const iimg::IQImageProvider*>(&data);

	CBitmap qtBitmap;
	if (imageProviderPtr == NULL){
		if (qtBitmap.CopyFrom(data)){
			imageProviderPtr = &qtBitmap;
		}
		else{
			SendErrorMessage(MI_BAD_OBJECT_TYPE, QT_TR_NOOP("Object is not supported image"));

			return OS_FAILED;
		}
	}

	Q_ASSERT(imageProviderPtr != NULL);

	const QImage& image = imageProviderPtr->GetQImage();
	if (image.save(filePath)){
		return OS_OK;
	}
	else{
		SendErrorMessage(MI_CANNOT_SAVE, QT_TR_NOOP(QString("Cannot save file %1").arg(filePath)));
	}

	return OS_FAILED;
}


// reimplemented (ifile::IFileTypeInfo)

bool CBitmapLoaderComp::GetFileExtensions(QStringList& result, const istd::IChangeable* /*dataObjectPtr*/, int flags, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	QSet<QByteArray> formatList;

	if ((flags & QF_LOAD) != 0){
		QList<QByteArray> imageFormats = QImageReader::supportedImageFormats();
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
		formatList += QSet<QByteArray>(imageFormats.begin(), imageFormats.end());
#else
		formatList += imageFormats.toSet();
#endif
	}
	if ((flags & QF_SAVE) != 0){
		QList<QByteArray> imageFormats = QImageWriter::supportedImageFormats();
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
		formatList += QSet<QByteArray>(imageFormats.begin(), imageFormats.end());
#else
		formatList += imageFormats.toSet();
#endif
	}

	if (m_extensionFilterAttrPtr.IsValid()){
		int extensionsCount = m_extensionFilterAttrPtr.GetCount();
		for (int i = 0; i < extensionsCount; ++i){
			const QByteArray& format = m_extensionFilterAttrPtr[i];

			if (formatList.contains(format)){
				result.push_back(QString(format));
			}
		}
	}
	else{
		for (		QSet<QByteArray>::const_iterator iter = formatList.begin();
					iter != formatList.end();
					++iter){
			const QByteArray& format = *iter;

			result.push_back(QString(format));
		}
	}

	return true;
}


QString CBitmapLoaderComp::GetTypeDescription(const QString* extensionPtr) const
{
	if (extensionPtr != NULL){
		if (		(extensionPtr->compare("jpeg", Qt::CaseInsensitive) == 0) ||
					(extensionPtr->compare("jpg", Qt::CaseInsensitive) == 0)){
			return tr("Joint Photographic Experts Group Image");
		}

		if (		(extensionPtr->compare("tiff", Qt::CaseInsensitive) == 0) ||
					(extensionPtr->compare("tif", Qt::CaseInsensitive) == 0)){
			return tr("Tagged Image File Format");
		}

		if (extensionPtr->compare("bmp", Qt::CaseInsensitive) == 0){
			return tr("Windows Bitmap");
		}

		if (extensionPtr->compare("gif", Qt::CaseInsensitive) == 0){
			return tr("Graphic Interchange Format");
		}

		if (extensionPtr->compare("png", Qt::CaseInsensitive) == 0){
			return tr("Portable Network Graphics");
		}

		if (extensionPtr->compare("pbm", Qt::CaseInsensitive) == 0){
			return tr("Portable Bitmap");
		}

		if (extensionPtr->compare("pgm", Qt::CaseInsensitive) == 0){
			return tr("Portable Graymap");
		}

		if (extensionPtr->compare("ppm", Qt::CaseInsensitive) == 0){
			return tr("Portable Pixmap");
		}

		if (extensionPtr->compare("xbm", Qt::CaseInsensitive) == 0){
			return tr("X11 Bitmap");
		}

		if (extensionPtr->compare("xpm", Qt::CaseInsensitive) == 0){
			return tr("X11 Pixmap");
		}

		if (extensionPtr->compare("svg", Qt::CaseInsensitive) == 0){
			return tr("Scalable Vector Graphics");
		}

		if (extensionPtr->compare("svgz", Qt::CaseInsensitive) == 0){
			return tr("Compressed Scalable Vector Graphics");
		}

		if (extensionPtr->compare("ico", Qt::CaseInsensitive) == 0){
			return tr("Windows Icon");
		}

		if (extensionPtr->compare("icns", Qt::CaseInsensitive) == 0){
			return tr("MAC Icon");
		}
	}

	return tr("Bitmap");
}


} // namespace iimg


