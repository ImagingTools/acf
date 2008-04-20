#include "iqmain/CExtendedDocumentTemplateComp.h"


#include <QMessageBox>


namespace iqmain
{


CExtendedDocumentTemplateComp::CExtendedDocumentTemplateComp()
:	m_globalMenuCommands("Global", NULL)
{
	m_globalMenuCommands.SetEnabled(false);
}


// reimplemented (idoc::ICommandsProvider)

const idoc::IHierarchicalCommand* CExtendedDocumentTemplateComp::GetCommands() const
{
	return &m_globalMenuCommands;
}


// reimplemented (icomp::CComponentBase)

void CExtendedDocumentTemplateComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_aboutCommandText = istd::CString("About ") + *m_aboutCommandTextAttrPtr + istd::CString("...");

	I_ASSERT(m_aboutCommandTextAttrPtr.IsValid());
	if (m_aboutGuiCompPtr.IsValid()){
		iqt::CHierarchicalCommand* aboutMenuPtr = new iqt::CHierarchicalCommand("&Help");
		iqt::CHierarchicalCommand* aboutCommandPtr = new iqt::CHierarchicalCommand(m_aboutCommandText);

		connect(aboutCommandPtr, SIGNAL(activated()), this, SLOT(OnAboutCommand()));

		aboutMenuPtr->InsertChild(aboutCommandPtr, true);
		m_globalMenuCommands.InsertChild(aboutMenuPtr, true);
	}
}


// protected slots

void CExtendedDocumentTemplateComp::OnAboutCommand()
{
	I_ASSERT(m_aboutGuiCompPtr.IsValid());

	QMessageBox messageBox(QMessageBox::NoIcon, iqt::GetQString(m_aboutCommandText), "");
	if (m_aboutGuiCompPtr->CreateGui(&messageBox)){
		messageBox.exec();
		m_aboutGuiCompPtr->DestroyGui();
	}
}


} // namespace iqmain


