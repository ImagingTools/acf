#include "iqtdoc/CExtendedDocumentTemplateComp.h"


#include <QMessageBox>
#include <QFileInfo>
#include <QAction>


namespace iqtdoc
{


CExtendedDocumentTemplateComp::CExtendedDocumentTemplateComp()
:	m_globalMenuCommands("Global")
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
	idoc::IDocumentTemplate::Ids retVal;

	idoc::IDocumentTemplate::Ids allIds = GetDocumentTypeIds();

	QString qtFilePath = iqt::GetQString(filePath);

	for (		IDocumentTemplate::Ids::const_iterator iter = allIds.begin();
				iter != allIds.end();
				++iter){
		const std::string& id = *iter;

		istd::CStringList extensions;
		iser::IFileLoader* loaderPtr = GetFileLoader(id);
		if ((loaderPtr != NULL) && loaderPtr->GetFileExtensions(extensions)){
			for (		istd::CStringList::const_iterator iter = extensions.begin();
						iter != extensions.end();
						++iter){
				QString extension = iqt::GetQString(*iter);
				if (qtFilePath.endsWith(extension, Qt::CaseInsensitive)){
					retVal.push_back(id);

					break;
				}
			}
		}
	}

	return retVal;
}


// reimplemented (icomp::CComponentBase)

void CExtendedDocumentTemplateComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_aboutCommandText = istd::CString("About ") + *m_aboutCommandTextAttrPtr + istd::CString("...");

	I_ASSERT(m_aboutCommandTextAttrPtr.IsValid());
	if (m_aboutGuiCompPtr.IsValid()){
		iqtgui::CHierarchicalCommand* aboutMenuPtr = new iqtgui::CHierarchicalCommand("&Help");
		iqtgui::CHierarchicalCommand* aboutCommandPtr = new iqtgui::CHierarchicalCommand(m_aboutCommandText);
		aboutCommandPtr->setMenuRole(QAction::AboutRole);

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


// protected methods

// reimplemented (idoc::CSingleDocumentTemplateComp)

istd::IPolymorphic* CExtendedDocumentTemplateComp::ExtractViewInterface(icomp::IComponent* componentPtr) const
{
	return m_viewGuiCompFact.ExtractInterface(componentPtr);
}


} // namespace iqtdoc


