#include "iqtdoc/CExtendedDocumentTemplateComp.h"


#include <QMessageBox>
#include <QFileInfo>


namespace iqtdoc
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


// reimplemented (idoc::IDocumentTemplate)

idoc::IDocumentTemplate::Ids CExtendedDocumentTemplateComp::GetDocumentTypeIdsForFile(const istd::CString& filePath) const
{
	QString path = iqt::GetQString(filePath);
	istd::CStringList extensions = GetFileExtensions();
	for (		istd::CStringList::const_iterator iter = extensions.begin();
				iter != extensions.end();
				++iter){
		QString extension = iqt::GetQString(*iter);
		if (path.endsWith(extension, Qt::CaseInsensitive)){
			return GetDocumentTypeIds();
		}
	}

	return idoc::IDocumentTemplate::Ids();
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


} // namespace iqtdoc


