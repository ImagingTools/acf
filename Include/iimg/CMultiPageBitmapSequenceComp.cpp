#include <iimg/CMultiPageBitmapSequenceComp.h>


namespace iimg
{


// protected methods
	
// reimplemented (idoc::IMultiPageDocument)

istd::IChangeable* CMultiPageBitmapSequenceComp::InsertPage(
			const idoc::IDocumentMetaInfo* pageMetaInfoPtr,
			const iprm::IParamsSet* /*pageParameterPtr*/,
			int position)
{
	istd::TDelPtr<iimg::IMultiBitmapProvider> multiBitmapPtr(m_multiBitmapFactoryCompPtr.CreateInstance());
	if (multiBitmapPtr.IsValid()){
		Page newPage;
			
		if (pageMetaInfoPtr != NULL){
			newPage.pageMetaInfo.CopyFrom(*pageMetaInfoPtr);
		}

		newPage.pagePtr.SetPtr(multiBitmapPtr.PopPtr());

		m_documentPages.insert(position, newPage);

		return newPage.pagePtr.GetPtr();
	}

	return NULL;
}


} // namespace iimg


