#include "icmpstr/CRegistryViewComp.h"


// Qt includes
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>

#include "istd/TChangeNotifier.h"

#include "iser/CMemoryReadArchive.h"
#include "iser/CMemoryWriteArchive.h"

#include "ibase/IApplication.h"

#include "iqtgui/TDesignerBasicGui.h"

#include "icmpstr/IRegistryEditController.h"
#include "icmpstr/CComponentView.h"
#include "icmpstr/CComponentConnector.h"
#include "icmpstr/CRegistryModelComp.h"

#include "icmpstr/Generated/ui_CRegistryPropertiesDialog.h"


namespace icmpstr
{


CRegistryViewComp::CRegistryViewComp()
{
	SetUpdateFilter(~CRegistryModelComp::CF_POSITION);

	int lightToolFlags = idoc::IHierarchicalCommand::CF_GLOBAL_MENU | idoc::IHierarchicalCommand::CF_TOOLBAR;

	m_removeComponentCommand.setEnabled(false);
	m_removeComponentCommand.SetGroupId(GI_COMPONENT);
	m_removeComponentCommand.SetStaticFlags(lightToolFlags);
	m_removeComponentCommand.setShortcut(QKeySequence(Qt::Key_Delete));
	m_renameComponentCommand.setEnabled(false);
	m_renameComponentCommand.SetGroupId(GI_COMPONENT);
	m_renameComponentCommand.setShortcut(QKeySequence(Qt::Key_F2));
	m_exportComponentCommand.setEnabled(false);
	m_exportComponentCommand.SetGroupId(GI_COMPONENT);
	m_exportInterfaceCommand.setEnabled(false);
	m_exportInterfaceCommand.SetGroupId(GI_COMPONENT);
	m_exportInterfaceCommand.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
	m_propertiesCommand.setEnabled(true);
	m_propertiesCommand.SetGroupId(GI_COMPONENT);
	m_propertiesCommand.setShortcut(QKeySequence(Qt::ALT + Qt::Key_Enter));
	m_exportToCodeCommand.SetGroupId(GI_CODEGEN);
	m_executeRegistryCommand.setEnabled(false);
	m_executeRegistryCommand.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
	m_executeRegistryCommand.SetGroupId(GI_PREVIEW);
	m_executeRegistryCommand.SetStaticFlags(lightToolFlags);
	m_abortRegistryCommand.setEnabled(false);
	m_abortRegistryCommand.SetGroupId(GI_PREVIEW);
	m_abortRegistryCommand.SetStaticFlags(lightToolFlags);
	m_abortRegistryCommand.setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5));
	m_removeNoteCommand.setEnabled(false);
	m_removeNoteCommand.SetGroupId(GI_COMPONENT);
	m_removeNoteCommand.SetStaticFlags(lightToolFlags);
	m_addNoteCommand.setEnabled(false);
	m_addNoteCommand.SetGroupId(GI_COMPONENT);
	m_addNoteCommand.SetStaticFlags(lightToolFlags);

	m_registryMenu.InsertChild(&m_removeComponentCommand);
	m_registryMenu.InsertChild(&m_renameComponentCommand);
	m_registryMenu.InsertChild(&m_exportComponentCommand);
	m_registryMenu.InsertChild(&m_exportInterfaceCommand);
	m_registryMenu.InsertChild(&m_propertiesCommand);
	m_registryMenu.InsertChild(&m_exportToCodeCommand);
	m_registryMenu.InsertChild(&m_executeRegistryCommand);
	m_registryMenu.InsertChild(&m_abortRegistryCommand);
	m_registryMenu.InsertChild(&m_addNoteCommand);
	m_registryMenu.InsertChild(&m_removeNoteCommand);
	m_registryCommand.InsertChild(&m_registryMenu);
}


bool CRegistryViewComp::TryCreateComponent(const icomp::CComponentAddress& address, const i2d::CVector2d& position)
{
	bool retVal = false;
	QString componentName = QInputDialog::getText(GetWidget(), tr("Application Compositor"), tr("Component name"), QLineEdit::Normal, "",&retVal);
	if (retVal && !componentName.isEmpty()){
		istd::TChangeNotifier<icomp::IRegistry> registryPtr(GetObjectPtr(), icomp::IRegistry::CF_COMPONENT_ADDED);
		if (registryPtr.IsValid()){
			icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->InsertElementInfo(componentName.toStdString(), address);
			if (elementInfoPtr != NULL){
				IRegistryEditController* providerPtr = dynamic_cast<IRegistryEditController*>(registryPtr.GetPtr());
				if (providerPtr != NULL){
					providerPtr->SetComponentPosition(componentName.toStdString(), position);
				}
				ConnectReferences(componentName);
				return true;
			}
		}

		QMessageBox::critical(GetWidget(), tr("Error"), tr("Component could not be added")); 
	}

	return false;
}


