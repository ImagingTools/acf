#include "iser/CXmlStreamWriteArchiveBase.h"


namespace iser
{


CXmlStreamWriteArchiveBase::~CXmlStreamWriteArchiveBase()
{
	Flush();
}


// protected methods

CXmlStreamWriteArchiveBase::CXmlStreamWriteArchiveBase(const IVersionInfo* versionInfoPtr, const CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr, rootTag)
{
}


// reimplemented (iser::CXmlWriteArchiveBase)

bool CXmlStreamWriteArchiveBase::WriteString(const QByteArray& value)
{
	m_stream << value.constData();

	return m_stream.status() == QTextStream::Ok;
}


} // namespace iser


