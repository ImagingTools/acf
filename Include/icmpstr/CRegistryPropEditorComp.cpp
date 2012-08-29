#include "icmpstr/CRegistryPropEditorComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "icomp/CComponentMetaDescriptionEncoder.h"


// public methods
	
namespace icmpstr
{


// protected methods

void CRegistryPropEditorComp::CreateOverview()
{
	// reset view:
	OverviewTree->clear();

	// setup colors:
	QFont boldFont = qApp->font();
	boldFont.setBold(true);

	// create overview infos:
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		const icomp::IRegistry::ExportedInterfacesMap& exportedInterfaces = registryPtr->GetExportedInterfacesMap();
		if (!exportedInterfaces.isEmpty()){
			QTreeWidgetItem* exportedInterfacesItemPtr = new QTreeWidgetItem();
			exportedInterfacesItemPtr->setText(0, tr("Exported Interfaces"));
			exportedInterfacesItemPtr->setFont(0, boldFont);
			
			OverviewTree->addTopLevelItem(exportedInterfacesItemPtr);

			for (		icomp::IRegistry::ExportedInterfacesMap::const_iterator iter = exportedInterfaces.begin();
						iter != exportedInterfaces.end();
						iter++){
				QTreeWidgetItem* exportedInterfaceItemPtr = new QTreeWidgetItem();
				exportedInterfaceItemPtr->setText(0, QString(iter.value()));
				exportedInterfaceItemPtr->setText(1, QString(iter.key()));
				exportedInterfacesItemPtr->addChild(exportedInterfaceItemPtr);		
			}

			exportedInterfacesItemPtr->setExpanded(true);
		}

		const icomp::IRegistry::ExportedElementsMap& exportedComponents = registryPtr->GetExportedElementsMap();
		if (!exportedComponents.isEmpty()){
			QTreeWidgetItem* exportedComponentsItemPtr = new QTreeWidgetItem();
			exportedComponentsItemPtr->setText(0, tr("Exported Components"));
			exportedComponentsItemPtr->setFont(0, boldFont);

			OverviewTree->addTopLevelItem(exportedComponentsItemPtr);

			for (		icomp::IRegistry::ExportedElementsMap::const_iterator iter = exportedComponents.begin();
						iter != exportedComponents.end();
						iter++){
				QTreeWidgetItem* exportedComponentItemPtr = new QTreeWidgetItem();
				exportedComponentItemPtr->setText(0, QString(iter.value()));
				exportedComponentItemPtr->setText(1, QString(iter.key()));
				exportedComponentsItemPtr->addChild(exportedComponentItemPtr);		
			}

			exportedComponentsItemPtr->setExpanded(true);
		}

		if (m_consistInfoCompPtr.IsValid()){
			TextLog textLog;
			if (!m_consistInfoCompPtr->IsRegistryValid(*registryPtr, false, true, &textLog)){
				ErrorsLabel->setText(textLog);
				ErrorsFrame->setVisible(true);
			}
			else{
				ErrorsFrame->setVisible(false);
			}
		}
	}
}


// reimplemented (imod::TGuiObserverWrap)

void CRegistryPropEditorComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	DescriptionEdit->setEnabled(true);
	RegistryInfoFrame->setEnabled(true);
}


void CRegistryPropEditorComp::OnGuiModelDetached()
{
	DescriptionEdit->clear();
	CompanyEdit->clear();
	CategoryEdit->clear();
	ProjectEdit->clear();
	AuthorEdit->clear();
	TagsEdit->clear();
	KeywordsEdit->clear();

	DescriptionEdit->setDisabled(true);
	RegistryInfoFrame->setDisabled(true);

	OverviewTree->clear();

	ErrorsLabel->clear();

	BaseClass::OnGuiModelDetached();
}


void CRegistryPropEditorComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	const icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr == NULL){
		return;
	}

	DescriptionEdit->setText(registryPtr->GetDescription());

	icomp::CComponentMetaDescriptionEncoder metaDescriptionEncoder(registryPtr->GetKeywords());

	QStringList companyList = (metaDescriptionEncoder.GetValues("Company"));
	QStringList projectsList = (metaDescriptionEncoder.GetValues("Project"));
	QStringList authorsList = (metaDescriptionEncoder.GetValues("Author"));
	QStringList categoriesList = (metaDescriptionEncoder.GetValues("Category"));
	QStringList tagsList = (metaDescriptionEncoder.GetValues("Tag"));
	QStringList keywords = (metaDescriptionEncoder.GetUnassignedKeywords());

	CompanyEdit->setText(companyList.join(","));
	CategoryEdit->setText(categoriesList.join(","));
	ProjectEdit->setText(projectsList.join(","));
	AuthorEdit->setText(authorsList.join(","));
	TagsEdit->setText(tagsList.join(","));
	KeywordsEdit->setText(keywords.join(","));

	CreateOverview();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CRegistryPropEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	connect(CompanyEdit, SIGNAL(editingFinished()), this, SLOT(OnUpdateKeywords()));
	connect(ProjectEdit, SIGNAL(editingFinished()), this, SLOT(OnUpdateKeywords()));
	connect(AuthorEdit, SIGNAL(editingFinished()), this, SLOT(OnUpdateKeywords()));
	connect(CategoryEdit, SIGNAL(editingFinished()), this, SLOT(OnUpdateKeywords()));
	connect(TagsEdit, SIGNAL(editingFinished()), this, SLOT(OnUpdateKeywords()));
	connect(KeywordsEdit, SIGNAL(editingFinished()), this, SLOT(OnUpdateKeywords()));

	OverviewTree->header()->setResizeMode(QHeaderView::ResizeToContents);
	OverviewTree->setStyleSheet("QTreeView {background: palette(window)} QTreeView::branch {background: palette(window);} QTreeView::item {min-height: 25px}");
}


// protected slots

void CRegistryPropEditorComp::on_DescriptionEdit_editingFinished()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		QString description = DescriptionEdit->text();

		if (description != registryPtr->GetDescription()){
			istd::CChangeNotifier notifier(registryPtr);

			registryPtr->SetDescription(description);
		}
	}
}


void CRegistryPropEditorComp::OnUpdateKeywords()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		QString company = ConvertToKeyword(CompanyEdit->text(), "Company");
		QString project = ConvertToKeyword(ProjectEdit->text(), "Project");
		QString author = ConvertToKeyword(AuthorEdit->text(), "Author");
		QString category = ConvertToKeyword(CategoryEdit->text(), "Category");
		QString tags = ConvertToKeyword(TagsEdit->text(), "Tag");
		QString unassignedKeywords = ConvertToKeyword(KeywordsEdit->text());

		QString allKeywords =
					unassignedKeywords + QString(" ") + 
					company + QString(" ") + 
					project + QString(" ") + 
					author + QString(" ") + 
					category + QString(" ") + 
					tags;
		
		QString keywords = allKeywords.simplified();

		if (keywords != registryPtr->GetKeywords()){
			istd::CChangeNotifier notifier(registryPtr);

			registryPtr->SetKeywords(keywords);
		}
	}
}


// private static methods

QString CRegistryPropEditorComp::ConvertToKeyword(const QString& input, const QString& key)
{
	static QString emptyString;
	if (input.isEmpty()){
		return emptyString;
	}

	QString keyword = !key.isEmpty() ? key + "=\'" : key;

	QStringList inputPartList = input.split(",", QString::SkipEmptyParts);

	for (int inputPartIndex = 0; inputPartIndex < inputPartList.count(); inputPartIndex++){
		QString inputPart = inputPartList[inputPartIndex].simplified();
		if (inputPart.contains(" ")){
			inputPart = QString("\"") + inputPart + QString("\"");
		}

		keyword += inputPart + QString(" ");
	}

	keyword = keyword.simplified();
	
	if (!key.isEmpty()){
		keyword += "\'";
	}

	return keyword;
}


// public methods of embedded class TextLog

// reimplemented (ibase::IMessageConsumer)

bool CRegistryPropEditorComp::TextLog::IsMessageSupported(
			int /*messageCategory*/,
			int /*messageId*/,
			const istd::IInformationProvider* /*messagePtr*/) const
{
	return true;
}


void CRegistryPropEditorComp::TextLog::AddMessage(const MessagePtr& messagePtr)
{
	if (messagePtr.IsValid()){
		if (!isEmpty()){
			operator+=("\n");
		}

		operator+=(messagePtr->GetInformationDescription());
	}
}


} // namespace icmpstr