// reimplemented (idoc::ICommandsProvider)

const idoc::IHierarchicalCommand* CRegistryViewComp::GetCommands() const
{
	return &m_registryCommand;
}


// reimplemented (imod::TModelEditorBase)

void CRegistryViewComp::UpdateEditor()
{
	CRegistryView* viewPtr = GetQtWidget();
	if (viewPtr == NULL){
		return;
	}

	viewPtr->ResetScene();

	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		icomp::IRegistry::Ids elementIds = registryPtr->GetElementIds();
		for (		icomp::IRegistry::Ids::iterator iter = elementIds.begin();
					iter != elementIds.end();
					iter++){
			const std::string& elementId = *iter;
			const icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->GetElementInfo(elementId);
			if (elementInfoPtr != NULL){
				CComponentView* componentViewPtr = viewPtr->CreateComponentView(registryPtr, elementInfoPtr, elementId);
				I_ASSERT(componentViewPtr != NULL);

				connect(componentViewPtr, 
							SIGNAL(selectionChanged(CComponentView*, bool)),
							this,
							SLOT(OnComponentViewSelected(CComponentView*, bool)));

				connect(componentViewPtr, 
							SIGNAL(positionChanged(CComponentView*, const QPoint&)),
							this,
							SLOT(OnComponentPositionChanged(CComponentView*, const QPoint&)));
				
				IRegistryEditController* geomeometryProviderPtr = dynamic_cast<IRegistryEditController*>(registryPtr);
				if (geomeometryProviderPtr != NULL){
					i2d::CVector2d position = geomeometryProviderPtr->GetComponentPosition(elementId);
					componentViewPtr->setPos(int(position.GetX() + 0.5), int(position.GetY() + 0.5));
				}

				const CComponentView* selectedComponentPtr = viewPtr->GetSelectedComponent();
				if (		selectedComponentPtr != NULL && 
							componentViewPtr->GetComponentName() == selectedComponentPtr->GetComponentName()){
					componentViewPtr->setSelected(true);
				}	
			}
		}
	}

	UpdateConnectors();
}


void CRegistryViewComp::UpdateModel() const
{
}


// reimplemented (iqtgui::CGuiComponentBase)

void CRegistryViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	OnRetranslate();

	connect(&m_removeComponentCommand, SIGNAL(activated()), this, SLOT(OnRemoveComponent()));
	connect(&m_renameComponentCommand, SIGNAL(activated()), this, SLOT(OnRenameComponent()));
	connect(&m_exportToCodeCommand, SIGNAL(activated()), this, SLOT(OnExportToCode()));
	connect(&m_executeRegistryCommand, SIGNAL(activated()), this, SLOT(OnExecute()));
	connect(&m_abortRegistryCommand, SIGNAL(activated()), this, SLOT(OnAbort()));
	connect(&m_addNoteCommand, SIGNAL(activated()), this, SLOT(OnAddNote()));
	connect(&m_removeNoteCommand, SIGNAL(activated()), this, SLOT(OnRemoveNote()));
	connect(&m_exportComponentCommand, SIGNAL(activated()), this, SLOT(OnExportComponent()));
	connect(&m_exportInterfaceCommand, SIGNAL(activated()), this, SLOT(OnExportInterface()));
	connect(&m_propertiesCommand, SIGNAL(activated()), this, SLOT(OnProperties()));

	if (m_registryPreviewCompPtr.IsValid()){
		connect(&m_executionObserverTimer, SIGNAL(timeout()), this, SLOT(OnExecutionTimerTick()));

		m_executionObserverTimer.start(500);
	}

	CRegistryView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr != NULL){
		connect(viewPtr, 
					SIGNAL(DropDataEventEntered(const QMimeData&, QGraphicsSceneDragDropEvent*)), 
					this, 
					SLOT(ProcessDroppedData(const QMimeData&, QGraphicsSceneDragDropEvent*)));
		viewPtr->SetPackagesManager(m_packagesManagerCompPtr.GetPtr());
	}
}


