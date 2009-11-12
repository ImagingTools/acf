#include "iqt/CClipboardSerializerComp.h"


// Qt includes
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QByteArray>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/CMemoryReadArchive.h"
#include "iser/CMemoryWriteArchive.h"


namespace iqt
{


// reimplemented (iser::IFileLoader)

bool CClipboardSerializerComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iser::ISerializable*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, "Object is not serializable");
		}

		return false;
	}

	if ((flags & QF_NAMED_ONLY) != 0){
		return false;
	}

	return (filePathPtr == NULL) || filePathPtr->IsEmpty();
}


int CClipboardSerializerComp::LoadFromFile(istd::IChangeable& data, const istd::CString& /*filePath*/) const
{
	if (IsOperationSupported(&data, NULL, QF_NO_SAVING | QF_ANONYMOUS_ONLY, false)){
		iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(const_cast<istd::IChangeable*>(&data));
		I_ASSERT(serializablePtr != NULL);	// it was checked in IsOperationSupported

		const QClipboard* clipboardPtr = QApplication::clipboard();
		const QMimeData* mimeDataPtr = clipboardPtr->mimeData();

		QString mimeType = iqt::GetQString(*m_mimeTypeAttrPtr);
		if (mimeDataPtr->hasFormat(mimeType)){
			QByteArray mimeData = mimeDataPtr->data(mimeType);
			if (!mimeData.isEmpty()){
				iser::CMemoryReadArchive archive(mimeData.constData(), mimeData.size());

				istd::CChangeNotifier changePtr(NULL, istd::IChangeable::CF_MODEL);

				if (serializablePtr->Serialize(archive)){
					changePtr.SetPtr(&data);
					
					return StateOk;
				}
				else{
					SendErrorMessage(MI_CANNOT_LOAD, "Cannot read data from clipboard archive (data corrupted)");
				}
			}
		}
		else{
			SendInfoMessage(MI_BAD_OBJECT_TYPE, "No MIME format in archive");
		}
	}

	return StateFailed;
}


int CClipboardSerializerComp::SaveToFile(const istd::IChangeable& data, const istd::CString& /*filePath*/) const
{
	if (IsOperationSupported(&data, NULL, QF_NO_LOADING | QF_ANONYMOUS_ONLY, false)){
		QClipboard* clipboardPtr = QApplication::clipboard();
		if (clipboardPtr == NULL){
			return StateAborted;
		}

		istd::TDelPtr<QMimeData> mimeDataPtr(new QMimeData);
		if (mimeDataPtr.IsValid()){
			iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(const_cast<istd::IChangeable*>(&data));
			I_ASSERT(serializablePtr != NULL);	// it was checked in IsOperationSupported

			iser::CMemoryWriteArchive archive(m_versionInfoCompPtr.GetPtr());
			if (serializablePtr->Serialize(archive)){
				QString mimeType = iqt::GetQString(*m_mimeTypeAttrPtr);
				mimeDataPtr->setData(mimeType, QByteArray((const char*)archive.GetBuffer(), archive.GetBufferSize()));

				clipboardPtr->setMimeData(mimeDataPtr.PopPtr());

				return StateOk;
			}
			else{
				SendErrorMessage(MI_CANNOT_LOAD, "Cannot store data to archive");
			}
		}
	}

	return StateFailed;
}


bool CClipboardSerializerComp::GetFileExtensions(istd::CStringList& /*result*/, int /*flags*/, bool /*doAppend*/) const
{
	return false;
}


istd::CString CClipboardSerializerComp::GetTypeDescription(const istd::CString* /*extensionPtr*/) const
{
	return istd::CString();
}


} // namespace iqt


