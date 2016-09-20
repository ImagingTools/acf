#include <iser/CTextWriteArchiveBase.h>


namespace iser
{


bool CTextWriteArchiveBase::Process(bool& value)
{
	return WriteTextNode(value? "true": "false");
}


bool CTextWriteArchiveBase::Process(char& value)
{
	return WriteTextNode(QByteArray::number(value));
}


bool CTextWriteArchiveBase::Process(quint8& value)
{
	return WriteTextNode(QByteArray::number(value));
}


bool CTextWriteArchiveBase::Process(qint8& value)
{
	return WriteTextNode(QByteArray::number(value));
}


bool CTextWriteArchiveBase::Process(quint16& value)
{
	return WriteTextNode(QByteArray::number(value));
}


bool CTextWriteArchiveBase::Process(qint16& value)
{
	return WriteTextNode(QByteArray::number(value));
}


bool CTextWriteArchiveBase::Process(quint32& value)
{
	return WriteTextNode(QByteArray::number(value));
}


bool CTextWriteArchiveBase::Process(qint32& value)
{
	return WriteTextNode(QByteArray::number(value));
}


bool CTextWriteArchiveBase::Process(quint64& value)
{
	return WriteTextNode(QByteArray::number(value));
}


bool CTextWriteArchiveBase::Process(qint64& value)
{
	return WriteTextNode(QByteArray::number(value));
}


bool CTextWriteArchiveBase::Process(float& value)
{
	return WriteTextNode(QByteArray::number(value));
}


bool CTextWriteArchiveBase::Process(double& value)
{
	return WriteTextNode(QByteArray::number(value, 'g', 12));
}


bool CTextWriteArchiveBase::Process(QByteArray& value)
{
	return WriteTextNode(value);
}


bool CTextWriteArchiveBase::ProcessData(void* dataPtr, int size)
{
	QByteArray encodedString = QByteArray((const char*)dataPtr, size).toBase64();

	return WriteTextNode(encodedString);
}


// protected methods

CTextWriteArchiveBase::CTextWriteArchiveBase(const IVersionInfo* versionInfoPtr)
:	BaseClass(versionInfoPtr)
{
}


} // namespace iser


