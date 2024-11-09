#pragma once


// ACF includes
#include <ilog/TLoggerCompWrap.h>
#include <ibase/IProgressManager.h>
#include <ifile/IDeviceBasedPersistence.h>


namespace ifile
{


/**
	Template implementation of file serializer using loading and storing archive implementation.

	\ingroup Persistence
*/
template <class ReadArchive, class WriteArchive>
class TDeviceBasedSerializerComp: public ilog::CLoggerComponentBase, virtual public IDeviceBasedPersistence
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(TDeviceBasedSerializerComp);
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Provide information about archive versions", false, "VersionInfo");
	I_END_COMPONENT;

	// reimplemented (ifile::IDeviceBasedPersistence)
	virtual bool IsDeviceOperationSupported(const istd::IChangeable& dataObject, const QIODevice& device, int deviceOperation) const override;
	virtual int ReadFromDevice(
				istd::IChangeable& data,
				QIODevice& device,
				ibase::IProgressManager* progressManagerPtr = nullptr) const override;
	virtual int WriteToDevice(
				const istd::IChangeable& data,
				QIODevice& device,
				ibase::IProgressManager* progressManagerPtr = nullptr) const override;

	// Wrapper classes for archives
	class ReadArchiveEx: public ReadArchive
	{
	public:
		typedef ReadArchive BaseClass;

		ReadArchiveEx(QIODevice& device, const istd::ILogger* loggerPtr)
		:	ReadArchive(device),
			m_loggerPtr(loggerPtr)
		{
		}

		virtual bool SendLogMessage(
					istd::IInformationProvider::InformationCategory category,
					int id,
					const QString& message,
					const QString& messageSource,
					int flags = 0) const
		{
			if (m_loggerPtr != nullptr){
				QString correctedMessage = message;
				QString correctedMessageSource = messageSource;

				BaseClass::DecorateMessage(category, id, flags, correctedMessage, correctedMessageSource);

				return m_loggerPtr->SendLogMessage(istd::IInformationProvider::IC_INFO, id, correctedMessage, correctedMessageSource, flags);
			}

			return false;
		}

	protected:
		// reimplemented (istd::ILogger)
		virtual bool IsLogConsumed(
					const istd::IInformationProvider::InformationCategory* /*categoryPtr*/,
					const int* flagsPtr = nullptr) const override
		{
			static const istd::IInformationProvider::InformationCategory slaveCategory = istd::IInformationProvider::IC_INFO;

			return (m_loggerPtr != nullptr) && m_loggerPtr->IsLogConsumed(&slaveCategory, flagsPtr);
		}

	private:
		const istd::ILogger* m_loggerPtr;
	};

	class WriteArchiveEx: public WriteArchive
	{
	public:
		typedef WriteArchive BaseClass;

		WriteArchiveEx(QIODevice& device, const iser::IVersionInfo* infoPtr, const istd::ILogger* loggerPtr)
		:	WriteArchive(device, infoPtr),
			m_loggerPtr(loggerPtr)
		{
		}

		virtual bool SendLogMessage(
					istd::IInformationProvider::InformationCategory category,
					int id,
					const QString& message,
					const QString& messageSource,
					int flags = 0) const
		{
			if (m_loggerPtr != nullptr){
				QString correctedMessage = message;
				QString correctedMessageSource = messageSource;

				BaseClass::DecorateMessage(category, id, flags, correctedMessage, correctedMessageSource);

				return m_loggerPtr->SendLogMessage(category, id, correctedMessage, correctedMessageSource, flags);
			}

			return false;
		}

	protected:
		// reimplemented (istd::ILogger)
		virtual bool IsLogConsumed(
					const istd::IInformationProvider::InformationCategory* /*categoryPtr*/,
					const int* flagsPtr = nullptr) const override
		{
			static const istd::IInformationProvider::InformationCategory slaveCategory = istd::IInformationProvider::IC_INFO;

			return (m_loggerPtr != nullptr) && m_loggerPtr->IsLogConsumed(&slaveCategory, flagsPtr);
		}

	private:
		const istd::ILogger* m_loggerPtr;
	};

protected:
	/**
		Get working version info.
	*/
	virtual const iser::IVersionInfo* GetVersionInfo() const;

