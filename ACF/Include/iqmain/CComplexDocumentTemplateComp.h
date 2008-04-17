#ifndef iqmain_CComplexDocumentTemplateComp_included
#define iqmain_CComplexDocumentTemplateComp_included


#include <QObject>

#include "idoc/ICommandsProvider.h"
#include "idoc/CSerializedDocumentTemplateComp.h"

#include "iqt/IGuiObject.h"
#include "iqt/CHierarchicalCommand.h"

#include "iqmain/iqmain.h"


namespace iqmain
{


/**
	Extension of \c idoc::CSerializedDocumentTemplateComp providing additional Qt-specified functionality.
*/
class CComplexDocumentTemplateComp:
			public QObject,
			public idoc::CSerializedDocumentTemplateComp,
			virtual public idoc::ICommandsProvider
{
	Q_OBJECT

public:
	typedef idoc::CSerializedDocumentTemplateComp BaseClass;

	CComplexDocumentTemplateComp();

	I_BEGIN_COMPONENT(CComplexDocumentTemplateComp)
		I_ASSIGN(m_aboutCommandTextAttrPtr, "AboutCommandText", "Text used in about menu", true, "Document");
		I_ASSIGN(m_aboutGuiCompPtr, "AboutGui", "GUI used inside about dialog box", false, "AboutGui");
	I_END_COMPONENT

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

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


#endif // !iqmain_CComplexDocumentTemplateComp_included


