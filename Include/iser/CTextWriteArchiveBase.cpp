#include "iser/CTextWriteArchiveBase.h"


namespace iser
{


bool CTextWriteArchiveBase::Process(bool& value)
{
	QByteArray elementText = value? "true": "false";

	return Process(elementText);
}


bool CTextWriteArchiveBase::Process(char& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(quint8& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(qint8& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(quint16& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(qint16& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(quint32& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(qint32& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(quint64& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(qint64& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(float& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(double& value)
{
	QString text = QString::number(value, 'g', 12);

	return Process(text);
}


bool CTextWriteArchiveBase::ProcessData(void* dataPtr, int size)
{
	QByteArray encodedString = QByteArray((const char*)dataPtr, size).toBase64();

	return Process(encodedString);
}


// protected methods

CTextWriteArchiveBase::CTextWriteArchiveBase(const IVersionInfo* versionInfoPtr)
:	BaseClass(versionInfoPtr)
{
}


} // namespace iser


