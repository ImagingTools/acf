#include "idoc/CSerializableDocument.h"


#include "iser/ISerializable.h"

#include "istd/TChangeNotifier.h"
 

namespace idoc
{		


// reimplemented (idoc::CDocument)

void CSerializableDocument::SetContent(imod::IModel* modelPtr)
{
	I_ASSERT(modelPtr != NULL);

	iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(modelPtr);
	
	I_ASSERT(serializablePtr != NULL);

	if (modelPtr->AttachObserver(&m_undoManager)){
		m_modelPtr.SetPtr(modelPtr);
	}
}


// reimplemented (idoc::IDocument)

bool CSerializableDocument::IsModified() const
{
	return m_undoManager.IsUndoAvailable();
}


imod::IUndoManager* CSerializableDocument::GetUndoManager() const
{
	return &(const_cast<CSerializableDocument*>(this))->m_undoManager;
}


} // namespace idoc

