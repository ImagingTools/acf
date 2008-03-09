#include "iser/CTextReadArchiveBase.h"


#include <strstream>


namespace iser
{


// reimplemented (iser::IArchive)

bool CTextReadArchiveBase::Process(bool& value)
{
	::std::string elementText;

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

	::std::string text;
	bool retVal = Process(text);

	if (retVal){
		::std::istrstream stream(text.c_str());

		stream >> ::std::hex;

		for (int i = 0; i < size; i++){
			stream >> data[i];
		}
	}

	return retVal;
}


} // namespace iser


