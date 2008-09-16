#include "iser/CBinaryReadArchiveBase.h"


#include <assert.h>

#include "istd/CString.h"

#include "iser/CArchiveTag.h"


namespace iser
{


bool CBinaryReadArchiveBase::BeginTag(const CArchiveTag& tag)
{
	I_DWORD readId;
	bool retVal = Process(readId);

	return retVal && (readId == tag.GetBinaryId());
}


bool CBinaryReadArchiveBase::EndTag(const CArchiveTag& tag)
{
	I_DWORD readId;
	bool retVal = Process(readId);

	return retVal && (~readId == tag.GetBinaryId());
}


bool CBinaryReadArchiveBase::Process(bool& value)
{
	return ProcessData(&value, sizeof(bool));
}


bool CBinaryReadArchiveBase::Process(char& value)
{
	return ProcessData(&value, sizeof(char));
}


bool CBinaryReadArchiveBase::Process(I_BYTE& value)
{
	return ProcessData(&value, sizeof(I_BYTE));
}


bool CBinaryReadArchiveBase::Process(I_SBYTE& value)
{
	return ProcessData(&value, sizeof(I_SBYTE));
}


bool CBinaryReadArchiveBase::Process(I_WORD& value)
{
	return ProcessData(&value, sizeof(I_WORD));
}


bool CBinaryReadArchiveBase::Process(I_SWORD& value)
{
	return ProcessData(&value, sizeof(I_SWORD));
}


bool CBinaryReadArchiveBase::Process(I_DWORD& value)
{
	return ProcessData(&value, sizeof(I_DWORD));
}


bool CBinaryReadArchiveBase::Process(I_SDWORD& value)
{
	return ProcessData(&value, sizeof(I_SDWORD));
}


bool CBinaryReadArchiveBase::Process(I_QWORD& value)
{
	return ProcessData(&value, sizeof(I_QWORD));
}


bool CBinaryReadArchiveBase::Process(I_SQWORD& value)
{
	return ProcessData(&value, sizeof(I_SQWORD));
}


bool CBinaryReadArchiveBase::Process(float& value)
{
	return ProcessData(&value, sizeof(float));
}


bool CBinaryReadArchiveBase::Process(double& value)
{
	return ProcessData(&value, sizeof(double));
}


bool CBinaryReadArchiveBase::Process(std::string& value)
{			
	int stringLength;

	bool retVal = Process(stringLength);

	if (retVal && (stringLength > 0)){
		if (stringLength > MaxStringLength){
			return false;
		}

		std::vector<char> buffer(stringLength + 1, 0);

		retVal = ProcessData(&buffer[0], stringLength * sizeof(char));	

		if (retVal){
			value = std::string(&buffer[0]);
		}
	}
	
	return retVal;
}


bool CBinaryReadArchiveBase::Process(istd::CString& value)
{
	int stringLength;

	bool retVal = Process(stringLength);

	if (retVal && (stringLength > 0)){
		if (stringLength > MaxStringLength){
			return false;
		}

		std::vector<wchar_t> buffer(stringLength + 1, 0);

		retVal = ProcessData(&buffer[0], stringLength * sizeof(wchar_t));	

		if (retVal){
			value = istd::CString(&buffer[0]);
		}
	}

	return retVal;
}


} // namespace iser

