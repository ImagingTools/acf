#include "iser/CTextWriteArchiveBase.h"


// ACF includes
#include "istd/CBase64.h"


namespace iser
{


bool CTextWriteArchiveBase::Process(bool& value)
{
	std::string elementText = value? "true": "false";

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
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::ProcessData(void* dataPtr, int size)
{
	std::string encodedString = istd::CBase64::ConvertToBase64(dataPtr, size);

	return Process(encodedString);
}


// protected methods

CTextWriteArchiveBase::CTextWriteArchiveBase(const IVersionInfo* versionInfoPtr)
:	BaseClass(versionInfoPtr)
{
}


} // namespace iser