void CRegistryViewComp::OnGuiDestroyed()
{
	OnComponentViewSelected(NULL, false);

	CRegistryView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr != NULL){
		viewPtr->ResetScene();
	}

	BaseClass::OnGuiDestroyed();
}


void CRegistryViewComp::OnRetranslate()
{
	BaseClass::OnRetranslate();

	m_registryMenu.SetVisuals(
				tr("&Registry"),
				tr("Registry"),
				tr("Set of commands manipulating registry"));
	m_removeComponentCommand.SetVisuals(
				tr("&Remove Component"), 
				tr("Remove"), 
				tr("Remove the selected component from the registry"),
				QIcon(":/Resources/Icons/delete_64.png"));
	m_renameComponentCommand.SetVisuals(
				tr("&Rename Component"), 
				tr("Rename"), 
				tr("Allow to assign new name to selected component"));
	m_exportComponentCommand.SetVisuals(
				tr("&Export Component"), 
				tr("&Export Component"), 
				tr("Export component"),
				QIcon(":/Resources/Icons/Export.png"));
	m_exportInterfaceCommand.SetVisuals(
				tr("&Export Inteface(s)"), 
				tr("&Export Interface(s)"), 
				tr("Export interface(s)"),
				QIcon(":/Resources/Icons/Export.png"));
	m_propertiesCommand.SetVisuals(
				tr("&Properties"), 
				tr("&Properties"), 
				tr("Edit registry properties"));
	m_exportToCodeCommand.SetVisuals(
				tr("&Export To Code..."),
				tr("Export"),
				tr("Generates C++ code according to this registry"));
	m_executeRegistryCommand.SetVisuals(
				tr("&Execute Registry"), 
				tr("&Execute Registry"), 
				tr("Start registry execution"),
				QIcon(":/Resources/Icons/player_play.png"));
	m_abortRegistryCommand.SetVisuals(
				tr("&Abort Registry"), 
				tr("&Abort Registry"), 
				tr("Abort registry execution"),
				QIcon(":/Resources/Icons/player_stop.png"));
	m_addNoteCommand.SetVisuals(
				tr("&Add Note"), 
				tr("&Add Note"), 
				tr("Add a note to selected component"),
				QIcon(":/Resources/Icons/.png"));
	m_removeNoteCommand.SetVisuals(
				tr("&Remove Note"), 
				tr("&Remove Note"), 
				tr("Remove the note from selected component"),
				QIcon(":/Resources/Icons/.png"));
}


// protected slots

void CRegistryViewComp::OnComponentViewSelected(CComponentView* componentViewPtr, bool isSelected)
{
	CRegistryView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	CComponentView* selectedComponentPtr = viewPtr->GetSelectedComponent();

	// detach last model from its observers:
	if (selectedComponentPtr != NULL){
		for (int observerIndex = 0; observerIndex < m_registryElementObserversCompPtr.GetCount(); observerIndex++){
			imod::IObserver* observerPtr = m_registryElementObserversCompPtr[observerIndex];
			if (observerPtr != NULL){
				selectedComponentPtr->DetachObserver(observerPtr);
			}
		}
	}

	if (isSelected){
		viewPtr->SetSelectedComponent(componentViewPtr);

		if (componentViewPtr != NULL && m_registryElementObserversCompPtr.IsValid()){
			for (int observerIndex = 0; observerIndex < m_registryElementObserversCompPtr.GetCount(); observerIndex++){
				imod::IObserver* observerPtr = m_registryElementObserversCompPtr[observerIndex];
				if (observerPtr != NULL){
					componentViewPtr->AttachObserver(observerPtr);
				}
			}

			const icomp::IRegistry::ElementInfo& elementInfo = componentViewPtr->GetElementInfo();

			if (m_quickHelpViewerCompPtr.IsValid()){
				m_quickHelpViewerCompPtr->ShowHelp(elementInfo.address.GetPackageId() + "/" + elementInfo.address.GetComponentId(), &elementInfo.address);
			}
		}
	}
	else{
		viewPtr->SetSelectedComponent(NULL);
	}

	m_removeComponentCommand.setEnabled(isSelected);
	m_renameComponentCommand.setEnabled(isSelected);
	m_exportComponentCommand.setEnabled(isSelected);
	m_exportInterfaceCommand.setEnabled(isSelected);

	UpdateExportInterfaceCommand();
}


