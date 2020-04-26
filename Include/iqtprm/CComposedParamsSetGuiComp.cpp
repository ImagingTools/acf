#include <iqtprm/CComposedParamsSetGuiComp.h>


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QDebug>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QGroupBox>
#else
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolBox>
#include <QtGui/QTabWidget>
#include <QtGui/QGroupBox>
#endif

// ACF includes
#include <imod/IModel.h>
#include <imod/IObserver.h>
#include <iprm/IParameterStateProvider.h>
#include <iprm/IEnableableParam.h>
#include <iqt/CSignalBlocker.h>
#include <iview/IShapeView.h>
#include <iqt2d/IViewProvider.h>


namespace iqtprm
{


CComposedParamsSetGuiComp::CComposedParamsSetGuiComp()
	:m_currentGuiIndex(-1),
	m_guiContainerPtr(NULL)
{
}


// reimplemented (imod::IModelEditor)

void CComposedParamsSetGuiComp::UpdateEditor(const istd::IChangeable::ChangeSet& changeSet)
{
	Q_ASSERT(IsGuiCreated());

	if (!changeSet.Contains(istd::IChangeable::CF_DELEGATED)){
		int editorsCount = m_editorsCompPtr.GetCount();
		for (int i = 0; i < editorsCount; ++i){
			iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];
			if ((guiPtr == NULL) || !guiPtr->IsGuiCreated()){
				continue;
			}

			imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
			if (m_connectedEditorsMap.contains(editorPtr)){
				Q_ASSERT(editorPtr != NULL); // only not NULL editors are stored in m_connectedEditorsMap

				m_connectedEditorsMap[editorPtr] = true;

				editorPtr->UpdateEditor(changeSet);
			}
		}
	}

	const iprm::IParamsSet* paramsSetPtr = GetObservedObject();
	Q_ASSERT(paramsSetPtr != NULL);

