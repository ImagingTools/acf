#ifndef iser_CTextReadArchiveBase_included
#define iser_CTextReadArchiveBase_included


// Qt includes
#include <QtCore/QTextStream>

// ACF includes
#include <iser/CReadArchiveBase.h>


namespace iser
{


/**
	Common archive implementation of text-based reading archives.
*/
class CTextReadArchiveBase: public CReadArchiveBase
{
public:
	typedef CReadArchiveBase BaseClass;

	// reimplemented (iser::IArchive)
	using BaseClass::Process;
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
	/**
		Read single unformatted text node.
	*/
	virtual bool ReadTextNode(QByteArray& text) = 0;
};


} // namespace iser


#endif // !iser_CTextReadArchiveBase_included


