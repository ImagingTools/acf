#include "idoc/CDocumentTemplateBase.h"

#include <algorithm>


#include "idoc/IDocument.h"
#include "idoc/IDocumentManager.h"


namespace idoc
{		

CDocumentTemplateBase::CDocumentTemplateBase(idoc::IDocumentManager* documentManagerPtr, int maxDocumentCount)
	:m_documentManagerPtr(documentManagerPtr),
	m_maxDocumentCount(maxDocumentCount),
	m_documentFactoryPtr(NULL),
	m_viewFactoryPtr(NULL)
{
}


void CDocumentTemplateBase::RegisterDocumentFactory(IDocumentFactory* documentFactoryPtr)
{
	m_documentFactoryPtr = documentFactoryPtr;
}


void CDocumentTemplateBase::RegisterViewFactory(IViewFactory* viewFactoryPtr)
{	
	m_viewFactoryPtr = viewFactoryPtr;
}


void CDocumentTemplateBase::SetFileFilters(const istd::CStringList& fileFilters)
{
	m_fileFilters = fileFilters;
}


void CDocumentTemplateBase::SetDefaultDirectory(const istd::CString& defaultDirectory)
{
	m_defaultDirectory = defaultDirectory;
}


void CDocumentTemplateBase::SetFileExtensions(const istd::CStringList& fileExtensions)
{	
	m_fileExtensions = fileExtensions;
}


void CDocumentTemplateBase::SetDefaultTitle(const istd::CString& defaultTitle)
{
	m_defaultTitle = defaultTitle;
}

	
// reimplemented (idoc::IDocumentTemplate)

idoc::IDocument* CDocumentTemplateBase::CreateDocument(const std::string& documentTypeId) const
{
	if (m_documentFactoryPtr == NULL){
		return NULL;
	}

	idoc::IDocument* documentPtr = m_documentFactoryPtr->CreateInstance();
	if (documentPtr == NULL){
		return NULL;
	}

	imod::IObserver* viewPtr = CreateView(*documentPtr);
	if (viewPtr != NULL){
		bool retVal = documentPtr->AddView(viewPtr);
		if (!retVal){
			delete documentPtr;
			delete viewPtr;

			return NULL;
		}
	}

	return documentPtr;
}


imod::IObserver* CDocumentTemplateBase::CreateView(const idoc::IDocument& document, const std::string& viewTypeId) const
{
	if (m_documentFactoryPtr == NULL || m_viewFactoryPtr == NULL){
		return NULL;
	}
	
	IDocumentFactory::KeyList factoryKeys = m_documentFactoryPtr->GetFactoryKeys();

	IDocumentFactory::KeyList::iterator foundIt = std::find(factoryKeys.begin(), factoryKeys.end(), document.GetDocumentId());
	if (foundIt == factoryKeys.end()){
		return NULL;
	}

	return m_viewFactoryPtr->CreateInstance(viewTypeId);
}


istd::CStringList CDocumentTemplateBase::GetFileFilters() const
{
	return m_fileFilters;
}


istd::CString CDocumentTemplateBase::GetDefaultDirectory() const
{
	return m_defaultDirectory;
}


istd::CStringList CDocumentTemplateBase::GetFileExtensions() const
{
	return m_fileExtensions;
}


istd::CString CDocumentTemplateBase::GetDefaultTitle() const
{
	return m_defaultTitle;
}


} // namespace idoc


