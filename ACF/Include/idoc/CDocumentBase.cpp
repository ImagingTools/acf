#include "idoc/CDocumentBase.h"


#include "iser/ISerializable.h"

#include "istd/TChangeNotifier.h"
 

namespace idoc
{		


CDocumentBase::CDocumentBase()
{
}


CDocumentBase::~CDocumentBase()
{
	if (m_modelPtr.IsValid()){
		m_modelPtr->DetachAllObservers();
	}
}



void CDocumentBase::SetDocumentId(const std::string& documentId)
{
	m_documentId = documentId;
}


void CDocumentBase::SetContent(imod::IModel* modelPtr)
{
	I_ASSERT(modelPtr != NULL);

	m_modelPtr.SetPtr(modelPtr);
}


void CDocumentBase::SetFileName(const istd::CString& fileName)
{
	m_fileName = fileName;
}


void CDocumentBase::SetDocumentTemplate(const idoc::IDocumentTemplate* documentTemplatePtr)
{
	I_ASSERT(documentTemplatePtr != NULL);

	m_documentTemplatePtr = documentTemplatePtr;
}


void CDocumentBase::RegisterFileEngine(const istd::IFileEngine* fileEnginePtr)
{
	m_fileEnginePtr = fileEnginePtr;
}


// reimplemented (idoc::IDocument)

istd::CString CDocumentBase::GetFileName() const
{
	return m_fileName;
}


bool CDocumentBase::Load(const istd::CString& fileName)
{
	imod::IModel* modelPtr = m_modelPtr.GetPtr();
	if (m_fileEnginePtr != NULL && modelPtr != NULL){
		bool retVal = m_fileEnginePtr->Load(*modelPtr, fileName);
		if (retVal){
			istd::CChangeNotifier changePtr(dynamic_cast<istd::IChangeable*>(modelPtr));

			SetFileName(fileName);

			return true;
		}
	}

	return false;
}


bool CDocumentBase::Save(const istd::CString& fileName)
{
	imod::IModel* modelPtr = m_modelPtr.GetPtr();
	if (m_fileEnginePtr != NULL && modelPtr != NULL){
		bool retVal = m_fileEnginePtr->Save(*modelPtr, fileName);
		if (retVal){
			SetFileName(fileName);
			
			return true;
		}
	}

	return false;
}


std::string CDocumentBase::GetDocumentId() const
{
	return m_documentId;
}


istd::CString CDocumentBase::GetDocumentTitle() const
{
	return m_title;
}


void CDocumentBase::SetDocumentTitle(const istd::CString& documentTitle)
{
	if (m_title != documentTitle){
		istd::TChangeNotifier<idoc::IDocument> changePtr(this, TitleChanged);

		m_title = documentTitle;
	}
}


imod::IModel* CDocumentBase::GetContent() const
{
	return const_cast<imod::IModel*>(m_modelPtr.GetPtr());
}


bool CDocumentBase::AddView(imod::IObserver* viewPtr)
{
	bool retVal = m_modelPtr->AttachObserver(viewPtr);
	if (!retVal){
		return false;
	}

	m_viewList.PushBack(viewPtr);

	return true;
}


void CDocumentBase::RemoveView(imod::IObserver* /*viewPtr*/)
{
}


int CDocumentBase::GetViewCount()
{
	return m_viewList.GetCount();
}


imod::IObserver* CDocumentBase::GetView(int viewIndex)
{
	I_ASSERT(viewIndex < m_viewList.GetCount());
	I_ASSERT(viewIndex >= 0);

	return m_viewList.GetAt(viewIndex);
}


bool CDocumentBase::HasView(const imod::IObserver* /*viewPtr*/)
{
	return false;
}


} // namespace idoc