	const iprm::IParameterStateProvider* stateProviderPtr = CompCastPtr<iprm::IParameterStateProvider>(paramsSetPtr);
	if (stateProviderPtr != NULL){
		const QList<QByteArray>& observedIds = GetIds();
		const QList<imod::IModelEditor*> editors = GetModelEditors();

		Q_ASSERT(observedIds.size() == editors.size());

		for (int i = 0; i < observedIds.size(); ++i){
			bool editState = stateProviderPtr->GetState(*paramsSetPtr, observedIds[i], iprm::IParameterStateProvider::ST_EDIT);
			if (editors[i] != NULL){
				editors[i]->SetReadOnly(!editState);
			}
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)


void CComposedParamsSetGuiComp::OnGuiCreated()
{
	DesignType guiMode = DesignType(*m_designTypeAttrPtr);

	if (!m_paramsLoaderCompPtr.IsValid()){
		LoaderFrame->hide();
	}

	// initialize the gui container
	QLayout* layoutPtr = ParamsFrame->layout();
	if (layoutPtr == NULL){
		if (*m_useHorizontalLayoutAttrPtr){
			layoutPtr = new QHBoxLayout(ParamsFrame);
		}
		else{
			layoutPtr = new QVBoxLayout(ParamsFrame);
		}
	}
	layoutPtr->setContentsMargins(0, 0, 0, 0);

	switch (guiMode){
	case DT_TAB_WIDGET:
		{
			ParamsFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
			QTabWidget* tabWidgetPtr = new QTabWidget(ParamsFrame);
			tabWidgetPtr->setTabPosition(QTabWidget::TabPosition(*m_tabOrientationAttrPtr));
			QObject::connect(tabWidgetPtr, SIGNAL(currentChanged(int)), this, SLOT(OnEditorChanged(int)));
			layoutPtr->addWidget(tabWidgetPtr);

			m_guiContainerPtr = tabWidgetPtr;
			m_currentGuiIndex = 0;
		}
		break;

	case DT_TOOL_BOX:
		{
			ParamsFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

			QToolBox* toolBoxPtr = new QToolBox(ParamsFrame);
			toolBoxPtr->setBackgroundRole(QPalette::Window);
			QObject::connect(toolBoxPtr, SIGNAL(currentChanged(int)), this, SLOT(OnEditorChanged(int)));
			layoutPtr->addWidget(toolBoxPtr);

			m_guiContainerPtr = toolBoxPtr;
			m_currentGuiIndex = 0;
		}
		break;

	default:
		m_guiContainerPtr = ParamsFrame;
		m_currentGuiIndex = -1;
	}

	// map gui objects with their names (the container will be filled by OnGuiModelAttached())
	int elementsCount = m_guisCompPtr.GetCount();
	for (int i = 0; i < elementsCount; ++i){
		iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];
		if (guiPtr == NULL){
			continue;
		}

		QString name;
		if (i < m_namesAttrPtr.GetCount()){
			name = m_namesAttrPtr[i];
		}

		PanelData& panelData = m_guiToWidgetMap[guiPtr];

		if (guiMode == DT_TAB_WIDGET){
			panelData.pagePtr = new QWidget(m_guiContainerPtr);
			QVBoxLayout* panelLayoutPtr = new QVBoxLayout(panelData.pagePtr);

			panelData.paramWidgetPtr = new QWidget(panelData.pagePtr);
			QVBoxLayout* paramLayoutPtr = new QVBoxLayout(panelData.paramWidgetPtr);
			paramLayoutPtr->setContentsMargins(0, 0, 0, 0);
			panelLayoutPtr->addWidget(panelData.paramWidgetPtr);

			QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Maximum);
			panelLayoutPtr->addItem(spacerPtr);
		}
		else if (guiMode == DT_TOOL_BOX){
			panelData.pagePtr = new QWidget(m_guiContainerPtr);

			QVBoxLayout* panelLayoutPtr = new QVBoxLayout(panelData.pagePtr);
			panelLayoutPtr->setContentsMargins(6, 0, 6, 0);

			panelData.paramWidgetPtr = new QWidget(panelData.pagePtr);
			QVBoxLayout* paramLayoutPtr = new QVBoxLayout(panelData.paramWidgetPtr);
			paramLayoutPtr->setContentsMargins(0, 0, 0, 0);
			panelLayoutPtr->addWidget(panelData.paramWidgetPtr);

			QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Maximum, QSizePolicy::Maximum);
			panelLayoutPtr->addItem(spacerPtr);
		}
		else{
			if (!name.isEmpty()){
				panelData.pagePtr = panelData.paramWidgetPtr = new QGroupBox(name, m_guiContainerPtr);
				new QVBoxLayout(panelData.pagePtr);
			}
			else{
				panelData.pagePtr = panelData.paramWidgetPtr = new QWidget(m_guiContainerPtr);
				QVBoxLayout* panelLayoutPtr = new QVBoxLayout(panelData.pagePtr);
				panelLayoutPtr->setContentsMargins(0, 0, 0, 0);
			}

			m_guiContainerPtr->layout()->addWidget(panelData.pagePtr);
		}
	}

	if (*m_useVerticalSpacerAttrPtr){
		QLayout* containerLayoutPtr = m_guiContainerPtr->layout();
		if (containerLayoutPtr != NULL){
			QSpacerItem* verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

			containerLayoutPtr->addItem(verticalSpacer);
		}
	}

	BaseClass::OnGuiCreated();
}


void CComposedParamsSetGuiComp::OnGuiDestroyed()
{
	int elementsCount = m_guisCompPtr.GetCount();
	for (int i = 0; i < elementsCount; ++i){
		iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];

		if ((guiPtr != NULL) && guiPtr->IsGuiCreated()){
			guiPtr->DestroyGui();
		}
	}

	delete m_guiContainerPtr;

	m_guiContainerPtr = NULL;

	BaseClass::OnGuiDestroyed();
}


// reimplemented (iqt2d::IViewExtender)

void CComposedParamsSetGuiComp::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	Q_ASSERT(providerPtr != NULL);

	m_connectedSceneFlags[providerPtr] = flags;

	AttachToScene(providerPtr, flags);
}


void CComposedParamsSetGuiComp::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	Q_ASSERT(providerPtr != NULL);

	DetachFromScene(providerPtr);

	m_connectedSceneFlags.remove(providerPtr);
}


// reimplemented (iview::IShapeFactory)

