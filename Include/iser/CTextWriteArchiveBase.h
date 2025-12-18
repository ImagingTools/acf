#ifndef iser_CTextWriteArchiveBase_included
#define iser_CTextWriteArchiveBase_included


// Qt includes
#include <QtCore/QTextStream>
#include <QtCore/QIODevice>

// ACF includes
#include <iser/CWriteArchiveBase.h>


namespace iser
{


/**
	Common archive implementation of text-based storing archives.
*/
class CTextWriteArchiveBase: public CWriteArchiveBase
{
public:
	typedef CWriteArchiveBase BaseClass;

	// reimplemented (iser::IArchive)
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
	virtual bool ProcessData(void* dataPtr, int size) override;

protected:
	explicit CTextWriteArchiveBase(const IVersionInfo* versionInfoPtr);

	/**
		Write single unformatted text node.
	*/
	virtual bool WriteTextNode(const QByteArray& text) = 0;
};


} // namespace iser


#endif // !iser_CTextWriteArchiveBase_included


