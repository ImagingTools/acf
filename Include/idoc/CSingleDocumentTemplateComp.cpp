#include <idoc/CSingleDocumentTemplateComp.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <imod/IModel.h>
#include <imod/TModelWrap.h>
#include <idoc/IUndoManager.h>


namespace idoc
{


// public methods

// reimplemented (idoc::IDocumentTemplate)

ifile::IFilePersistence* CSingleDocumentTemplateComp::GetFileLoader(const QByteArray& documentTypeId) const
{
	if (IsDocumentTypeSupported(documentTypeId)){
		return m_fileLoaderCompPtr.GetPtr();
	}
	else{
		return NULL;
	}
}


istd::IChangeable* CSingleDocumentTemplateComp::CreateDocument(
			QByteArray& documentTypeId,
			bool /*initialize*/,
			bool /*beQuiet*/,
			bool* ignoredFlagPtr) const
{
	if (ignoredFlagPtr != NULL){
		*ignoredFlagPtr = false;
	}

	if (m_documentCompFact.IsValid() && IsDocumentTypeSupported(documentTypeId)){
		if (documentTypeId.isEmpty()){
			documentTypeId = BaseClass2::GetDocumentTypeId();
		}

		return m_documentCompFact.CreateInstance();
	}

	return NULL;
}


istd::IPolymorphic* CSingleDocumentTemplateComp::CreateView(
			const QByteArray& documentTypeId,
			istd::IChangeable* documentPtr,
			const QByteArray& viewTypeId) const
{
	Q_ASSERT(documentPtr != NULL);

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


idoc::IUndoManager* CSingleDocumentTemplateComp::CreateUndoManager(const QByteArray& documentTypeId, istd::IChangeable* documentPtr) const
{
	if (IsDocumentTypeSupported(documentTypeId)){
		iser::ISerializable* serializablePtr = CompCastPtr<iser::ISerializable>(documentPtr);

		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(serializablePtr);
		if (modelPtr == NULL){
			modelPtr = CompCastPtr<imod::IModel>(documentPtr);
		}

		if ((serializablePtr != NULL) && (modelPtr != NULL)){
			istd::TDelPtr< idoc::IUndoManager > undoManagerModelPtr(m_undoManagerCompFact.CreateInstance());
			if (undoManagerModelPtr.IsValid()){
				imod::IObserver* observerPtr = m_undoManagerObserverCompFact.ExtractInterface(undoManagerModelPtr.GetPtr());
				if ((observerPtr != NULL) && modelPtr->AttachObserver(observerPtr)){
					return undoManagerModelPtr.PopPtr();
				}
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

	Q_ASSERT(m_documentTypeIdAttrPtr.IsValid());
	SetDocumentTypeId(*m_documentTypeIdAttrPtr);

	if (m_documentTypeNameAttrPtr.IsValid()){
		SetDocumentTypeName(*m_documentTypeNameAttrPtr);
	}
	else{
		SetDocumentTypeName((*m_documentTypeIdAttrPtr));
	}

	Q_ASSERT(m_defaultDirectoryAttrPtr.IsValid());
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

