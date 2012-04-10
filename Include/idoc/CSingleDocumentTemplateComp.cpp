#include "idoc/CSingleDocumentTemplateComp.h"


#include "istd/TDelPtr.h"

#include "imod/IModel.h"
#include "imod/CSerializedUndoManager.h"
#include "imod/TModelWrap.h"


namespace idoc
{		


// reimplemented (idoc::IDocumentTemplate)

iser::IFileLoader* CSingleDocumentTemplateComp::GetFileLoader(const QByteArray& documentTypeId) const
{
	if (IsDocumentTypeSupported(documentTypeId)){
		return m_fileLoaderCompPtr.GetPtr();
	}
	else{
		return NULL;
	}
}


istd::IChangeable* CSingleDocumentTemplateComp::CreateDocument(const QByteArray& documentTypeId) const
{
	if (m_documentCompFact.IsValid() && IsDocumentTypeSupported(documentTypeId)){
		return m_documentCompFact.CreateInstance();
	}

	return NULL;
}


istd::IPolymorphic* CSingleDocumentTemplateComp::CreateView(
			const QByteArray& documentTypeId,
			istd::IChangeable* documentPtr,
			const QByteArray& viewTypeId) const
{
	I_ASSERT(documentPtr != NULL);

	imod::IModel* modelPtr = CompCastPtr<imod::IModel>(documentPtr);

	if (		(modelPtr != NULL) &&
				m_viewCompFact.IsValid() &&
				IsDocumentTypeSupported(documentTypeId) &&
				IsViewTypeSupported(viewTypeId)){
		istd::TDelPtr<icomp::IComponent> componentPtr(m_viewCompFact.CreateComponent());

		imod::IObserver* observerPtr = m_viewCompFact.ExtractInterface(componentPtr.GetPtr());
		istd::IPolymorphic* viewPtr = ExtractViewInterface(componentPtr.GetPtr());

		if (		(viewPtr != NULL) &&
					(observerPtr != NULL) &&
					modelPtr->AttachObserver(observerPtr)){
			componentPtr.PopPtr();

			return viewPtr;
		}
	}

	return NULL;
}


imod::IUndoManager* CSingleDocumentTemplateComp::CreateUndoManager(const QByteArray& documentTypeId, istd::IChangeable* documentPtr) const
{
	if (IsDocumentTypeSupported(documentTypeId)){
		iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(documentPtr);
		imod::IModel* modelPtr = CompCastPtr<imod::IModel>(documentPtr);
		if ((serializablePtr != NULL) && (modelPtr != NULL)){
			istd::TDelPtr<imod::TModelWrap<imod::CSerializedUndoManager> > undoManagerModelPtr(new imod::TModelWrap<imod::CSerializedUndoManager>);
			if (		undoManagerModelPtr.IsValid() &&
						modelPtr->AttachObserver(undoManagerModelPtr.GetPtr())){
				return undoManagerModelPtr.PopPtr();
			}
		}
	}

	return NULL;
}


// protected methods

istd::IPolymorphic* CSingleDocumentTemplateComp::ExtractViewInterface(icomp::IComponent* componentPtr) const
{
	return m_viewCompFact.ExtractInterface(componentPtr);
}


// reimplemented (icomp::CComponentBase)

void CSingleDocumentTemplateComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_ASSERT(m_documentTypeIdAttrPtr.IsValid());
	SetDocumentTypeId(*m_documentTypeIdAttrPtr);

	if (m_documentTypeNameAttrPtr.IsValid()){
		SetDocumentTypeName(*m_documentTypeNameAttrPtr);
	}
	else{
		SetDocumentTypeName((*m_documentTypeIdAttrPtr));
	}

	I_ASSERT(m_defaultDirectoryAttrPtr.IsValid());
	SetDefaultDirectory(*m_defaultDirectoryAttrPtr);

	int featureFlags = 0;
	if (*m_isNewSupportedAttrPtr){
		featureFlags |= SF_NEW_DOCUMENT;
	}

	if (*m_isEditSupportedAttrPtr){
		featureFlags |= SF_EDIT_DOCUMENT;
	}

	SetSupportedFeatures(featureFlags);
}


} // namespace idoc