void CRegistryViewComp::OnComponentPositionChanged(CComponentView* view, const QPoint& newPosition)
{
	CRegistryView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	IRegistryEditController* geometryProviderPtr = dynamic_cast<IRegistryEditController*>(GetObjectPtr());
	if (geometryProviderPtr != NULL){
		geometryProviderPtr->SetComponentPosition(view->GetComponentName(), i2d::CVector2d(newPosition.x(), newPosition.y()));
	}

	viewPtr->UpdateCompositeGeometry();
}


void CRegistryViewComp::OnRemoveComponent()
{
	CRegistryView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		const CComponentView* selectedComponentPtr = viewPtr->GetSelectedComponent();

		if (selectedComponentPtr != NULL){
			m_removeComponentCommand.setEnabled(false);

			registryPtr->RemoveElementInfo(selectedComponentPtr->GetComponentName());

			viewPtr->RemoveSelectedComponent();
		}
	}
}


void CRegistryViewComp::OnRenameComponent()
{
	CRegistryView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		CComponentView* selectedComponentPtr = const_cast<CComponentView*>(viewPtr->GetSelectedComponent());

		if (selectedComponentPtr != NULL){
			const std::string& oldName = selectedComponentPtr->GetComponentName();

			i2d::CVector2d position(0, 0);
			IRegistryEditController* geometryProviderPtr = dynamic_cast<IRegistryEditController*>(registryPtr);
			if (geometryProviderPtr != NULL){
				position = geometryProviderPtr->GetComponentPosition(oldName);
			}

			const icomp::IRegistry::ElementInfo& oldInfo = selectedComponentPtr->GetElementInfo();

			bool isOk = false;
			std::string newName = QInputDialog::getText(
						NULL,
						tr("ACF Compositor"),
						tr("New component name"),
						QLineEdit::Normal,
						iqt::GetQString(oldName),
						&isOk).toStdString();
			if (isOk && !newName.empty() && (oldName != newName) && oldInfo.elementPtr.IsValid()){
				istd::CChangeNotifier notifier(registryPtr);

				const icomp::IRegistry::ElementInfo* newInfoPtr = registryPtr->InsertElementInfo(newName, oldInfo.address, true);

				if (newInfoPtr != NULL){
					I_ASSERT(newInfoPtr->elementPtr.IsValid());	// InsertElementInfo has to return NULL if element cannot be created with option ensureElementCreated = true

					if (iser::CMemoryReadArchive::CloneObjectByArchive(*oldInfo.elementPtr, *newInfoPtr->elementPtr)){
						icomp::IRegistry::Ids elementIds = registryPtr->GetElementIds();
						for (		icomp::IRegistry::Ids::iterator compIdIter = elementIds.begin();
									compIdIter != elementIds.end();
									++compIdIter){
							const std::string& componentId = *compIdIter;
							const icomp::IRegistry::ElementInfo* infoPtr = registryPtr->GetElementInfo(componentId);
							if (infoPtr == NULL){
								continue;
							}
							const icomp::IRegistryElement* elementPtr = infoPtr->elementPtr.GetPtr();
							if (elementPtr == NULL){
								continue;
							}

							icomp::IRegistryElement::Ids attrIds = elementPtr->GetAttributeIds();

							for (		icomp::IRegistryElement::Ids::iterator attrIdIter = attrIds.begin();
										attrIdIter != attrIds.end();
										++attrIdIter){
								const std::string& attributeId = *attrIdIter;
								const icomp::IRegistryElement::AttributeInfo* attrInfoPtr = elementPtr->GetAttributeInfo(attributeId);
								if (attrInfoPtr == NULL){
									continue;
								}

								iser::ISerializable* attributePtr = attrInfoPtr->attributePtr.GetPtr();
								icomp::TSingleAttribute<std::string>* singleAttrPtr = dynamic_cast<icomp::TSingleAttribute<std::string>*>(attributePtr);
								icomp::TMultiAttribute<std::string>* multiAttrPtr = dynamic_cast<icomp::TMultiAttribute<std::string>*>(attributePtr);

								if (		(dynamic_cast<icomp::CReferenceAttribute*>(attributePtr) != NULL) ||
											(dynamic_cast<icomp::CFactoryAttribute*>(attributePtr) != NULL)){
									if (singleAttrPtr->GetValue() == oldName){
										singleAttrPtr->SetValue(newName);
									}
								}

								if (		(dynamic_cast<icomp::CMultiReferenceAttribute*>(attributePtr) != NULL) ||
											(dynamic_cast<icomp::CMultiFactoryAttribute*>(attributePtr) != NULL)){
									int valuesCount = multiAttrPtr->GetValuesCount();
									for (int i = 0; i < valuesCount; ++i){
										if (multiAttrPtr->GetValueAt(i) == oldName){
											multiAttrPtr->SetValueAt(i, newName);
										}
									}
								}
							}
						}

						registryPtr->RemoveElementInfo(oldName);

						if (geometryProviderPtr != NULL){
							geometryProviderPtr->SetComponentPosition(newName, position);
						}

						selectedComponentPtr->setPos(position.GetX(), position.GetY());
						selectedComponentPtr->SetElementInfo(newInfoPtr);

						return;
					}

					registryPtr->RemoveElementInfo(newName);
				}
			}
		}
	}
}


