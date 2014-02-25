#include "iimg/CMultiPageBitmapBase.h"


namespace iimg
{


// public methods

// reimplemented (idoc::IMultiPageDocument)

istd::IChangeable* CMultiPageBitmapBase::InsertPage(
			const QString& pageTitle,
			const QSizeF& /*pageSize*/,
			int position)
{
	istd::CChangeNotifier changePtr(this);

	Page newPage;

	IBitmap* bitmapPtr = CreateBitmap();

	newPage.pageMetaInfo.SetDocumentMetaInfo(idoc::IDocumentMetaInfo::MIT_TITLE, pageTitle);
	newPage.pagePtr.SetPtr(bitmapPtr);

	if (position < 0){
		m_documentPages.push_back(newPage);
	}
	else{
		position = qMin(m_documentPages.count(), position);

		m_documentPages.insert(position, newPage);
	}

	return bitmapPtr;
}


// reimplemented (iimg::IMultiBitmapProvider)

const iprm::IOptionsList* CMultiPageBitmapBase::GetBitmapListInfo() const
{
	return this;
}


int CMultiPageBitmapBase::GetBitmapsCount() const
{
	return GetPagesCount();
}


const iimg::IBitmap* CMultiPageBitmapBase::GetBitmap(int bitmapIndex) const
{
	return dynamic_cast<const iimg::IBitmap*>(m_documentPages.at(bitmapIndex).pagePtr.GetPtr());
}


// reimplemented (iimg::IMultiPageBitmapController)

void CMultiPageBitmapBase::Reset()
{
	ResetPages();
}


iimg::IBitmap* CMultiPageBitmapBase::InsertBitmap(
			iimg::IBitmap::PixelFormat pixelFormat, 
			const istd::CIndex2d& size)
{
	iimg::IBitmap* pagePtr = dynamic_cast<iimg::IBitmap*>(InsertPage());
	if (!pagePtr->CreateBitmap(pixelFormat, size)){
		RemovePage(GetPagesCount() - 1);

		return NULL;
	}

	return pagePtr;
}


iimg::IBitmap* CMultiPageBitmapBase::InsertBitmap(
			iimg::IBitmap::PixelFormat pixelFormat, 
			const istd::CIndex2d& size, 
			void* dataPtr, 
			bool releaseFlag, 
			int linesDifference /*= 0*/)
{
	iimg::IBitmap* pagePtr = dynamic_cast<iimg::IBitmap*>(InsertPage());
	if (!pagePtr->CreateBitmap(pixelFormat, size, dataPtr, releaseFlag, linesDifference)){
		RemovePage(GetPagesCount() - 1);

		return NULL;
	}

	return pagePtr;
}


void CMultiPageBitmapBase::RemoveBitmap(int index)
{
	RemovePage(index);
}


// reimplemented (istd::IChangeable)

bool CMultiPageBitmapBase::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
{
	const CMultiPageBitmapBase* sourcePtr = dynamic_cast<const CMultiPageBitmapBase*>(&object);
	if (sourcePtr != NULL){
		istd::CChangeNotifier changePtr(this);

		m_documentPages.clear();

		for (int pageIndex = 0; pageIndex < sourcePtr->m_documentPages.count(); ++pageIndex){
			istd::IChangeable* pagePtr = InsertPage();
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

		if (!m_metaInfo.CopyFrom(sourcePtr->m_metaInfo)){
			return false;
		}

		return true;
	}

	return false;
}


} // namespace iimg


