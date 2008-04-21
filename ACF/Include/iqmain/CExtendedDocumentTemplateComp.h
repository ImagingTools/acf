#ifndef iqmain_CExtendedDocumentTemplateComp_included
#define iqmain_CExtendedDocumentTemplateComp_included


#include <QObject>

#include "idoc/ICommandsProvider.h"
#include "idoc/CSingleDocumentTemplateComp.h"

#include "iqt/IGuiObject.h"
#include "iqt/CHierarchicalCommand.h"

#include "iqmain/iqmain.h"


namespace iqmain
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
		I_ASSIGN(m_aboutCommandTextAttrPtr, "AboutCommandText", "Text used in about menu", true, "Document");
		I_ASSIGN(m_aboutGuiCompPtr, "AboutGui", "GUI used inside about dialog box", false, "AboutGui");
	I_END_COMPONENT

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

	// reimplemented (idoc::IDocumentTemplate)
	virtual Ids GetDocumentTypeIdsForFile(const istd::CString& filePath) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

protected slots:
	void OnAboutCommand();

private:
	I_ATTR(istd::CString, m_aboutCommandTextAttrPtr);
	I_REF(iqt::IGuiObject, m_aboutGuiCompPtr);

	iqt::CHierarchicalCommand m_globalMenuCommands;
	istd::CString m_aboutCommandText;
};


} // namespace iqmain


#endif // !iqmain_CExtendedDocumentTemplateComp_included