void CRegistryViewComp::OnExportInterface()
{
	CRegistryView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		const CComponentView* selectedComponentPtr = viewPtr->GetSelectedComponent();
		if (selectedComponentPtr != NULL){
			const std::string& componentName = selectedComponentPtr->GetComponentName();

			bool doExport = !HasExportedInterfaces(*selectedComponentPtr);

			registryPtr->SetElementExported(componentName, istd::CClassInfo(), doExport);

			OnExecutionTimerTick();
		}
	}
}


void CRegistryViewComp::OnProperties()
{	
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr == NULL){
		return;
	}

	iqtgui::TDesignerBasicGui<Ui::CRegistryPropertiesDialog, QDialog> dialog;

	dialog.DescriptionEdit->setText(iqt::GetQString(registryPtr->GetDescription()));
	dialog.KeywordsEdit->setText(iqt::GetQString(registryPtr->GetKeywords()));

	if (dialog.exec() == QDialog::Accepted){
		registryPtr->SetDescription(iqt::GetCString(dialog.DescriptionEdit->text()));
		registryPtr->SetKeywords(iqt::GetCString(dialog.KeywordsEdit->text()));
	}
}


void CRegistryViewComp::OnExportComponent()
{
	// TODO: Implement component export
}


void CRegistryViewComp::OnExportToCode()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (		(registryPtr != NULL) &&
				m_registryCodeSaverCompPtr.IsValid()){
		QString filter = tr("C++ code file (*.cpp)");
		QString file = QFileDialog::getSaveFileName(NULL, tr("Export registry to code"), "", filter);
		if (!file.isEmpty()){
			if (m_registryCodeSaverCompPtr->SaveToFile(*registryPtr, iqt::GetCString(file)) == iser::IFileLoader::StateFailed){
				QMessageBox::warning(GetQtWidget(), tr("Error"), tr("Cannot export to file\n%1").arg(file));
			}
		}
	}
}


void CRegistryViewComp::OnExecute()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (		(registryPtr != NULL) &&
				m_registryPreviewCompPtr.IsValid()){
		bool retVal = m_registryPreviewCompPtr->StartRegistry(*registryPtr);
		if (retVal){
			OnExecutionTimerTick();
		}
	}
}


void CRegistryViewComp::OnAbort()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (		(registryPtr != NULL) &&
				m_registryPreviewCompPtr.IsValid()){
		m_registryPreviewCompPtr->AbortRegistry();

		OnExecutionTimerTick();
	}
}


void CRegistryViewComp::OnAddNote()
{
	CRegistryView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}
	
	const CComponentView* selectedComponentPtr = viewPtr->GetSelectedComponent();	
	I_ASSERT(selectedComponentPtr != NULL);

	IRegistryEditController* providerPtr = dynamic_cast<IRegistryEditController*>(GetObjectPtr());
	if (providerPtr != NULL){
		providerPtr->SetComponentNote(selectedComponentPtr->GetComponentName(), istd::CString());
	}
}


void CRegistryViewComp::OnRemoveNote()
{
}


