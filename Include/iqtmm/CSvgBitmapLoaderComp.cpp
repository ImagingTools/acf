#include "iqtmm/CSvgBitmapLoaderComp.h"


// Qt includes
#include <QFileInfo>
#include <QSvgRenderer>
#include <QPainter>


// ACF includes
#include "iimg/IBitmap.h"
#include "iqt/IQImageProvider.h"


namespace iqtmm
{


// public methods


// reimplemented (iser::IFileLoader)

bool CSvgBitmapLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if (		(dataObjectPtr != NULL) &&
				(dynamic_cast<const iimg::IRasterImage*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, iqt::GetCString(QObject::tr("Object is not a valid")));
		}

		return false;
	}

	if (filePathPtr != NULL){
		if (filePathPtr->IsEmpty()){
			return false;
		}

		QString qtFilePath = iqt::GetQString(*filePathPtr);
		QFileInfo info(qtFilePath);

		istd::CStringList extensions;
		if (GetFileExtensions(extensions, flags)){
			QStringList extensionsList = iqt::GetQStringList(extensions);

			if (!extensionsList.contains(info.suffix(), Qt::CaseInsensitive)){
				if (!beQuiet){
					SendInfoMessage(MI_BAD_EXTENSION, iqt::GetCString(QObject::tr("Bad SVG file extension %1").arg(info.suffix())));
				}

				return false;
			}
		}
	}

	return ((flags & (QF_ANONYMOUS_ONLY | QF_NO_LOADING)) == 0);
}


int CSvgBitmapLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	if (IsOperationSupported(&data, &filePath, QF_NO_SAVING, false)){
		I_ASSERT(!filePath.IsEmpty());	// should be checked by IsOperationSupported

		iimg::IRasterImage* imagePtr = dynamic_cast<iimg::IBitmap*>(&data);
		I_ASSERT(imagePtr != NULL);	// should be checked by IsOperationSupported
		if (filePath != m_lastFilePath){
			QSvgRenderer renderer;
			if (renderer.load(iqt::GetQString(filePath))){
				if (!m_lastBitmap.CreateBitmap(istd::CIndex2d(*m_bitmapWidthAttrPtr, *m_bitmapHeightAttrPtr), 32, 4)){
					m_lastFilePath.Reset();

					SendInfoMessage(MI_BITMAP_TYPE, iqt::GetCString(QObject::tr("Cannot create bitmap")));

					return StateFailed;
				}

				QPainter painter(&m_lastBitmap.GetQImageRef());

				renderer.render(&painter);

				m_lastFilePath = filePath;
			}
			else{
				SendInfoMessage(MI_CANNOT_LOAD, iqt::GetCString(QObject::tr("Cannot load SVG file '%1'").arg(iqt::GetQString(filePath))));

				return StateFailed;
			}
		}

		if (imagePtr->CopyFrom(m_lastBitmap)){
			return StateOk;
		}
		else{
			SendInfoMessage(MI_BITMAP_TYPE, iqt::GetCString(QObject::tr("Cannot copy bitmap from Qt bitmap")));
		}
	}

	return StateFailed;
}


int CSvgBitmapLoaderComp::SaveToFile(const istd::IChangeable&/* data*/, const istd::CString&/* filePath*/) const
{
	return StateFailed;
}


// reimplemented (iser::IFileTypeInfo)

bool CSvgBitmapLoaderComp::GetFileExtensions(istd::CStringList& result, int flags, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	if ((flags & QF_NO_LOADING) == 0){
		result.push_back("svg");
	}

	return true;
}


istd::CString CSvgBitmapLoaderComp::GetTypeDescription(const istd::CString* extensionPtr) const
{
	bool isKnown = (extensionPtr == NULL);

	if (!isKnown){
		istd::CStringList extensions;
		if (GetFileExtensions(extensions)){
			QStringList extensionsList = iqt::GetQStringList(extensions);
			if (!extensionsList.contains(iqt::GetQString(*extensionPtr), Qt::CaseInsensitive)){
				isKnown = true;
			}
		}
	}

	if (isKnown){
		return iqt::GetCString(QObject::tr("SVG image"));
	}

	return "";
}


} // namespace iqtmm


