#ifndef iqt_CXmlFileWriteArchive_included
#define iqt_CXmlFileWriteArchive_included


#include <QDomDocument>
#include <QDomNode>
#include <QFile>

#include "iser/CWriteArchiveBase.h"
#include "iser/CXmlDocumentInfoBase.h"

#include "iqt/iqt.h"


namespace iqt
{


class CXmlFileWriteArchive: public iser::CWriteArchiveBase, public iser::CXmlDocumentInfoBase
{
public:
	typedef iser::CWriteArchiveBase BaseClass;

	CXmlFileWriteArchive(
				const istd::CString& fileName = "",
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);
	~CXmlFileWriteArchive();

	bool Flush();

	bool OpenDocument(const istd::CString& fileName);

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
	bool PushTextNode(const QString& text);

private:
	QDomDocument m_document;
	QDomElement m_currentParent;

	QFile m_file;

	iser::CArchiveTag m_rootTag;
	bool m_serializeHeader;

	bool m_isSeparatorNeeded;
};


} // namespace iqt


#endif // !iqt_CXmlFileWriteArchive_included


