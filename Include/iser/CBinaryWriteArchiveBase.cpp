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
	return ProcessData(&value, int(sizeof(bool)));
}


bool CBinaryWriteArchiveBase::Process(char& value)
{
	return ProcessData(&value, int(sizeof(char)));
}


bool CBinaryWriteArchiveBase::Process(I_BYTE& value)
{
	return ProcessData(&value, int(sizeof(I_BYTE)));
}


bool CBinaryWriteArchiveBase::Process(I_SBYTE& value)
{
	return ProcessData(&value, int(sizeof(I_SBYTE)));
}


bool CBinaryWriteArchiveBase::Process(I_WORD& value)
{
	return ProcessData(&value, int(sizeof(I_WORD)));
}


bool CBinaryWriteArchiveBase::Process(I_SWORD& value)
{
	return ProcessData(&value, int(sizeof(I_SWORD)));
}


bool CBinaryWriteArchiveBase::Process(I_DWORD& value)
{
	return ProcessData(&value, int(sizeof(I_DWORD)));
}


bool CBinaryWriteArchiveBase::Process(I_SDWORD& value)
{
	return ProcessData(&value, int(sizeof(I_SDWORD)));
}


bool CBinaryWriteArchiveBase::Process(I_QWORD& value)
{
	return ProcessData(&value, int(sizeof(I_QWORD)));
}


bool CBinaryWriteArchiveBase::Process(I_SQWORD& value)
{
	return ProcessData(&value, int(sizeof(I_SQWORD)));
}


bool CBinaryWriteArchiveBase::Process(float& value)
{
	return ProcessData(&value, int(sizeof(float)));
}


bool CBinaryWriteArchiveBase::Process(double& value)
{
	return ProcessData(&value, int(sizeof(double)));
}


bool CBinaryWriteArchiveBase::Process(std::string& value)
{			
	int stringLength = value.size();

	bool retVal = Process(stringLength);

	retVal = retVal && ProcessData((void*)value.c_str(), stringLength * int(sizeof(char)));

	return retVal;
}


bool CBinaryWriteArchiveBase::Process(istd::CString& value)
{
	int stringLength = value.size();

	bool retVal = Process(stringLength);

	retVal = retVal && ProcessData((void*)value.c_str(), stringLength * int(sizeof(wchar_t)));	

	return retVal;
}


// protected methods

CBinaryWriteArchiveBase::CBinaryWriteArchiveBase(const IVersionInfo* versionInfoPtr)
:	BaseClass(versionInfoPtr)
{
}


} // namespace iser


