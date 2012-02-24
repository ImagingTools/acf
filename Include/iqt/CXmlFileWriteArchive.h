#ifndef iqt_CXmlFileWriteArchive_included
#define iqt_CXmlFileWriteArchive_included


#include <QDomDocument>
#include <QDomNode>
#include <QFile>

#include "iser/CWriteArchiveBase.h"
#include "iser/CFileArchiveInfo.h"
#include "iser/CXmlDocumentInfoBase.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Qt-based implementation of archive for writing in XML format.
	Please note that it doesn't create \c counter attribute needed by \c iser::CXmlFileReadArchive.

	\ingroup Persistence
*/
class CXmlFileWriteArchive:
			public iser::CWriteArchiveBase,
			public iser::CFileArchiveInfo,
			public iser::CXmlDocumentInfoBase
{
public:
	typedef iser::CWriteArchiveBase BaseClass;
	typedef iser::CFileArchiveInfo BaseClass2;

	CXmlFileWriteArchive(
				const QString& filePath = "",
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);
	~CXmlFileWriteArchive();

	bool Flush();

	bool OpenDocument(const QString& filePath);

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
	virtual bool Process(QString& value);
	virtual bool ProcessData(void* dataPtr, int size);

protected:
	/**
		Find the next text node and move the current node to the next sibling.
	*/
	bool PushTextNode(const QString& text);

private:
	QDomDocument m_document;
	QDomElement m_currentParent;

	QFile m_file;

	bool m_serializeHeader;
	iser::CArchiveTag m_rootTag;

	bool m_isSeparatorNeeded;
};


} // namespace iqt


#endif // !iqt_CXmlFileWriteArchive_included


