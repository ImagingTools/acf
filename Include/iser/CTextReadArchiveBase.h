#ifndef iser_CTextReadArchiveBase_included
#define iser_CTextReadArchiveBase_included


// Qt includes
#include <QtCore/QTextStream>

// ACF includes
#include "iser/CReadArchiveBase.h"


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
	QByteArray elementText;

	if (Process(elementText) && !elementText.isEmpty()){
		QTextStream stream(&elementText, QIODevice::ReadOnly);

		stream >> value;

		return true;
	}

	return false;
}


} // namespace iser


#endif // !iser_CTextReadArchiveBase_included


