#ifndef ibase_TFileSerializerComp_included
#define ibase_TFileSerializerComp_included


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QDir>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CSystem.h"

#include "ibase/CFileSerializerCompBase.h"


namespace ibase
{


/**
	Template implementation of file serializer using loading and storing archive implementation.
*/
template <class ReadArchive, class WriteArchive>
class TFileSerializerComp: public CFileSerializerCompBase
{
public:	
	typedef CFileSerializerCompBase BaseClass;

	I_BEGIN_COMPONENT(TFileSerializerComp);
		I_ASSIGN(m_autoCreateDirectoryAttrPtr, "AutoCreatePath", "Create directory/file path automatically if not exists", true, false);
	I_END_COMPONENT;

	// reimplemented (iser::IFileLoader)
	virtual int LoadFromFile(istd::IChangeable& data, const QString& filePath = QString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const QString& filePath = QString()) const;

protected:
	class ReadArchiveEx: public ReadArchive
	{
	public:
		typedef ReadArchive BaseClass;

		ReadArchiveEx(const QString& filePath, const TFileSerializerComp* loggerPtr)
		:	ReadArchive(filePath),
			m_loggerPtr(loggerPtr)
		{
		}

	protected:
		// reimplemented (istd::ILogger)
		virtual bool IsLogConsumed(
					const istd::IInformationProvider::InformationCategory* /*categoryPtr*/,
					const int* flagsPtr = NULL) const
		{
			static const istd::IInformationProvider::InformationCategory slaveCategory = istd::IInformationProvider::IC_INFO;

			return (m_loggerPtr != NULL) && m_loggerPtr->IsLogConsumed(&slaveCategory, flagsPtr);
		}
		virtual bool SendLogMessage(istd::IInformationProvider::InformationCategory category, int id, const QString& message, const QString& messageSource, int flags = 0) const
		{
			if (m_loggerPtr != NULL){
				QString correctedMessage = message;
				QString correctedMessageSource = messageSource;

				BaseClass::DecorateMessage(category, id, flags, correctedMessage, correctedMessageSource);

				return m_loggerPtr->SendLogMessage(istd::IInformationProvider::IC_INFO, id, correctedMessage, correctedMessageSource, flags);
			}

			return false;
		}

	private:
		const TFileSerializerComp* m_loggerPtr;
	};

	class WriteArchiveEx: public WriteArchive
	{
	public:
		typedef WriteArchive BaseClass;

		WriteArchiveEx(const QString& filePath, const iser::IVersionInfo* infoPtr, const TFileSerializerComp* loggerPtr)
		:	WriteArchive(filePath, infoPtr),
			m_loggerPtr(loggerPtr)
		{
		}

	protected:
		// reimplemented (istd::ILogger)
		virtual bool IsLogConsumed(
					const istd::IInformationProvider::InformationCategory* /*categoryPtr*/,
					const int* flagsPtr = NULL) const
		{
			static const istd::IInformationProvider::InformationCategory slaveCategory = istd::IInformationProvider::IC_INFO;

			return (m_loggerPtr != NULL) && m_loggerPtr->IsLogConsumed(&slaveCategory, flagsPtr);
		}
		virtual bool SendLogMessage(istd::IInformationProvider::InformationCategory category, int id, const QString& message, const QString& messageSource, int flags = 0) const
		{
			if (m_loggerPtr != NULL){
				QString correctedMessage = message;
				QString correctedMessageSource = messageSource;

				BaseClass::DecorateMessage(category, id, flags, correctedMessage, correctedMessageSource);

				return m_loggerPtr->SendLogMessage(category, id, correctedMessage, correctedMessageSource, flags);
			}

			return false;
		}

	private:
		const TFileSerializerComp* m_loggerPtr;
	};

	/**
		Called if read error is occurred.
	*/
	virtual void OnReadError(const ReadArchive& archive, const istd::IChangeable& data, const QString& filePath) const;

protected:
	I_ATTR(bool, m_autoCreateDirectoryAttrPtr);
};


// public methods

// reimplemented (iser::IFileLoader)

template <class ReadArchive, class WriteArchive>
int TFileSerializerComp<ReadArchive, WriteArchive>::LoadFromFile(istd::IChangeable& data, const QString& filePath) const
{
	if (IsOperationSupported(&data, &filePath, QF_LOAD | QF_FILE, false)){
		ReadArchiveEx archive(filePath, this);

		Q_ASSERT(!archive.IsStoring());

		iser::ISerializable* serializablePtr = CompCastPtr<iser::ISerializable>(&data);
		Q_ASSERT(serializablePtr != NULL);

		istd::CChangeNotifier changePtr(NULL, istd::IChangeable::CF_MODEL);

		if (serializablePtr->Serialize(archive)){
			changePtr.SetPtr(&data);

			return StateOk;
		}
		else{
			OnReadError(archive, data, filePath);
		}
	}

	return StateFailed;
}


template <class ReadArchive, class WriteArchive>
int TFileSerializerComp<ReadArchive, WriteArchive>::SaveToFile(const istd::IChangeable& data, const QString& filePath) const
{
	if (*m_autoCreateDirectoryAttrPtr){
		QFileInfo fileInfo(filePath);

		if (!istd::CSystem::EnsurePathExists(fileInfo.dir().absolutePath())){
			SendErrorMessage(MI_CANNOT_SAVE, QObject::tr("Cannot create path to file"));
		}
	}

	if (IsOperationSupported(&data, &filePath, QF_SAVE | QF_FILE, false)){
		WriteArchiveEx archive(filePath, GetVersionInfo(), this);
		Q_ASSERT(archive.IsStoring());

		const iser::ISerializable* serializablePtr = CompCastPtr<iser::ISerializable>(&data);
		Q_ASSERT(serializablePtr != NULL);

		if (!CheckMinimalVersion(*serializablePtr, archive.GetVersionInfo())){
			SendWarningMessage(MI_UNSUPPORTED_VERSION, QObject::tr("Archive version is not supported, possible lost of data"));
		}

		if ((const_cast<iser::ISerializable*>(serializablePtr))->Serialize(archive)){
			return StateOk;
		}
		else{
			SendInfoMessage(MI_CANNOT_SAVE, QObject::tr("Cannot serialize object to file"));
		}
	}

	return StateFailed;
}


// protected methods

template <class ReadArchive, class WriteArchive>
void TFileSerializerComp<ReadArchive, WriteArchive>::OnReadError(const ReadArchive& /*archive*/, const istd::IChangeable& /*data*/, const QString& filePath) const
{
	SendWarningMessage(MI_CANNOT_LOAD, QString(QObject::tr("Cannot load object from file ")) + filePath);
}


} // namespace ibase


#endif // !ibase_TFileSerializerComp_included