	/**
		Check if the minimal version of some serializable object is supported by version info.
	*/
	bool CheckMinimalVersion(const iser::ISerializable& object, const iser::IVersionInfo& versionInfo) const;

	/**
		Called if read error is occurred.
	*/
	virtual void OnReadError(const ReadArchive& archive, const istd::IChangeable& data) const;
private:
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
};


// public methods

// reimplemented (ifile::IFilePersistence)

template <class ReadArchive, class WriteArchive>
bool TDeviceBasedSerializerComp<ReadArchive, WriteArchive>::IsDeviceOperationSupported(
			const istd::IChangeable& /*dataObject*/,
			const QIODevice& /*device*/,
			int /*deviceOperation*/) const
{
	return true;
}


template <class ReadArchive, class WriteArchive>
int TDeviceBasedSerializerComp<ReadArchive, WriteArchive>::ReadFromDevice(
			istd::IChangeable& data,
			QIODevice& device,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (IsDeviceOperationSupported(data, device, ReadOperation)){
		ReadArchiveEx archive(device, this);

		Q_ASSERT(!archive.IsStoring());

		iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(&data);
		if (serializablePtr == nullptr){
			serializablePtr = CompCastPtr<iser::ISerializable>(&data);
		}

		Q_ASSERT(serializablePtr != nullptr);

		if (serializablePtr->Serialize(archive)){
			return Successful;
		}
		else{
			OnReadError(archive, data);
		}
	}

	return Failed;
}


template <class ReadArchive, class WriteArchive>
int TDeviceBasedSerializerComp<ReadArchive, WriteArchive>::WriteToDevice(
			const istd::IChangeable& data,
			QIODevice& device,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (IsDeviceOperationSupported(data, device, WriteOperation)){
		WriteArchiveEx archive(device, GetVersionInfo(), this);
		Q_ASSERT(archive.IsStoring());

		const iser::ISerializable* serializablePtr = dynamic_cast<const iser::ISerializable*>(&data);
		if(serializablePtr == nullptr){
			serializablePtr = CompCastPtr<iser::ISerializable>(&data);
		}
		Q_ASSERT(serializablePtr != nullptr);

		if (!CheckMinimalVersion(*serializablePtr, archive.GetVersionInfo())){
			SendWarningMessage(UnsupportedArchiveVersion, QObject::tr("Archive version is not supported, possible lost of data"));
		}

		if ((const_cast<iser::ISerializable*>(serializablePtr))->Serialize(archive)){
			return Successful;
		}
		else{
			SendInfoMessage(WriteOperationFailed, QObject::tr("Cannot serialize object"));
		}
	}

	return Failed;
}


// protected methods

template <class ReadArchive, class WriteArchive>
const iser::IVersionInfo* TDeviceBasedSerializerComp<ReadArchive, WriteArchive>::GetVersionInfo() const
{
	return m_versionInfoCompPtr.GetPtr();
}



template <class ReadArchive, class WriteArchive>
bool TDeviceBasedSerializerComp<ReadArchive, WriteArchive>::CheckMinimalVersion(const iser::ISerializable& object, const iser::IVersionInfo& versionInfo) const
{
	iser::IVersionInfo::VersionIds ids = versionInfo.GetVersionIds();

	for (iser::IVersionInfo::VersionIds::const_iterator iter = ids.begin();
		iter != ids.end();
		++iter) {
		int id = *iter;

		quint32 objectMinimalVersion = object.GetMinimalVersion(id);

		quint32 versionNumber;
		if (versionInfo.GetVersionNumber(id, versionNumber)) {
			if (versionNumber < objectMinimalVersion) {
				return false;
			}
		}
	}

	return true;
}


template <class ReadArchive, class WriteArchive>
void TDeviceBasedSerializerComp<ReadArchive, WriteArchive>::OnReadError(
			const ReadArchive& /*archive*/,
			const istd::IChangeable& /*data*/) const
{
	SendWarningMessage(ReadOperationFailed, QString(QObject::tr("Cannot load object")) );
}


} // namespace ifile


