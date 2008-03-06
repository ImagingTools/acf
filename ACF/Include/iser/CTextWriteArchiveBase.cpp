#include "iser/CTextWriteArchiveBase.h"


#include <strstream>


namespace iser
{


bool CTextWriteArchiveBase::Process(bool& value)
{
	return ProcessInternal(value);
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
	I_BYTE* data = (I_BYTE*)dataPtr;

	::std::ostrstream stream;

	stream << ::std::hex;

	for (int i = 0; i < size; i++){
		stream << data[i];

		if (i < size - 1){
			stream << " ";
		}

		if ((i % 32) == 31){
			stream << ::std::endl;
		}
	}

	stream << ::std::endl;

	::std::string str(stream.str());

	return Process(str);
}


// protected methods

CTextWriteArchiveBase::CTextWriteArchiveBase(const IVersionInfo* versionInfoPtr)
:	BaseClass(versionInfoPtr)
{
}


} // namespace iser


