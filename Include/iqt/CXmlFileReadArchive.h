#ifndef iqt_CXmlFileReadArchive_included
#define iqt_CXmlFileReadArchive_included


#include <QDomDocument>
#include <QDomNode>


#include "iser/CXmlDocumentInfoBase.h"
#include "iser/CReadArchiveBase.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Qt-based implementation of archive reading from XML file.

	\ingroup Persistence
*/
class CXmlFileReadArchive: public iser::CReadArchiveBase, public iser::CXmlDocumentInfoBase
{
public:
	CXmlFileReadArchive(
				const istd::CString& filePath = "",
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);

	bool OpenDocument(const istd::CString& filePath);

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool Process(bool& value);
	virtual bool Process(char& value);
	virtual bool Process(I_BYTE& value);
	virtual bool Process(I_SBYTE& value);
	virtual bool Process(I_WORD& value);
	virtual bool Process(I_SWORD& value);
	virtual bool Process(I_DWORD& value);
	virtual bool Process(I_SDWORD& value);
	virtual bool Process(I_QWORD& value);
	virtual bool Process(I_SQWORD& value);
	virtual bool Process(float& value);
	virtual bool Process(double& value);
	virtual bool Process(std::string& value);
	virtual bool Process(istd::CString& value);
	virtual bool ProcessData(void* dataPtr, int size);

protected:
	/**
		Find the next text node and move the current node to the next sibling.
	*/
	QString PullTextNode();

private:
	QDomDocument m_document;
	QDomNode m_currentNode;

	bool m_serializeHeader;
	iser::CArchiveTag m_rootTag;
};


} // namespace iqt


#endif // !iqt_CXmlFileReadArchive_included


