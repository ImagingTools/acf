#include "iqtmm/CMediaLoaderComp.h"


// Qt includes
#include <QFileInfo>

#include "imm/IMediaController.h"


namespace iqtmm
{


// public methods


// reimplemented (iser::IFileLoader)

bool CMediaLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const imm::IMediaController*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, iqt::GetCString(QObject::tr("Object is not a valid")));
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
					SendInfoMessage(MI_BAD_EXTENSION, iqt::GetCString(QObject::tr("Bad multimedia file extension %1").arg(info.suffix())));
				}

				return false;
			}
		}
	}

	return ((flags & (QF_ANONYMOUS_ONLY | QF_NO_LOADING)) == 0);
}


int CMediaLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	if (IsOperationSupported(&data, &filePath, QF_NO_SAVING, false)){
		imm::IMediaController* controllerPtr = dynamic_cast<imm::IMediaController*>(&data);
		if (controllerPtr != NULL){
			if (controllerPtr->OpenMediumUrl(filePath, *m_autoPlayAttrPtr)){
				return StateOk;
			}
			else{
				SendInfoMessage(MI_CANNOT_LOAD, iqt::GetCString(QObject::tr("Cannot open media '%1'").arg(iqt::GetQString(filePath))));
			}
		}
		else{
			SendWarningMessage(MI_BAD_OBJECT_TYPE, iqt::GetCString(QObject::tr("Wrong object type")));
		}
	}

	return StateFailed;
}


int CMediaLoaderComp::SaveToFile(const istd::IChangeable&/* data*/, const istd::CString&/* filePath*/) const
{
	return StateFailed;
}


bool CMediaLoaderComp::GetFileExtensions(istd::CStringList& result, int flags, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	if ((flags & QF_NO_LOADING) == 0){
		result.push_back("mpg");
		result.push_back("mpeg");
		result.push_back("mov");
		result.push_back("wmv");
		result.push_back("avi");
		result.push_back("m2p");
		result.push_back("mp4");
	}

	return true;
}


istd::CString CMediaLoaderComp::GetTypeDescription(const istd::CString* extensionPtr) const
{
	bool isKnown = (extensionPtr == NULL);

	if (!isKnown){
		istd::CStringList extensions;
		QStringList extensionsList = iqt::GetQStringList(extensions);

		if (!extensionsList.contains(iqt::GetQString(*extensionPtr), Qt::CaseInsensitive)){
			isKnown = true;
		}
	}

	if (isKnown){
		return iqt::GetCString(QObject::tr("Multimedia"));
	}

	return "";
}


} // namespace iqtmm


