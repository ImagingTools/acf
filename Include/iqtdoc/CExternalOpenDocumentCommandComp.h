#ifndef iqtdoc_CExternalOpenDocumentCommandComp_included
#define iqtdoc_CExternalOpenDocumentCommandComp_included


// ACF includes
#include <imod/TSingleModelObserverBase.h>
#include <idoc/IDocumentManager.h>
#include <ifilegui/CExternalOpenDocumentCommandCompBase.h>


namespace iqtdoc
{


class CExternalOpenDocumentCommandComp:
			public ifilegui::CExternalOpenDocumentCommandCompBase,
			protected imod::TSingleModelObserverBase<idoc::IDocumentManager>
{
public:
	typedef ifilegui::CExternalOpenDocumentCommandCompBase BaseClass;

	I_BEGIN_COMPONENT(CExternalOpenDocumentCommandComp);
		I_ASSIGN(m_documentManagerCompPtr, "DocumentManager", "Manager of the documents in the MDI workspace", true, "DocumentManager");
		I_ASSIGN_TO(m_documentManagerModelCompPtr, m_documentManagerCompPtr, true);
		I_ASSIGN(m_documentTypeIdAttrPtr, "DocumentTypeId", "ID of the document type used by MDI workspace", true, "Default");
	I_END_COMPONENT;

protected:
	virtual void UpdateCommands();

	// reimplemented (ifilegui::CExternalOpenDocumentCommandCompBase)
	virtual const istd::IChangeable* GetDocumentPtr() const;

	// reimplemented (iqtgui::CStartProcessCommandComp)
	virtual void CreateMenu();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(idoc::IDocumentManager, m_documentManagerCompPtr);
	I_REF(imod::IModel, m_documentManagerModelCompPtr);
	I_ATTR(QByteArray, m_documentTypeIdAttrPtr);

	iqtgui::CHierarchicalCommand m_fileCommand;
};


}  // namespace iqtdoc


#endif // iqtdoc_CExternalOpenDocumentCommandComp_included


