#include "icmpstr/CRegistryPropEditorComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


// public methods
	
namespace icmpstr
{


// reimplmented (imod::IModelEditor)

void CRegistryPropEditorComp::UpdateEditor(int /*updateFlags*/)
{
	if (!IsGuiCreated()){
		return;
	}

	const icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr == NULL){
		return;
	}

	DescriptionEdit->setText(iqt::GetQString(registryPtr->GetDescription()));
	KeywordsEdit->setText(iqt::GetQString(registryPtr->GetKeywords()));
	m_categoryComboBox->SetItemsChecked(GetCategoryStringList(registryPtr->GetCategory()));

	CreateOverview();
}


void CRegistryPropEditorComp::UpdateModel() const
{
}


// reimplemented (iqtgui::CGuiComponentBase)

void CRegistryPropEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	OverviewTree->header()->setResizeMode(QHeaderView::ResizeToContents);
	OverviewTree->setStyleSheet("QTreeView {background: palette(window)} QTreeView::branch {background: palette(window);} QTreeView::item {min-height: 25px}");

	m_categoryComboBox = new iqtgui::CCheckableComboBox(CategoryGroupBox);
	QLayout* groubBoxLayout = CategoryGroupBox->layout();
	if (groubBoxLayout != NULL){
		groubBoxLayout->addWidget(m_categoryComboBox);
	}

	m_categoriesMap[icomp::IComponentStaticInfo::CCT_APPLICATION] = "Application";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_MULTIMEDIA] = "Multimedia";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_DATA] = "Data";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_PERSISTENCY] = "Persistency";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_GUI] = "Gui";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_DATA_PRESENTATION] = "Data Presentation";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_DATA_PROCESSING] = "Data Proceassing";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_APPLICATION_LOGIC] = "Application Logic";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_SERVICE] = "Service";

	for (		CategoriesMap::const_iterator iterator = m_categoriesMap.begin();
				iterator != m_categoriesMap.end();
				iterator++){		
		m_categoryComboBox->addItem(iterator.value(), false);
	}

	connect(m_categoryComboBox, SIGNAL(EmitActivatedItems(const QStringList&)), this, SLOT(OnCategoriesChanged(const QStringList&)));
}


// protected slots:

void CRegistryPropEditorComp::on_DescriptionEdit_editingFinished()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		istd::CString description = iqt::GetCString(DescriptionEdit->text());

		if (description != registryPtr->GetDescription()){
			istd::CChangeNotifier notifier(registryPtr);

			registryPtr->SetDescription(description);
		}
	}
}


void CRegistryPropEditorComp::on_KeywordsEdit_editingFinished()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		istd::CString keywords = iqt::GetCString(KeywordsEdit->text());

		if (keywords != registryPtr->GetKeywords()){
			istd::CChangeNotifier notifier(registryPtr);

			registryPtr->SetKeywords(keywords);
		}
	}
}


void CRegistryPropEditorComp::OnCategoriesChanged(const QStringList& categories)
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		int category = icomp::IComponentStaticInfo::CCT_NONE;

		for (int index = 0; index < categories.count(); index++){
			int singleCategory = m_categoriesMap.key(categories[index], -1);
			if (singleCategory != -1){
				category |= singleCategory;
			}
		}

		if (category != registryPtr->GetCategory()){
			istd::CChangeNotifier notifier(registryPtr);

			registryPtr->SetCategory(category);
		}
	}
}




// private methods

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
		if (!exportedInterfaces.empty()){
			QTreeWidgetItem* exportedInterfacesItemPtr = new QTreeWidgetItem();
			exportedInterfacesItemPtr->setText(0, tr("Exported Interfaces"));
			exportedInterfacesItemPtr->setFont(0, boldFont);
			
			OverviewTree->addTopLevelItem(exportedInterfacesItemPtr);

			for (		icomp::IRegistry::ExportedInterfacesMap::const_iterator index = exportedInterfaces.begin();
						index != exportedInterfaces.end();
						index++){
				QTreeWidgetItem* exportedInterfaceItemPtr = new QTreeWidgetItem();
				exportedInterfaceItemPtr->setText(0, QString(index->second.c_str()));
				exportedInterfaceItemPtr->setText(1, QString(index->first.GetName().c_str()));
				exportedInterfacesItemPtr->addChild(exportedInterfaceItemPtr);		
			}

			exportedInterfacesItemPtr->setExpanded(true);
		}

		const icomp::IRegistry::ExportedComponentsMap& exportedComponents = registryPtr->GetExportedComponentsMap();
		if (!exportedComponents.empty()){
			QTreeWidgetItem* exportedComponentsItemPtr = new QTreeWidgetItem();
			exportedComponentsItemPtr->setText(0, tr("Exported Components"));
			exportedComponentsItemPtr->setFont(0, boldFont);

			OverviewTree->addTopLevelItem(exportedComponentsItemPtr);

			for (		icomp::IRegistry::ExportedComponentsMap::const_iterator index = exportedComponents.begin();
						index != exportedComponents.end();
						index++){
				QTreeWidgetItem* exportedComponentItemPtr = new QTreeWidgetItem();
				exportedComponentItemPtr->setText(0, QString(index->second.c_str()));
				exportedComponentItemPtr->setText(1, QString(index->first.c_str()));
				exportedComponentsItemPtr->addChild(exportedComponentItemPtr);		
			}

			exportedComponentsItemPtr->setExpanded(true);
		}
	}
}


QStringList CRegistryPropEditorComp::GetCategoryStringList(int category) const
{
	QStringList retVal;

	for (		CategoriesMap::const_iterator iterator = m_categoriesMap.begin();
				iterator != m_categoriesMap.end();
				iterator++){		
		if ((iterator.key() & category) != 0){
			retVal.push_back(iterator.value());
		}
	}

	return retVal;
}



} // namespace icmpstr


