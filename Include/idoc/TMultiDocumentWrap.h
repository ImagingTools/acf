#ifndef idoc_TMultiDocumentWrap_included
#define idoc_TMultiDocumentWrap_included


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TPointerVector.h"
#include "idoc/IMultiPageDocument.h"
#include "idoc/CStandardDocumentMetaInfo.h"


namespace idoc
{


/**
	Generic implementation of IMultiPageDocument interface.
*/
template <class Base, class PageInterface>
class TMultiDocumentWrap:
			virtual public Base,
			public CStandardDocumentMetaInfo
{
public:
	typedef Base BaseClass;
	typedef CStandardDocumentMetaInfo BaseClass2;
	typedef typename PageInterface PageInterfaceType;

	const PageInterfaceType* GetPageObject(int pageIndex) const;

	// pseudo-reimplemented (IMultiPageDocument)
	virtual int GetPagesCount() const;
	virtual const istd::IChangeable& GetDocumentPage(int pageIndex) const;
	virtual void ResetPages();
	virtual bool RemovePage(int pageIndex);
	virtual const IDocumentMetaInfo& GetDocumentMetaInfo() const;

protected:
	typedef istd::TPointerVector<PageInterface> Pages;

	Pages m_documentPages;
};


// public methods

template <class Base, class PageInterface>
const typename TMultiDocumentWrap<Base, PageInterface>::PageInterfaceType* TMultiDocumentWrap<Base, PageInterface>::GetPageObject(int pageIndex) const
{
	Q_ASSERT(pageIndex < m_documentPages.GetCount());
	Q_ASSERT(pageIndex >= 0);

	return m_documentPages.GetAt(pageIndex);
}


// pseudo-reimplemented (IMultiPageDocument)

template <class Base, class PageInterface>
int TMultiDocumentWrap<Base, PageInterface>::GetPagesCount() const
{
	return m_documentPages.GetCount();
}


template <class Base, class PageInterface>
const istd::IChangeable& TMultiDocumentWrap<Base, PageInterface>::GetDocumentPage(int pageIndex) const
{
	return *GetPageObject(pageIndex);
}


template <class Base, class PageInterface>
void TMultiDocumentWrap<Base, PageInterface>::ResetPages()
{
	istd::CChangeNotifier changePtr(this);

	m_documentPages.Reset();
}


template <class Base, class PageInterface>
bool TMultiDocumentWrap<Base, PageInterface>::RemovePage(int pageIndex)
{
	Q_ASSERT(pageIndex < m_documentPages.GetCount());
	Q_ASSERT(pageIndex >= 0);

	istd::CChangeNotifier changePtr(this);

	m_documentPages.RemoveAt(pageIndex);

	return true;
}


template <class Base, class PageInterface>
const IDocumentMetaInfo& TMultiDocumentWrap<Base, PageInterface>::GetDocumentMetaInfo() const
{
	return *this;
}


} // namespace idoc


#endif // !idoc_TMultiDocumentWrap_included


