#include "idoc/CSingleDocumentTemplateBase.h"


// STL includes
#include <algorithm>

#include "istd/TDelPtr.h"
#include "istd/CStaticServicesProvider.h"

#include "imod/IModel.h"

#include "isys/IFileSystem.h"

#include "idoc/IDocumentManager.h"

#include "iser/ISerializable.h"

#include "imod/CSerializedUndoManager.h"
#include "imod/TModelWrap.h"


namespace idoc
{		


CSingleDocumentTemplateBase::CSingleDocumentTemplateBase()
:	m_documentFactoryPtr(NULL),
	m_viewFactoryPtr(NULL)
{
}


void CSingleDocumentTemplateBase::SetDocumentFactory(IDocumentFactory* documentFactoryPtr)
{
	m_documentFactoryPtr = documentFactoryPtr;
}


void CSingleDocumentTemplateBase::SetViewFactory(IViewFactory* viewFactoryPtr)
{	
	m_viewFactoryPtr = viewFactoryPtr;
}


void CSingleDocumentTemplateBase::SetDocumentTypeId(const std::string& documentTypeId)
{
	m_documentTypeId = documentTypeId;
}


void CSingleDocumentTemplateBase::SetFileFilters(const istd::CStringList& fileFilters)
{
	m_fileFilters = fileFilters;
}


void CSingleDocumentTemplateBase::SetDefaultDirectory(const istd::CString& defaultDirectory)
{
	m_defaultDirectory = defaultDirectory;
}


void CSingleDocumentTemplateBase::SetFileExtensions(const istd::CStringList& fileExtensions)
{	
	m_fileExtensions = fileExtensions;
}


// reimplemented (idoc::IDocumentTemplate)

IDocumentTemplate::Ids CSingleDocumentTemplateBase::GetDocumentTypeIds() const
{
	IDocumentTemplate::Ids retVal;

	retVal.push_back(m_documentTypeId);

	return retVal;
}


IDocumentTemplate::Ids CSingleDocumentTemplateBase::GetViewTypeIds(const std::string& documentTypeId) const
{
	if (IsDocumentTypeSupported(documentTypeId)){
		return CSingleDocumentTemplateBase::GetDocumentTypeIds();
	}

	return Ids();
}


IDocumentTemplate::Ids CSingleDocumentTemplateBase::GetDocumentTypeIdsForFile(const istd::CString& filePath) const
{
	isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
	if (fileSystemPtr != NULL){
		istd::CString fileExtension = fileSystemPtr->GetExtension(filePath);
		istd::CStringList::const_iterator findIter = std::find(m_fileExtensions.begin(), m_fileExtensions.end(), fileExtension);

		if (findIter == m_fileExtensions.end()){
			return Ids();
		}
	}

	return GetDocumentTypeIds();
}


imod::IModel* CSingleDocumentTemplateBase::CreateDocument(const std::string& documentTypeId) const
{
	if ((m_documentFactoryPtr != NULL) && IsDocumentTypeSupported(documentTypeId)){
		return m_documentFactoryPtr->CreateInstance();
	}

	return NULL;
}


istd::IPolymorphic* CSingleDocumentTemplateBase::CreateView(imod::IModel* documentPtr, const std::string& viewTypeId) const
{
	I_ASSERT(documentPtr != NULL);

	if ((m_viewFactoryPtr != NULL) && IsViewTypeSupported(viewTypeId)){
		istd::TDelPtr<imod::IObserver> viewPtr(m_viewFactoryPtr->CreateInstance());
		if (viewPtr.IsValid() && documentPtr->AttachObserver(viewPtr.GetPtr())){
			return viewPtr.PopPtr();
		}
	}

	return NULL;
}


imod::IUndoManager* CSingleDocumentTemplateBase::CreateUndoManager(imod::IModel* documentPtr) const
{
	iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(documentPtr);
	if (serializablePtr != NULL){
		imod::TModelWrap<imod::CSerializedUndoManager>* undoManagerModelPtr = new imod::TModelWrap<imod::CSerializedUndoManager>();
		bool retVal = documentPtr->AttachObserver(undoManagerModelPtr);
		if (!retVal){
			delete undoManagerModelPtr;
			
			return NULL;
		}

		return undoManagerModelPtr;
	}

	return NULL;
}


istd::CStringList CSingleDocumentTemplateBase::GetFileFilters(const std::string* documentTypeIdPtr) const
{
	if ((documentTypeIdPtr == NULL) || IsDocumentTypeSupported(*documentTypeIdPtr)){
		return m_fileFilters;
	}

	return istd::CStringList();
}

istd::CStringList CSingleDocumentTemplateBase::GetFileExtensions(const std::string* documentTypeIdPtr) const
{
	if ((documentTypeIdPtr == NULL) || IsDocumentTypeSupported(*documentTypeIdPtr)){
		return m_fileExtensions;
	}

	return istd::CStringList();
}


istd::CString CSingleDocumentTemplateBase::GetDefaultDirectory(const istd::CString& sugestedDir, const std::string* documentTypeIdPtr) const
{
	if (sugestedDir.IsEmpty()){
		if ((documentTypeIdPtr == NULL) || IsDocumentTypeSupported(*documentTypeIdPtr)){
			return m_defaultDirectory;
		}
		else{
			return istd::CString();
		}
	}

	return sugestedDir;
}


} // namespace idoc


