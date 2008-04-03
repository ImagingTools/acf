#include "iser/CBinaryWriteArchiveBase.h"


#include "istd/CString.h"

#include "iser/CArchiveTag.h"


namespace iser
{


// reimplemented (iser::IArchive)

bool CBinaryWriteArchiveBase::BeginTag(const CArchiveTag& tag)
{
	I_DWORD binaryId = tag.GetBinaryId();

	return Process(binaryId);
}


bool CBinaryWriteArchiveBase::EndTag(const CArchiveTag& tag)
{
	I_DWORD binaryId = ~tag.GetBinaryId();

	return Process(binaryId);
}


bool CBinaryWriteArchiveBase::Process(bool& value)
{
	return ProcessData(&value, sizeof(bool));
}


bool CBinaryWriteArchiveBase::Process(char& value)
{
	return ProcessData(&value, sizeof(char));
}


bool CBinaryWriteArchiveBase::Process(I_BYTE& value)
{
	return ProcessData(&value, sizeof(I_BYTE));
}


bool CBinaryWriteArchiveBase::Process(I_SBYTE& value)
{
	return ProcessData(&value, sizeof(I_SBYTE));
}


bool CBinaryWriteArchiveBase::Process(I_WORD& value)
{
	return ProcessData(&value, sizeof(I_WORD));
}


bool CBinaryWriteArchiveBase::Process(I_SWORD& value)
{
	return ProcessData(&value, sizeof(I_SWORD));
}


bool CBinaryWriteArchiveBase::Process(I_DWORD& value)
{
	return ProcessData(&value, sizeof(I_DWORD));
}


bool CBinaryWriteArchiveBase::Process(I_SDWORD& value)
{
	return ProcessData(&value, sizeof(I_SDWORD));
}


bool CBinaryWriteArchiveBase::Process(I_QWORD& value)
{
	return ProcessData(&value, sizeof(I_QWORD));
}


bool CBinaryWriteArchiveBase::Process(I_SQWORD& value)
{
	return ProcessData(&value, sizeof(I_SQWORD));
}


bool CBinaryWriteArchiveBase::Process(float& value)
{
	return ProcessData(&value, sizeof(float));
}


bool CBinaryWriteArchiveBase::Process(double& value)
{
	return ProcessData(&value, sizeof(double));
}


bool CBinaryWriteArchiveBase::Process(std::string& value)
{			
	int size = value.size() * sizeof(char);

	bool retVal = Process(size);

	retVal = retVal && ProcessData((void*)value.c_str(), size);

	return retVal;
}


bool CBinaryWriteArchiveBase::Process(istd::CString& value)
{
	int size = value.size() * sizeof(wchar_t);

	bool retVal = Process(size);

	retVal = retVal && ProcessData((void*)value.c_str(), size);	

	return retVal;
}


// protected methods

CBinaryWriteArchiveBase::CBinaryWriteArchiveBase(const IVersionInfo* versionInfoPtr)
:	BaseClass(versionInfoPtr)
{
}


} // namespace iser


