#ifndef idoc_TMultiPageDocumentWrap_included
#define idoc_TMultiPageDocumentWrap_included


// Qt includes
#include <QtCore/QList>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/TSmartPtr.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <iprm/IOptionsList.h>
#include <idoc/IMultiPageDocument.h>
#include <idoc/IMultiPageDocumentProvider.h>
#include <idoc/CStandardDocumentMetaInfo.h>


namespace idoc
{


/**
	Generic implementation of IMultiPageDocument interface.
*/
template <class Base>
class TMultiPageDocumentWrap:
			virtual public Base,
			public CStandardDocumentMetaInfo,
			virtual public iprm::IOptionsList
{
public:
	typedef Base BaseClass;
	typedef CStandardDocumentMetaInfo BaseClass2;

	// pseudo-reimplemented (idoc::IMultiPageDocument)
	virtual int GetPagesCount() const;
	virtual const istd::IChangeable& GetDocumentPage(int pageIndex) const;
	virtual const idoc::IDocumentMetaInfo* GetPageMetaInfo(int pageIndex) const;
	virtual void ResetPages();
	virtual bool RemovePage(int pageIndex);
	virtual const IDocumentMetaInfo& GetDocumentMetaInfo() const;

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;
	virtual bool IsOptionEnabled(int index) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual bool ResetData(CompatibilityMode mode);

protected:
	struct Page
	{
		idoc::CStandardDocumentMetaInfo pageMetaInfo;
		istd::TSmartPtr<istd::IChangeable> pagePtr;
	};

protected:
	/**
		Serialize a single page item into or from the archive.
	*/
	virtual bool SerializePage(iser::IArchive& archive, Page& pageItem);

protected:
	typedef QList<Page> Pages;

	Pages m_documentPages;
};


// public methods

// pseudo-reimplemented (idoc::IMultiPageDocument)

template <class Base>
int TMultiPageDocumentWrap<Base>::GetPagesCount() const
{
	return m_documentPages.count();
}


template <class Base>
const istd::IChangeable& TMultiPageDocumentWrap<Base>::GetDocumentPage(int pageIndex) const
{
	Q_ASSERT(pageIndex < m_documentPages.count());
	Q_ASSERT(pageIndex >= 0);
	Q_ASSERT(m_documentPages.at(pageIndex).pagePtr.IsValid());

	return *m_documentPages.at(pageIndex).pagePtr.GetPtr();
}


template <class Base>
const idoc::IDocumentMetaInfo* TMultiPageDocumentWrap<Base>::GetPageMetaInfo(int pageIndex) const
{
	Q_ASSERT(pageIndex < m_documentPages.count());
	Q_ASSERT(pageIndex >= 0);

	return &m_documentPages.at(pageIndex).pageMetaInfo;
}


template <class Base>
void TMultiPageDocumentWrap<Base>::ResetPages()
{
	istd::CChangeNotifier changePtr(this);

	m_documentPages.clear();
}


template <class Base>
bool TMultiPageDocumentWrap<Base>::RemovePage(int pageIndex)
{
	Q_ASSERT(pageIndex < m_documentPages.count());
	Q_ASSERT(pageIndex >= 0);

	istd::CChangeNotifier changePtr(this);

	m_documentPages.removeAt(pageIndex);

	return true;
}


template <class Base>
const IDocumentMetaInfo& TMultiPageDocumentWrap<Base>::GetDocumentMetaInfo() const
{
	return *this;
}


// reimplemented (iprm::IOptionsList)

template <class Base>
int TMultiPageDocumentWrap<Base>::GetOptionsFlags() const
{
	return SCF_NONE;
}


template <class Base>
int TMultiPageDocumentWrap<Base>::GetOptionsCount() const
{
	return GetPagesCount();
}


template <class Base>
QString TMultiPageDocumentWrap<Base>::GetOptionName(int index) const
{
	Q_ASSERT(index < m_documentPages.count());
	Q_ASSERT(index>= 0);

	return m_documentPages[index].pageMetaInfo.GetMetaInfo(idoc::IDocumentMetaInfo::MIT_TITLE).toString();
}


template <class Base>
QString TMultiPageDocumentWrap<Base>::GetOptionDescription(int /*index*/) const
{
	return QString();
}


template <class Base>
QByteArray TMultiPageDocumentWrap<Base>::GetOptionId(int index) const
{
	return GetOptionName(index).toUtf8();
}


template <class Base>
bool TMultiPageDocumentWrap<Base>::IsOptionEnabled(int /*index*/) const
{
	return true;
}


// reimplemented (iser::ISerializable)

template <class Base>
bool TMultiPageDocumentWrap<Base>::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag metaInfoTag("MetaInfo", "Meta information about the document", iser::CArchiveTag::TT_GROUP);
	static iser::CArchiveTag pagesTag("Pages", "Container of the document pages", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag pageTag("Page", "Single document page", iser::CArchiveTag::TT_GROUP, &pagesTag);

	// Serialize meta info:
	bool retVal = archive.BeginTag(metaInfoTag);
	retVal = retVal && BaseClass2::Serialize(archive);
	retVal = retVal && archive.EndTag(metaInfoTag);

	// Serialize document pages:
	int pagesCount = m_documentPages.count();

	retVal = retVal && archive.BeginMultiTag(pagesTag, pageTag, pagesCount);

	if (archive.IsStoring()){
		for (int pageIndex = 0; pageIndex < pagesCount; ++pageIndex){
			retVal = retVal && archive.BeginTag(pageTag);
			retVal = retVal && SerializePage(archive, m_documentPages[pageIndex]);
			retVal = retVal && archive.EndTag(pageTag);
		}
	}
	else{
		istd::CChangeNotifier changePtr(this);

		m_documentPages.clear();

		for (int pageIndex = 0; pageIndex < pagesCount; ++pageIndex){
			this->InsertPage(NULL, NULL, pageIndex);

			retVal = retVal && archive.BeginTag(pageTag);
			retVal = retVal && SerializePage(archive, m_documentPages[pageIndex]);
			retVal = retVal && archive.EndTag(pageTag);
		}
	}

	retVal = retVal && archive.EndTag(pagesTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

template <class Base>
bool TMultiPageDocumentWrap<Base>::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
{
	// native copy
	const TMultiPageDocumentWrap<Base>* sourcePtr = dynamic_cast<const TMultiPageDocumentWrap<Base>*>(&object);
	if (sourcePtr != NULL){
		istd::CChangeNotifier changePtr(this);

		m_documentPages.clear();

		for (int pageIndex = 0; pageIndex < sourcePtr->m_documentPages.count(); ++pageIndex){
			istd::IChangeable* pagePtr = this->InsertPage();
			if (pagePtr == NULL){
				return false;
			}

			const istd::IChangeable* sourcePagePtr = sourcePtr->m_documentPages.at(pageIndex).pagePtr.GetPtr();
			Q_ASSERT(sourcePagePtr != NULL);

			if (!pagePtr->CopyFrom(*sourcePagePtr)){
				return false;
			}

			if (!m_documentPages[pageIndex].pageMetaInfo.CopyFrom(sourcePtr->m_documentPages.at(pageIndex).pageMetaInfo)){
				return false;
			}
		}

		if (!BaseClass2::CopyFrom(object, mode)){
			return false;
		}

		return true;
	}

	// copy via idoc::IMultiPageDocument
	const idoc::IMultiPageDocument* docPtr = dynamic_cast<const idoc::IMultiPageDocument*>(&object);
	if (docPtr != NULL){
		istd::CChangeNotifier changePtr(this);

		m_documentPages.clear();

		int pagesCount = docPtr->GetPagesCount();
		for (int pageIndex = 0; pageIndex < pagesCount; ++pageIndex){
			istd::IChangeable* pagePtr = this->InsertPage();
			if (pagePtr == NULL){
				return false;
			}

			const istd::IChangeable& sourcePage = docPtr->GetDocumentPage(pageIndex);
			if (!pagePtr->CopyFrom(sourcePage)){
				return false;
			}

			const idoc::IDocumentMetaInfo* metaInfoPtr = docPtr->GetPageMetaInfo(pageIndex);
			if (metaInfoPtr != NULL){
				if (!m_documentPages[pageIndex].pageMetaInfo.CopyFrom(*metaInfoPtr)){
					return false;
				}
			}
		}

		if (!BaseClass2::CopyFrom(object, mode)){
			return false;
		}

		return true;
	}

	// copy via idoc::IMultiPageDocumentProvider
	const idoc::IMultiPageDocumentProvider* docProviderPtr = dynamic_cast<const idoc::IMultiPageDocumentProvider*>(&object);
	if (docProviderPtr != NULL){
		const idoc::IMultiPageDocument* docPtr = docProviderPtr->GetDocument();
		if (docPtr != NULL){	
			// recursive call
			return CopyFrom(*docPtr, mode);
		}
	}

	return false;
}


template <class Base>
bool TMultiPageDocumentWrap<Base>::ResetData(CompatibilityMode /*mode*/)
{
	ResetPages();

	return true;
}


// protected methods

template <class Base>
bool TMultiPageDocumentWrap<Base>::SerializePage(iser::IArchive& archive, Page& pageItem)
{
	static iser::CArchiveTag pageTitleTag("PageTitle", "Title of the page", iser::CArchiveTag::TT_GROUP);
	static iser::CArchiveTag pageContentTag("PageContent", "The contents of the page", iser::CArchiveTag::TT_GROUP);

	bool retVal = archive.BeginTag(pageTitleTag);
	retVal = retVal && pageItem.pageMetaInfo.Serialize(archive);
	retVal = retVal && archive.EndTag(pageTitleTag);

	iser::ISerializable* serializablePagePtr = dynamic_cast<iser::ISerializable*>(pageItem.pagePtr.GetPtr());
	if (serializablePagePtr != NULL){
		retVal = retVal && archive.BeginTag(pageContentTag);
		retVal = serializablePagePtr->Serialize(archive);
		retVal = retVal && archive.EndTag(pageContentTag);
	}
	else{
		qWarning("Page object doesn't support serialization");

		return false;
	}

	return retVal;
}


typedef idoc::TMultiPageDocumentWrap<idoc::IMultiPageDocument> CMultiPageDocumentBase;


} // namespace idoc


#endif // !idoc_TMultiPageDocumentWrap_included


