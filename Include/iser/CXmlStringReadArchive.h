#ifndef iser_CXmlStringReadArchive_included
#define iser_CXmlStringReadArchive_included


// Qt includes
#include <QtCore/QBuffer>

// ACF includes
#include <iser/CXmlStreamReadArchiveBase.h>


namespace iser
{


/**
	Simple implementation for reading of a XML from a string.
	This imlementation uses only single pass and is very fast,
	but it needs \c counter attribute idicating number of subtags for each mutli tag node.

	\ingroup Persistence
*/
class CXmlStringReadArchive: public CXmlStreamReadArchiveBase
{
public:
	typedef CXmlStreamReadArchiveBase BaseClass;

	CXmlStringReadArchive(const QByteArray& inputString, bool serializeHeader = true, const CArchiveTag& rootTag = s_acfRootTag);

private:
	QBuffer m_buffer;
};


} // namespace iser


#endif // !iser_CXmlStringReadArchive_included

