#include <iser/CTextReadArchiveBase.h>


// STL includes
#include <cstring>


namespace iser
{


// reimplemented (iser::IArchive)

bool CTextReadArchiveBase::Process(bool& value)
{
	QByteArray text;
	if (ReadTextNode(text)){
		if (text == "true"){
			value = true;

			return true;
		}
		else if (text == "false"){
			value = false;

			return true;
		}
		else{
			if (IsLogConsumed()){
				SendLogMessage(
							istd::IInformationProvider::IC_ERROR,
							MI_TAG_ERROR,
							QString("Expected boolean value, found '%1'").arg(QString(text)),
							"TextReader",
							istd::IInformationProvider::ITF_SYSTEM);
			}
		}
	}

	return false;
}


bool CTextReadArchiveBase::Process(char& value)
{
	QByteArray text;
	if (ReadTextNode(text)){
		if (!text.isEmpty()){
			value = text.at(0);

			return true;
		}

		if (IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Expected sigle character"),
						"TextReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}
	}

	return false;
}


bool CTextReadArchiveBase::Process(quint8& value)
{
	bool retVal = false;

	QByteArray text;
	if (ReadTextNode(text)){
		value = quint8(text.toShort(&retVal));

		if (!retVal && IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Expected integer value, found '%1'").arg(QString(text)),
						"TextReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}
	}

	return retVal;
}


bool CTextReadArchiveBase::Process(qint8& value)
{
	bool retVal = false;

	QByteArray text;
	if (ReadTextNode(text)){
		value = qint8(text.toShort(&retVal));

		if (!retVal && IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Expected integer value, found '%1'").arg(QString(text)),
						"TextReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}
	}

	return retVal;
}


bool CTextReadArchiveBase::Process(quint16& value)
{
	bool retVal = false;

	QByteArray text;
	if (ReadTextNode(text)){
		value = text.toUShort(&retVal);

		if (!retVal && IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Expected integer value, found '%1'").arg(QString(text)),
						"TextReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}
	}

	return retVal;
}


bool CTextReadArchiveBase::Process(qint16& value)
{
	bool retVal = false;

	QByteArray text;
	if (ReadTextNode(text)){
		value = text.toShort(&retVal);

		if (!retVal && IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Expected integer value, found '%1'").arg(QString(text)),
						"TextReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}
	}

	return retVal;
}


bool CTextReadArchiveBase::Process(quint32& value)
{
	bool retVal = false;

	QByteArray text;
	if (ReadTextNode(text)){
		value = text.toUInt(&retVal);

		if (!retVal && IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Expected integer value, found '%1'").arg(QString(text)),
						"TextReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}
	}

	return retVal;
}


bool CTextReadArchiveBase::Process(qint32& value)
{
	bool retVal = false;

	QByteArray text;
	if (ReadTextNode(text)){
		value = text.toInt(&retVal);

		if (!retVal && IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Expected integer value, found '%1'").arg(QString(text)),
						"TextReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}
	}

	return retVal;
}


bool CTextReadArchiveBase::Process(quint64& value)
{
	bool retVal = false;

	QByteArray text;
	if (ReadTextNode(text)){
		value = text.toULongLong(&retVal);

		if (!retVal && IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Expected integer value, found '%1'").arg(QString(text)),
						"TextReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}
	}

	return retVal;
}


bool CTextReadArchiveBase::Process(qint64& value)
{
	bool retVal = false;

	QByteArray text;
	if (ReadTextNode(text)){
		value = text.toLongLong(&retVal);

		if (!retVal && IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Expected integer value, found '%1'").arg(QString(text)),
						"TextReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}
	}

	return retVal;
}


bool CTextReadArchiveBase::Process(float& value)
{
	bool retVal = false;

	QByteArray text;
	if (ReadTextNode(text)){
		value = text.toFloat(&retVal);

		if (!retVal && IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Expected numeric value, found '%1'").arg(QString(text)),
						"TextReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}
	}

	return retVal;
}


bool CTextReadArchiveBase::Process(double& value)
{
	bool retVal = false;

	QByteArray text;
	if (ReadTextNode(text)){
		value = text.toDouble(&retVal);

		if (!retVal && IsLogConsumed()){
			SendLogMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_TAG_ERROR,
						QString("Expected numeric value, found '%1'").arg(QString(text)),
						"TextReader",
						istd::IInformationProvider::ITF_SYSTEM);
		}
	}

	return retVal;
}


bool CTextReadArchiveBase::Process(QByteArray& value)
{
	return ReadTextNode(value);
}


bool CTextReadArchiveBase::ProcessData(void* dataPtr, int size)
{
	QByteArray text;
	if (ReadTextNode(text)){
		QByteArray decodedData = QByteArray::fromBase64(text);
		Q_ASSERT(size == int(decodedData.size()));

		std::memcpy(dataPtr, decodedData.constData(), size);

		return true;
	}

	return false;
}


} // namespace iser


