#ifndef iqtdoc_CExtendedDocumentTemplateComp_included
#define iqtdoc_CExtendedDocumentTemplateComp_included


#include <QObject>

#include "idoc/ICommandsProvider.h"
#include "idoc/CSingleDocumentTemplateComp.h"

#include "iqtgui/IGuiObject.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "iqtdoc/iqtdoc.h"


namespace iqtdoc
{


/**
	Extension of \c idoc::CSingleDocumentTemplateComp providing additional Qt-specified functionality.
*/
class CExtendedDocumentTemplateComp:
			public QObject,
			public idoc::CSingleDocumentTemplateComp,
			virtual public idoc::ICommandsProvider
{
	Q_OBJECT

public:
	typedef idoc::CSingleDocumentTemplateComp BaseClass;

	CExtendedDocumentTemplateComp();

	I_BEGIN_COMPONENT(CExtendedDocumentTemplateComp)
		I_REGISTER_INTERFACE(idoc::ICommandsProvider)
		I_ASSIGN(m_aboutCommandTextAttrPtr, "AboutCommandText", "Text used in about menu", true, "Document");
		I_ASSIGN(m_aboutGuiCompPtr, "AboutGui", "GUI used inside about dialog box", false, "AboutGui");
		I_ASSIGN(m_viewGuiCompFact, "ViewFactory", "Create of document GUI", true, "ViewFactory");
	I_END_COMPONENT

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

	// reimplemented (idoc::IDocumentTemplate)
	virtual Ids GetDocumentTypeIdsForFile(const istd::CString& filePath) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

protected slots:
	void OnAboutCommand();

protected:
	// reimplemented (idoc::CSingleDocumentTemplateComp)
	virtual istd::IPolymorphic* ExtractViewInterface(icomp::IComponent* componentPtr) const;

private:
	I_ATTR(istd::CString, m_aboutCommandTextAttrPtr);
	I_REF(iqtgui::IGuiObject, m_aboutGuiCompPtr);
	I_FACT(iqtgui::IGuiObject, m_viewGuiCompFact);

	iqtgui::CHierarchicalCommand m_globalMenuCommands;
	istd::CString m_aboutCommandText;
};


} // namespace iqtdoc


#endif // !iqtdoc_CExtendedDocumentTemplateComp_included