iview::IShape* CComposedParamsSetGuiComp::CreateShape(const istd::IChangeable* objectPtr, bool connectToModel) const
{
	const iprm::IParamsSet* paramsSetPtr = dynamic_cast<const iprm::IParamsSet*>(objectPtr);

	if (paramsSetPtr != NULL){
		int elementsCount = qMin(m_shapeFactoriesCompPtr.GetCount(), m_idsAttrPtr.GetCount());
		for (int i = 0; i < elementsCount; ++i){
			const QByteArray& paramId = m_idsAttrPtr[i];
			if (paramId.isEmpty()){
				continue;
			}

			const iview::IShapeFactory* factoryPtr = m_shapeFactoriesCompPtr[i];
			if (factoryPtr == NULL){
				continue;
			}

			const istd::IChangeable* subObjectPtr = objectPtr;
			if (paramId != "*"){
				subObjectPtr = paramsSetPtr->GetParameter(paramId);
			}

			iview::IShape* shapePtr = factoryPtr->CreateShape(subObjectPtr, connectToModel);
			if (shapePtr != NULL){
				return shapePtr;
			}
		}
	}

	return NULL;
}


// protected methods

void CComposedParamsSetGuiComp::AttachToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	Q_ASSERT(m_showAllShapesAttrPtr.IsValid());

	int elementsCount = m_extendersCompPtr.GetCount();

	iview::IShapeView* viewPtr = NULL;

	if ((m_currentGuiIndex >= 0) && !*m_showAllShapesAttrPtr){
		if (m_currentGuiIndex < elementsCount){
			iqt2d::IViewExtender* extenderPtr = m_extendersCompPtr[m_currentGuiIndex];
			if (extenderPtr != NULL){
				extenderPtr->AddItemsToScene(providerPtr, flags);

				viewPtr = providerPtr->GetView();
			}
		}
	}
	else{
		for (int i = 0; i < elementsCount; ++i){
			iqt2d::IViewExtender* extenderPtr = m_extendersCompPtr[i];
			if (extenderPtr != NULL){
				extenderPtr->AddItemsToScene(providerPtr, flags);

				viewPtr = providerPtr->GetView();
			}
		}
	}

	if (viewPtr != NULL){
		viewPtr->Update();
	}
}


