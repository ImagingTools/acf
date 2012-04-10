#ifndef iser_CTextWriteArchiveBase_included
#define iser_CTextWriteArchiveBase_included


// Qt includes
#include <QtCore/QTextStream>
#include <QtCore/QIODevice>

// ACF includes
#include "iser/CWriteArchiveBase.h"


namespace iser
{


/**
	Common archive implementation of text-based storing archives.
*/
class CTextWriteArchiveBase: public CWriteArchiveBase
{
public:
	typedef CWriteArchiveBase BaseClass;

	using BaseClass::Process;

	// reimplemented (iser::IArchive)
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
	CTextWriteArchiveBase(const IVersionInfo* versionInfoPtr);

	// template methods
	template <typename Type>
	bool ProcessInternal(const Type& value);
};


// template methods

template <typename Type>
bool CTextWriteArchiveBase::ProcessInternal(const Type& value)
{
	QByteArray string;

	{
		QTextStream stream(&string,  QIODevice::WriteOnly);

		stream << value;
	}

	return Process(string);
}


} // namespace iser


#endif // !iser_CTextWriteArchiveBase_included