bool CRegistryViewComp::ProcessDroppedData(const QMimeData& data, QGraphicsSceneDragDropEvent* eventPtr)
{
	QByteArray byteData = data.data("component");
	iser::CMemoryReadArchive archive(byteData.constData(), byteData.size());

	icomp::CComponentAddress address;

	i2d::CVector2d position(0, 0);
	if (eventPtr != NULL){
		position = iqt::GetCVector2d(eventPtr->pos());
	}

	return address.Serialize(archive) && TryCreateComponent(address, position);
}


void CRegistryViewComp::UpdateConnectors()
{
	CRegistryView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	viewPtr->RemoveAllConnectors();

	CRegistryView::ComponentViewList componentViews = viewPtr->GetComponentViews();

	foreach(CComponentView* componentItemPtr, componentViews){
		const icomp::IRegistry::ElementInfo& elementInfo = componentItemPtr->GetElementInfo();

		icomp::IRegistryElement::Ids attributeIds = elementInfo.elementPtr->GetAttributeIds();

		for (		icomp::IRegistryElement::Ids::const_iterator iter = attributeIds.begin(); 
					iter != attributeIds.end(); 
					iter++){
			std::string attributeId = *iter;

			const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = elementInfo.elementPtr->GetAttributeInfo(attributeId);
			if (attributeInfoPtr != NULL){
				iser::ISerializable* attributePtr = attributeInfoPtr->attributePtr.GetPtr();
				const icomp::CReferenceAttribute* referenceAttributePtr = dynamic_cast<icomp::CReferenceAttribute*>(attributePtr);
				if (referenceAttributePtr != NULL){		
					const std::string& componentId = referenceAttributePtr->GetValue();
				
					viewPtr->CreateConnector(*componentItemPtr, componentId);
				}
	
				const icomp::CMultiReferenceAttribute* multiReferenceAttributePtr = dynamic_cast<icomp::CMultiReferenceAttribute*>(attributePtr);
				if (multiReferenceAttributePtr != NULL){
					for (int referenceIndex = 0; referenceIndex < multiReferenceAttributePtr->GetValuesCount(); referenceIndex++){
						const std::string& componentId = multiReferenceAttributePtr->GetValueAt(referenceIndex);
						
						viewPtr->CreateConnector(*componentItemPtr, componentId);
					}
				}

				const icomp::CFactoryAttribute* factoryAttributePtr = dynamic_cast<icomp::CFactoryAttribute*>(attributePtr);
				if (factoryAttributePtr != NULL){		
					const std::string& componentId = factoryAttributePtr->GetValue();
				
					viewPtr->CreateConnector(*componentItemPtr, componentId);
				}
	
				const icomp::CMultiFactoryAttribute* multiFactoryAttributePtr = dynamic_cast<icomp::CMultiFactoryAttribute*>(attributePtr);
				if (multiFactoryAttributePtr != NULL){
					for (int referenceIndex = 0; referenceIndex < multiFactoryAttributePtr->GetValuesCount(); referenceIndex++){
						const std::string& componentId = multiFactoryAttributePtr->GetValueAt(referenceIndex);
						
						viewPtr->CreateConnector(*componentItemPtr, componentId);
					}
				}
			}
		}
	}
}


void CRegistryViewComp::OnExecutionTimerTick()
{
	bool isExecutable = false;
	bool isRunning = false;

	if (m_registryPreviewCompPtr.IsValid()){
		isRunning = m_registryPreviewCompPtr->IsRunning();

		icomp::IRegistry* registryPtr = GetObjectPtr();
		if (registryPtr != NULL){
			const icomp::IRegistry::ExportedInterfacesMap& interfacesMap = registryPtr->GetExportedInterfacesMap();
			isExecutable = (interfacesMap.find(istd::CClassInfo::GetInfo<ibase::IApplication>()) != interfacesMap.end());
		}
	}

	m_executeRegistryCommand.setEnabled(!isRunning && isExecutable);
	m_abortRegistryCommand.setEnabled(isRunning);
}


// private methods

