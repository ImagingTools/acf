#ifndef iser_CBinaryWriteArchiveBase_included
#define iser_CBinaryWriteArchiveBase_included


// ACF includes
#include "iser/CWriteArchiveBase.h"


namespace iser
{


/**
	Base class for archive implementations writting data based on its binary representation.

	\ingroup Persistence
*/
class CBinaryWriteArchiveBase: public CWriteArchiveBase
{
public:
	typedef CWriteArchiveBase BaseClass;

	// reimplemented (iser::IArchive)
	virtual bool BeginTag(const CArchiveTag& tag);
	virtual bool EndTag(const CArchiveTag& tag);
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
	virtual bool Process(std::string& value);
	virtual bool Process(istd::CString& value);

protected:
	CBinaryWriteArchiveBase(const IVersionInfo* versionInfoPtr);
};


} // namespace iser


#endif // !iser_CBinaryWriteArchiveBase_included

