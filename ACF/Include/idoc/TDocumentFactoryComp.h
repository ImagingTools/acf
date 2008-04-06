#ifndef idoc_TDocumentFactoryComp_included
#define idoc_TDocumentFactoryComp_included


#include "icomp/CComponentBase.h"

#include "istd/TSingleFactory.h"

#include "idoc/IDocument.h"


namespace idoc
{		


template <class DocumentImpl> 
class TDocumentFactoryComp: public icomp::CComponentBase, public istd::TSingleFactory<idoc::IDocument, DocumentImpl>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef  istd::TSingleFactory<idoc::IDocument, DocumentImpl> BaseClass2;
	typedef istd::TIFactory<imod::IModel> IModelFactory;
	
	I_BEGIN_COMPONENT(TDocumentFactoryComp)
		I_REGISTER_INTERFACE(idoc::IDocument::IDocumentFactory)
		I_ASSIGN(m_modelFactoryCompPtr, "ModelFactory", "Model factory", true, "ModelFactory")
	I_END_COMPONENT

	TDocumentFactoryComp();

	// reimplemented (istd::TIFactory)
	virtual idoc::IDocument* CreateInstance(const std::string& keyId = "") const;

private:
	I_REF(IModelFactory, m_modelFactoryCompPtr);
};


// public methods

template <class DocumentImpl> 
TDocumentFactoryComp<DocumentImpl>::TDocumentFactoryComp()
	:BaseClass2(std::string())
{
}


// reimplemented (istd::TIFactory)

template <class DocumentImpl> 
idoc::IDocument* TDocumentFactoryComp<DocumentImpl>::CreateInstance(const std::string& keyId) const
{
	idoc::IDocument* documentPtr = BaseClass2::CreateInstance(keyId);
	if (documentPtr != NULL){
		DocumentImpl* documentImplPtr = dynamic_cast<DocumentImpl*>(documentPtr);
		I_ASSERT(documentImplPtr != NULL);

		imod::IModel* modelPtr = m_modelFactoryCompPtr->CreateInstance();

		if (modelPtr != NULL){
			documentImplPtr->SetContent(modelPtr);
		}
		else{
			delete documentPtr;

			return NULL;
		}
	}

	return documentPtr;
}


} // namespace idoc


#endif // !idoc_TDocumentFactoryComp_included

