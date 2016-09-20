#include <iqtdoc/CExternalOpenDocumentCommandComp.h>


// Qt includes
#include <QtCore/QProcess>


namespace iqtdoc
{


// protected methods

void CExternalOpenDocumentCommandComp::UpdateCommands()
{
	m_openDocumentCommand.SetEnabled(false);

	const idoc::IDocumentManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		istd::IPolymorphic* activeViewPtr = objectPtr->GetActiveView();
		if (activeViewPtr != NULL){
			idoc::IDocumentManager::DocumentInfo documentInfo;
			istd::IChangeable* documentPtr = objectPtr->GetDocumentFromView(*activeViewPtr, &documentInfo);
			if ((documentPtr != NULL) && (documentInfo.documentTypeId == *m_documentTypeIdAttrPtr)){
				m_openDocumentCommand.SetEnabled(true);
			}
		}
	}
}


// reimplemented (ifilegui::CExternalOpenDocumentCommandCompBase)

const istd::IChangeable* CExternalOpenDocumentCommandComp::GetDocumentPtr() const
{
	Q_ASSERT(m_documentManagerCompPtr.IsValid());

	istd::IPolymorphic* activeViewPtr = m_documentManagerCompPtr->GetActiveView();
	if (activeViewPtr != NULL){
		return m_documentManagerCompPtr->GetDocumentFromView(*activeViewPtr);
	}

	return NULL;
}


// reimplemented (imod::CSingleModelObserverBase)

void CExternalOpenDocumentCommandComp::OnUpdate(const ChangeSet& /*changeSet*/)
{
	UpdateCommands();
}


// reimplemented (icomp::CComponentBase)

void CExternalOpenDocumentCommandComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (!m_documentManagerCompPtr.IsValid()){
		m_openDocumentCommand.setEnabled(false);
	}

	if (m_documentManagerModelCompPtr.IsValid()){
		m_documentManagerModelCompPtr->AttachObserver(this);
	}

	UpdateCommands();
}


void CExternalOpenDocumentCommandComp::OnComponentDestroyed()
{
	EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace iqtdoc