void CRegistryViewComp::ConnectReferences(const QString& componentRole)
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr == NULL){
		return;
	}

	icomp::IRegistry::Ids componentIds = registryPtr->GetElementIds();

	for (		icomp::IRegistry::Ids::const_iterator iter = componentIds.begin();
				iter != componentIds.end();
				iter++){

		const icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->GetElementInfo(*iter);
		I_ASSERT(elementInfoPtr != NULL);
		I_ASSERT(elementInfoPtr->elementPtr.IsValid());

		icomp::IRegistryElement* registryElementPtr = elementInfoPtr->elementPtr.GetPtr();
		const icomp::IComponentStaticInfo& elementStaticInfo = registryElementPtr->GetComponentStaticInfo();
		const icomp::IComponentStaticInfo::AttributeInfos staticAttributes = elementStaticInfo.GetAttributeInfos();
	
		for (int staticAttributeIndex = 0; staticAttributeIndex < staticAttributes.GetElementsCount(); staticAttributeIndex++){
			const std::string& attributeId = staticAttributes.GetKeyAt(staticAttributeIndex);
			const icomp::IAttributeStaticInfo* staticAttributeInfoPtr = staticAttributes.GetValueAt(staticAttributeIndex);
			I_ASSERT(staticAttributeInfoPtr != NULL);

			const iser::ISerializable* attributePtr = staticAttributeInfoPtr->GetAttributeDefaultValue();
			const icomp::CReferenceAttribute* referenceAttributePtr = dynamic_cast<const icomp::CReferenceAttribute*>(attributePtr);
			const icomp::CFactoryAttribute* factoryAttributePtr = dynamic_cast<const icomp::CFactoryAttribute*>(attributePtr);
			const icomp::CMultiReferenceAttribute* multiReferenceAttributePtr = dynamic_cast<const icomp::CMultiReferenceAttribute*>(attributePtr);
			const icomp::CMultiFactoryAttribute* multiFactoryAttributePtr = dynamic_cast<const icomp::CMultiFactoryAttribute*>(attributePtr);

			bool createAttribute = false;

			if (referenceAttributePtr != NULL && referenceAttributePtr->GetValue() == componentRole.toStdString()){
				createAttribute = true;
			}
			else if (factoryAttributePtr != NULL && factoryAttributePtr->GetValue() == componentRole.toStdString()){
				createAttribute = true;
			}
			else if (multiReferenceAttributePtr != NULL){
				for (int valueIndex = 0; valueIndex < multiReferenceAttributePtr->GetValuesCount(); valueIndex++){
					if (multiReferenceAttributePtr->GetValueAt(valueIndex) == componentRole.toStdString()){
						createAttribute = true;
						break;
					}
				}
			}
			else if (multiFactoryAttributePtr != NULL){
				for (int valueIndex = 0; valueIndex < multiFactoryAttributePtr->GetValuesCount(); valueIndex++){
					if (multiFactoryAttributePtr->GetValueAt(valueIndex) == componentRole.toStdString()){
						createAttribute = true;
						break;
					}
				}
			}

			const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = registryElementPtr->GetAttributeInfo(attributeId);
			if (attributeInfoPtr == NULL && createAttribute){
				attributeInfoPtr = registryElementPtr->InsertAttributeInfo(attributeId);
			}
		}
	}
}

bool CRegistryViewComp::HasExportedInterfaces(const CComponentView& componentView) const
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		const icomp::IRegistry::ExportedInterfacesMap& interfacesMap = registryPtr->GetExportedInterfacesMap();
		for (		icomp::IRegistry::ExportedInterfacesMap::const_iterator iter = interfacesMap.begin();
					iter != interfacesMap.end();
					++iter){
			if (iter->second == componentView.GetComponentName()){
				return true;
			}
		}
	}

	return false;
}


void CRegistryViewComp::UpdateExportInterfaceCommand()
{
	CRegistryView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	const CComponentView* selectedComponentPtr = viewPtr->GetSelectedComponent();
	if ((selectedComponentPtr != NULL) && HasExportedInterfaces(*selectedComponentPtr)){
		m_exportInterfaceCommand.SetVisuals(
				tr("&Remove Inteface Export"), 
				tr("&Remove Inteface Export"), 
				tr("Remove Inteface Export"),
				QIcon(":/Resources/Icons/Export.png"));
	}
	else{
		m_exportInterfaceCommand.SetVisuals(
				tr("&Export Inteface(s)"), 
				tr("&Export Interface(s)"), 
				tr("Export interface(s)"),
				QIcon(":/Resources/Icons/Export.png"));
	}
}


} // namespace icmpstr


