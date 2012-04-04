#ifndef iser_CBinaryReadArchiveBase_included
#define iser_CBinaryReadArchiveBase_included


// ACF includes
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
	virtual bool Process(quint8& value);
	virtual bool Process(qint8& value);
	virtual bool Process(quint16& value);
	virtual bool Process(qint16& value);
	virtual bool Process(quint32& value);
	virtual bool Process(qint32& value);
	virtual bool Process(quint64& value);
	virtual bool Process(qint64& value);
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

