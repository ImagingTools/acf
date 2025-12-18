#ifndef iser_CBinaryReadArchiveBase_included
#define iser_CBinaryReadArchiveBase_included


// ACF includes
#include <iser/CReadArchiveBase.h>


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
		MI_STRING_TOO_LONG = 0x3f320b0
	};

	// reimplemented (iser::IArchive)
	virtual bool BeginTag(const CArchiveTag& tag) override;
	virtual bool EndTag(const CArchiveTag& tag) override;
	virtual bool Process(bool& value) override;
	virtual bool Process(char& value) override;
	virtual bool Process(quint8& value) override;
	virtual bool Process(qint8& value) override;
	virtual bool Process(quint16& value) override;
	virtual bool Process(qint16& value) override;
	virtual bool Process(quint32& value) override;
	virtual bool Process(qint32& value) override;
	virtual bool Process(quint64& value) override;
	virtual bool Process(qint64& value) override;
	virtual bool Process(float& value) override;
	virtual bool Process(double& value) override;
	virtual bool Process(QByteArray& value) override;
	virtual bool Process(QString& value) override;

protected:
	/**
		Get maximal allowed string size.
	*/
	virtual int GetMaxStringLength() const;
};


} // namespace iser


#endif // !iser_CBinaryReadArchiveBase_included

