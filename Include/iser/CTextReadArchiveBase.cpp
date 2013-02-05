#include "iser/CTextReadArchiveBase.h"


// STL includes
#include <cstring>

// ACF includes
#include "istd/CBase64.h"


namespace iser
{


// reimplemented (iser::IArchive)

bool CTextReadArchiveBase::Process(bool& value)
{
	QByteArray elementText;

	if (Process(elementText)){
		value = (elementText == "true");

		return true;
	}

	return false;
}


bool CTextReadArchiveBase::Process(char& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::Process(quint8& value)
{
	QByteArray elementText;

	if (Process(elementText) && !elementText.isEmpty()){
		QTextStream stream(&elementText, QIODevice::ReadOnly);

		int val;
		stream >> val;
		value = (quint8)val;

		return true;
	}

	return false;
}


bool CTextReadArchiveBase::Process(qint8& value)
{
	QByteArray elementText;

	if (Process(elementText) && !elementText.isEmpty()){
		QTextStream stream(&elementText, QIODevice::ReadOnly);

		int val;
		stream >> val;
		value = (qint8)val;

		return true;
	}

	return false;
}


bool CTextReadArchiveBase::Process(quint16& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::Process(qint16& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::Process(quint32& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::Process(qint32& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::Process(quint64& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::Process(qint64& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::Process(float& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::Process(double& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::ProcessData(void* dataPtr, int size)
{
	quint8* data = (quint8*)dataPtr;

	QByteArray text;
	bool retVal = Process(text);

	if (retVal){
		QVector<quint8> decodedData = istd::CBase64::ConvertFromBase64(text);
		Q_ASSERT(size == int(decodedData.size()));

		std::memcpy(data, &decodedData[0], size);
	}

	return retVal;
}


} // namespace iser


