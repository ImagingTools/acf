#ifndef idoc_TMultiPageDocumentWrap_included
#define idoc_TMultiPageDocumentWrap_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TSmartPtr.h"
#include "idoc/IMultiPageDocument.h"
#include "idoc/CStandardDocumentMetaInfo.h"


namespace idoc
{


/**
	Generic implementation of IMultiPageDocument interface.
*/
template <class Base, class PageInterface>
class TMultiPageDocumentWrap:
			virtual public Base,
			public CStandardDocumentMetaInfo
{
public:
	typedef Base BaseClass;
	typedef CStandardDocumentMetaInfo BaseClass2;
	typedef PageInterface PageInterfaceType;

	const PageInterfaceType* GetPageObject(int pageIndex) const;

	// pseudo-reimplemented (IMultiPageDocument)
	virtual int GetPagesCount() const;
	virtual const istd::IChangeable& GetDocumentPage(int pageIndex) const;
	virtual void ResetPages();
	virtual bool RemovePage(int pageIndex);
	virtual const IDocumentMetaInfo& GetDocumentMetaInfo() const;

protected:
	struct Page
	{
		QString pageTitle;
		istd::TSmartPtr<PageInterfaceType> pagePtr;
	};

	typedef QVector<Page> Pages;

	Pages m_documentPages;
};


// public methods

template <class Base, class PageInterface>
const typename TMultiPageDocumentWrap<Base, PageInterface>::PageInterfaceType* TMultiPageDocumentWrap<Base, PageInterface>::GetPageObject(int pageIndex) const
{
	Q_ASSERT(pageIndex < m_documentPages.count());
	Q_ASSERT(pageIndex >= 0);

	return m_documentPages.at(pageIndex).pagePtr.GetPtr();
}


// pseudo-reimplemented (IMultiPageDocument)

template <class Base, class PageInterface>
int TMultiPageDocumentWrap<Base, PageInterface>::GetPagesCount() const
{
	return m_documentPages.count();
}


template <class Base, class PageInterface>
const istd::IChangeable& TMultiPageDocumentWrap<Base, PageInterface>::GetDocumentPage(int pageIndex) const
{
	return *GetPageObject(pageIndex);
}


template <class Base, class PageInterface>
void TMultiPageDocumentWrap<Base, PageInterface>::ResetPages()
{
	istd::CChangeNotifier changePtr(this);

	m_documentPages.clear();
}


template <class Base, class PageInterface>
bool TMultiPageDocumentWrap<Base, PageInterface>::RemovePage(int pageIndex)
{
	Q_ASSERT(pageIndex < m_documentPages.count());
	Q_ASSERT(pageIndex >= 0);

	istd::CChangeNotifier changePtr(this);

	m_documentPages.remove(pageIndex);

	return true;
}


template <class Base, class PageInterface>
const IDocumentMetaInfo& TMultiPageDocumentWrap<Base, PageInterface>::GetDocumentMetaInfo() const
{
	return *this;
}


} // namespace idoc


#endif // !idoc_TMultiPageDocumentWrap_included


