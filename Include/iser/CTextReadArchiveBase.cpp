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
	std::string elementText;

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


bool CTextReadArchiveBase::Process(I_BYTE& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::Process(I_SBYTE& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::Process(I_WORD& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::Process(I_SWORD& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::Process(I_DWORD& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::Process(I_SDWORD& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::Process(I_QWORD& value)
{
	return ProcessInternal(value);
}


bool CTextReadArchiveBase::Process(I_SQWORD& value)
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
	I_BYTE* data = (I_BYTE*)dataPtr;

	std::string text;
	bool retVal = Process(text);

	if (retVal){
		std::vector<I_BYTE> decodedData = istd::CBase64::ConvertFromBase64(text);
		I_ASSERT(size == int(decodedData.size()));

		std::memcpy(data, &decodedData[0], size);
	}

	return retVal;
}


} // namespace iser


