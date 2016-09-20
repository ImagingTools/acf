#ifndef iser_CBinaryWriteArchiveBase_included
#define iser_CBinaryWriteArchiveBase_included


// ACF includes
#include <iser/CWriteArchiveBase.h>


namespace iser
{


/**
	Base class for archive implementations writting data based on its binary representation.
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
	virtual bool Process(QByteArray& value);
	virtual bool Process(QString& value);

protected:
	CBinaryWriteArchiveBase(const IVersionInfo* versionInfoPtr);
};


} // namespace iser


#endif // !iser_CBinaryWriteArchiveBase_included

