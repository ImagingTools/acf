#ifndef iser_CXmlStreamWriteArchiveBase_included
#define iser_CXmlStreamWriteArchiveBase_included


// Qt includes
#include <QtCore/QTextStream>

// ACF includes
#include "iser/CXmlWriteArchiveBase.h"


namespace iser
{


/**
	Base implementation of archive for writing in XML format using a stream implementation.
*/
class CXmlStreamWriteArchiveBase: public CXmlWriteArchiveBase
{
public:
	typedef CXmlWriteArchiveBase BaseClass;

	virtual ~CXmlStreamWriteArchiveBase();

protected:
	CXmlStreamWriteArchiveBase(const IVersionInfo* versionInfoPtr, const CArchiveTag& rootTag);

	// reimplemented (iser::CXmlWriteArchiveBase)
	virtual bool WriteString(const QByteArray& value);

protected:
	QTextStream m_stream;
};


} // namespace iser


#endif // !iser_CXmlStreamWriteArchiveBase_included