void CComposedParamsSetGuiComp::DetachFromScene(iqt2d::IViewProvider* providerPtr)
{
	Q_ASSERT(providerPtr != NULL);

	int elementsCount = m_extendersCompPtr.GetCount();

	iview::IShapeView* viewPtr = NULL;

	if ((m_currentGuiIndex >= 0) && !*m_showAllShapesAttrPtr){
		if (m_currentGuiIndex < elementsCount){
			iqt2d::IViewExtender* extenderPtr = m_extendersCompPtr[m_currentGuiIndex];
			if (extenderPtr != NULL){
				extenderPtr->RemoveItemsFromScene(providerPtr);

				viewPtr = providerPtr->GetView();
			}
		}
	}
	else{
		for (int i = 0; i < elementsCount; ++i){
			iqt2d::IViewExtender* extenderPtr = m_extendersCompPtr[i];
			if (extenderPtr != NULL){
				extenderPtr->RemoveItemsFromScene(providerPtr);

				viewPtr = providerPtr->GetView();
			}
		}
	}

	if (viewPtr != NULL){
		viewPtr->Update();
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CComposedParamsSetGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iqt::CSignalBlocker blocker(m_guiContainerPtr);

	iprm::IParamsSet* paramsSetPtr = GetObservedObject();
	Q_ASSERT(paramsSetPtr != NULL);

	int guiMode = *m_designTypeAttrPtr;

	bool keepGlobalVisible = false;

	int elementsCount = qMin(m_observersCompPtr.GetCount(), m_idsAttrPtr.GetCount());
	for (int i = 0; i < elementsCount; ++i){
		const QByteArray& paramId = m_idsAttrPtr[i];

		QString name;
		if (i < m_namesAttrPtr.GetCount()){
			name = m_namesAttrPtr[i];
		}

		bool keepVisible = true;

		if (!paramId.isEmpty()){
			imod::IModel* parameterModelPtr = GetObservedModel();
			if (paramId != "*"){
				iser::ISerializable* parameterPtr = paramsSetPtr->GetEditableParameter(paramId);
				parameterModelPtr = dynamic_cast<imod::IModel*>(parameterPtr);
				if (parameterModelPtr == NULL){
					qDebug() << "Parameter with ID: " << paramId << " doesn't support imod::IModel interface and cannot be connected to the UI editor";
				}

				keepVisible = (parameterPtr != NULL);
			}

			imod::IObserver* observerPtr = m_observersCompPtr[i];

			if ((parameterModelPtr != NULL) && (observerPtr != NULL) && !parameterModelPtr->IsAttached(observerPtr)){
				if (parameterModelPtr->AttachObserver(observerPtr)){
					imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
					if (editorPtr != NULL){
						m_connectedEditorsMap[editorPtr] = false;
					}
				}
			}
		}

		iqtgui::IGuiObject* guiObjectPtr = m_guisCompPtr[i];
		if (guiObjectPtr != NULL){
			const PanelData& panelData = m_guiToWidgetMap[guiObjectPtr];

			if ((panelData.pagePtr != NULL) && (panelData.paramWidgetPtr != NULL)){
				iprm::IParamsSet::Ids::const_iterator iter;

				// add or remove gui items to the container
				if (keepVisible){
					if (guiMode == DT_TAB_WIDGET){
						QTabWidget* tabWidgetPtr = static_cast<QTabWidget*>(m_guiContainerPtr);
						tabWidgetPtr->addTab(panelData.pagePtr, name);
					}
					else if (guiMode == DT_TOOL_BOX){
						QToolBox* toolBoxPtr = static_cast<QToolBox*>(m_guiContainerPtr);
						toolBoxPtr->addItem(panelData.pagePtr, name);
					}
					else{
						panelData.pagePtr->setVisible(true);
					}

					QWidget* guiObjectWidgetPtr = guiObjectPtr->GetWidget();
					if (guiObjectWidgetPtr != NULL){
						QLayout* panelLayoutPtr = panelData.paramWidgetPtr->layout();
						if (panelLayoutPtr != NULL){
							panelLayoutPtr->addWidget(guiObjectWidgetPtr);
						}
						else{
							guiObjectWidgetPtr->setParent(panelData.paramWidgetPtr);
						}
					}
					else{
						guiObjectPtr->CreateGui(panelData.paramWidgetPtr);
					}
				}
				else{
					if (guiMode == DT_TAB_WIDGET){
						QTabWidget* tabWidgetPtr = static_cast<QTabWidget*>(m_guiContainerPtr);
						int index = tabWidgetPtr->indexOf(panelData.pagePtr);
						if (index >= 0){
							tabWidgetPtr->removeTab(index);
						}
					}
					else if (guiMode == DT_TOOL_BOX){
						QToolBox* toolBoxPtr = static_cast<QToolBox*>(m_guiContainerPtr);
						int index = toolBoxPtr->indexOf(panelData.pagePtr);
						if (index >= 0){
							toolBoxPtr->removeItem(index);
						}
					}
					else{
						panelData.pagePtr->setVisible(false);
					}
				}
			}
		}

		keepGlobalVisible = keepGlobalVisible || keepVisible;
	}

	// restore selection
	if (guiMode == DT_TAB_WIDGET){
		QTabWidget* tabWidgetPtr = static_cast<QTabWidget*>(m_guiContainerPtr);
		tabWidgetPtr->setCurrentIndex(m_currentGuiIndex);
	}
	else if (guiMode == DT_TOOL_BOX){
		QToolBox* toolBoxPtr = static_cast<QToolBox*>(m_guiContainerPtr);
		toolBoxPtr->setCurrentIndex(m_currentGuiIndex);
	}

	GetWidget()->setVisible(keepGlobalVisible);
}


void CComposedParamsSetGuiComp::OnGuiModelDetached()
{
	iprm::IParamsSet* paramsSetPtr = GetObservedObject();
	Q_ASSERT(paramsSetPtr != NULL);

	iqt::CSignalBlocker blocker(m_guiContainerPtr);

	// clear the gui container
	int guiMode = *m_designTypeAttrPtr;
	if (guiMode == DT_TAB_WIDGET){
		QTabWidget* tabWidget = static_cast<QTabWidget*>(m_guiContainerPtr);
		if (tabWidget != NULL){
			for (int i = tabWidget->count() - 1; i >= 0; i--){
				tabWidget->removeTab(i);
			}
		}
	}
	else if (guiMode == DT_TOOL_BOX){
		QToolBox* toolBox = static_cast<QToolBox*>(m_guiContainerPtr);
		for (int i = toolBox->count() - 1; i >= 0; i--){
			toolBox->removeItem(i);
		}
	}

	m_connectedEditorsMap.clear();

	int elementsCount = qMin(m_observersCompPtr.GetCount(), m_idsAttrPtr.GetCount());
	for (int i = 0; i < elementsCount; ++i){
		const QByteArray& paramId = m_idsAttrPtr[i];
		if (paramId.isEmpty()){
			continue;
		}

		imod::IModel* parameterModelPtr = GetObservedModel();
		if (paramId != "*"){
			iser::ISerializable* parameterPtr = paramsSetPtr->GetEditableParameter(paramId);
			parameterModelPtr = dynamic_cast<imod::IModel*>(parameterPtr);
		}

		imod::IObserver* observerPtr = m_observersCompPtr[i];
		if ((parameterModelPtr != NULL) && (observerPtr != NULL) && parameterModelPtr->IsAttached(observerPtr)){
			parameterModelPtr->DetachObserver(observerPtr);
		}
	}

	BaseClass::OnGuiModelDetached();
}


void CComposedParamsSetGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated() && (GetObservedObject() != NULL));

	int editorsCount = m_editorsCompPtr.GetCount();
	for (int i = 0; i < editorsCount; ++i){
		iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];
		if ((guiPtr == NULL) || !guiPtr->IsGuiCreated()){
			continue;
		}

		imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
		ConnectedEditorsMap::ConstIterator findIter = m_connectedEditorsMap.constFind(editorPtr);
		if (findIter != m_connectedEditorsMap.constEnd() && (findIter.value() == true)){
			Q_ASSERT(editorPtr != NULL); // only not NULL editors are stored in m_connectedEditorsMap

			editorPtr->UpdateModelFromEditor();
		}
	}
}

