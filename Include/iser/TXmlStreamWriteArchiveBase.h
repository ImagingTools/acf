#ifndef iser_TXmlStreamWriteArchiveBase_included
#define iser_TXmlStreamWriteArchiveBase_included


#include "iser/CXmlWriteArchiveBase.h"


namespace iser
{


/**
	Base implementation of archive for writing in XML format using a stream implementation.
*/
template <class StreamClass>
class TXmlStreamWriteArchiveBase: public CXmlWriteArchiveBase
{
public:
	typedef CXmlWriteArchiveBase BaseClass;

	virtual ~TXmlStreamWriteArchiveBase();

protected:
	TXmlStreamWriteArchiveBase(const IVersionInfo* versionInfoPtr, const iser::CArchiveTag& rootTag);

	// reimplemented (iser::CXmlWriteArchiveBase)
	virtual bool WriteString(const std::string& value);

protected:
	StreamClass m_stream;
};


// public methods

template <class StreamClass>
TXmlStreamWriteArchiveBase<StreamClass>::~TXmlStreamWriteArchiveBase()
{
	Flush();
}


// protected methods

template <class StreamClass>
TXmlStreamWriteArchiveBase<StreamClass>::TXmlStreamWriteArchiveBase(const IVersionInfo* versionInfoPtr, const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr, rootTag)
{
}


// reimplemented (iser::CXmlWriteArchiveBase)

template <class StreamClass>
bool TXmlStreamWriteArchiveBase<StreamClass>::WriteString(const std::string& value)
{
	m_stream << value;

	return !m_stream.fail();
}


} // namespace iser


#endif // !iser_TXmlStreamWriteArchiveBase_included


