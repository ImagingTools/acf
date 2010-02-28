#ifndef iser_CTextWriteArchiveBase_included
#define iser_CTextWriteArchiveBase_included


// STL includes
#include <sstream>


// ACF includes
#include "iser/CWriteArchiveBase.h"


namespace iser
{


/**
	Common archive implementation of text-based storing archives.

	\ingroup Persistence
*/
class CTextWriteArchiveBase: public CWriteArchiveBase
{
public:
	typedef CWriteArchiveBase BaseClass;

	using BaseClass::Process;

	// reimplemented (iser::IArchive)
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
	CTextWriteArchiveBase(const IVersionInfo* versionInfoPtr);

	// template methods
	template <typename Type>
	bool ProcessInternal(const Type& value);
};


// template methods

template <typename Type>
bool CTextWriteArchiveBase::ProcessInternal(const Type& value)
{
	bool retVal = true;

	std::ostringstream stream;

	stream << value;

	std::string str(stream.str());

	retVal = retVal && Process(str);

	return retVal;
}


} // namespace iser


#endif // !iser_CTextWriteArchiveBase_included