QList<imod::IModelEditor*> CComposedParamsSetGuiComp::GetModelEditors() const
{
	QList<imod::IModelEditor*> result;
	if (!m_editorsCompPtr.IsValid()){
		return result;
	}

	for (int i = 0, size = m_editorsCompPtr.GetCount(); i < size; ++i){
		result.append(m_editorsCompPtr[i]);
	}

	return result;
}

QList<QByteArray> CComposedParamsSetGuiComp::GetIds() const
{
	QList<QByteArray> result;
	if (!m_idsAttrPtr.IsValid()){
		return result;
	}

	for (int i = 0, size = m_idsAttrPtr.GetCount(); i < size; ++i){
		result.append(m_idsAttrPtr[i]);
	}

	return result;
}

// protected slots

void CComposedParamsSetGuiComp::OnEditorChanged(int index)
{
	if (index != m_currentGuiIndex){
		for (ConnectedSceneFlags::const_iterator iter = m_connectedSceneFlags.begin();
				iter != m_connectedSceneFlags.end();
				++iter){
			iqt2d::IViewProvider* providerPtr = iter.key();

			DetachFromScene(providerPtr);
		}

		m_currentGuiIndex = index;

		for (ConnectedSceneFlags::const_iterator iter = m_connectedSceneFlags.begin();
				iter != m_connectedSceneFlags.end();
				++iter){
			iqt2d::IViewProvider* providerPtr = iter.key();
			int flags = iter.value();

			AttachToScene(providerPtr, flags);
		}
	}
}


void CComposedParamsSetGuiComp::on_LoadParamsButton_clicked()
{
	iprm::IParamsSet* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		m_paramsLoaderCompPtr->LoadFromFile(*objectPtr);
	}
}


void CComposedParamsSetGuiComp::on_SaveParamsButton_clicked()
{
	const iprm::IParamsSet* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		m_paramsLoaderCompPtr->SaveToFile(*objectPtr);
	}
}


} // namespace iqtprm


