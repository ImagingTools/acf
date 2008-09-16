#ifndef iser_CTextReadArchiveBase_included
#define iser_CTextReadArchiveBase_included


#include "iser/CReadArchiveBase.h"


namespace iser
{


class CTextReadArchiveBase: public iser::CReadArchiveBase
{
public:
	typedef iser::CReadArchiveBase BaseClass;

	// reimplemented (iser::IArchive)
	using BaseClass::Process;
	virtual bool Process(bool& value);
	virtual bool Process(char& value);
	virtual bool Process(I_BYTE& value);
	virtual bool Process(I_SBYTE& value);
	virtual bool Process(I_WORD& value);
	virtual bool Process(I_SWORD& value);
	virtual bool Process(I_DWORD& value);
	virtual bool Process(I_SDWORD& value);
	virtual bool Process(I_QWORD& value);
	virtual bool Process(I_SQWORD& value);
	virtual bool Process(float& value);
	virtual bool Process(double& value);
	virtual bool ProcessData(void* dataPtr, int size);

protected:
	// template methods
	template <typename Type>
	bool ProcessInternal(Type& value);
};


// template methods

template <typename Type>
bool CTextReadArchiveBase::ProcessInternal(Type& value)
{
	std::string elementText;

	if (Process(elementText) && !elementText.empty()){
		std::istrstream stream(elementText.c_str());

		stream >> value;

		return true;
	}

	return false;
}


} // namespace iser


#endif // !iser_CTextReadArchiveBase_included


