#ifndef iser_CBinaryReadArchiveBase_included
#define iser_CBinaryReadArchiveBase_included


// ACF includes
#include "istd/itr.h"
#include "iser/CReadArchiveBase.h"


namespace iser
{


/**
	Base class for archive implementations reading data based on its binary representation.
*/
class CBinaryReadArchiveBase: public CReadArchiveBase
{
public:
	typedef CReadArchiveBase BaseClass;

	enum MessageId
	{
		MI_TAG_ERROR = 0x3f320b0,
		MI_STRING_TOO_LONG
	};

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
	virtual bool Process(QString& value);

protected:
	enum{
		MaxStringLength = 100000
	};
};


} // namespace iser


#endif // !iser_CBinaryReadArchiveBase_included

