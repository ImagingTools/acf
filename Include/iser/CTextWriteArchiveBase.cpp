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


bool CTextWriteArchiveBase::Process(I_BYTE& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(I_SBYTE& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(I_WORD& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(I_SWORD& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(I_DWORD& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(I_SDWORD& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(I_QWORD& value)
{
	return ProcessInternal(value);
}


bool CTextWriteArchiveBase::Process(I_SQWORD& value)
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


