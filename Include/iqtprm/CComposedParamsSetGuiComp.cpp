#include "iqtprm/CComposedParamsSetGuiComp.h"


// Qt includes
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolBox>
#include <QtGui/QTabWidget>
#include <QtGui/QGroupBox>

// ACF includes
#include "istd/CChangeDelegator.h"

#include "imod/IModel.h"
#include "imod/IObserver.h"

#include "iqtgui/CWidgetUpdateBlocker.h"

#include "iview/IShapeView.h"

#include "iqt2d/IViewProvider.h"


namespace iqtprm
{


CComposedParamsSetGuiComp::CComposedParamsSetGuiComp()
:	m_currentGuiIndex(-1),
	m_guiContainerPtr(NULL)
{
}


// reimplemented (imod::IModelEditor)

void CComposedParamsSetGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated() && (GetObjectPtr() != NULL));

	int editorsCount = m_editorsCompPtr.GetCount();
	for (int i = 0; i < editorsCount; ++i){
		iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];
		if ((guiPtr == NULL) || !guiPtr->IsGuiCreated()){
			continue;
		}

		imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
		ConnectedEditorsMap::ConstIterator findIter = m_connectedEditorsMap.constFind(editorPtr);
		if (findIter != m_connectedEditorsMap.constEnd() && (findIter.value() == true)){
			I_ASSERT(editorPtr != NULL); // only not NULL editors are stored in m_connectedEditorsMap

			editorPtr->UpdateModel();
		}
	}
}


void CComposedParamsSetGuiComp::UpdateEditor(int updateFlags)
{
	I_ASSERT(IsGuiCreated());

	if ((updateFlags & istd::CChangeDelegator::CF_DELEGATED) == 0){
		int editorsCount = m_editorsCompPtr.GetCount();
		for (int i = 0; i < editorsCount; ++i){
			iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];
			if ((guiPtr == NULL) || !guiPtr->IsGuiCreated()){
				continue;
			}

			imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
			if (m_connectedEditorsMap.contains(editorPtr)){
				I_ASSERT(editorPtr != NULL); // only not NULL editors are stored in m_connectedEditorsMap

				m_connectedEditorsMap[editorPtr] = true;

				editorPtr->UpdateEditor(updateFlags);
			}
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CComposedParamsSetGuiComp::OnGuiCreated()
{
	int guiMode = *m_designTypeAttrPtr;

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
	layoutPtr->setMargin(0);

	QToolBox* toolBoxPtr;
	QTabWidget* tabWidgetPtr;
	switch (guiMode){
	case 2:
		ParamsFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
		m_guiContainerPtr = tabWidgetPtr = new QTabWidget(ParamsFrame);
		QObject::connect(tabWidgetPtr, SIGNAL(currentChanged(int)), this, SLOT(OnEditorChanged(int)));
		layoutPtr->addWidget(tabWidgetPtr);
		m_currentGuiIndex = 0;
		break;

	case 1:
		ParamsFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
		m_guiContainerPtr = toolBoxPtr = new QToolBox(ParamsFrame);
		QObject::connect(toolBoxPtr, SIGNAL(currentChanged(int)), this, SLOT(OnEditorChanged(int)));
		layoutPtr->addWidget(toolBoxPtr);
		m_currentGuiIndex = 0;
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
		else{
			continue;
		}

		m_guiNames[guiPtr] = name;
	}

	if (*m_useVerticalSpacerAttrPtr){
		QSpacerItem* VerticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

		layoutPtr->layout()->addItem(VerticalSpacer);
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

	BaseClass::OnGuiDestroyed();
}


// reimplemented (iqt2d::IViewExtender)

void CComposedParamsSetGuiComp::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	I_ASSERT(providerPtr != NULL);

	m_connectedSceneFlags[providerPtr] = flags;

	AttachToScene(providerPtr, flags);
}


void CComposedParamsSetGuiComp::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	I_ASSERT(providerPtr != NULL);

	DetachFromScene(providerPtr);

	m_connectedSceneFlags.remove(providerPtr);
}


// protected methods

void CComposedParamsSetGuiComp::AttachToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	I_ASSERT(m_showAllShapesAttrPtr.IsValid());

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
	I_ASSERT(providerPtr != NULL);

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


// reimplemented (imod::IObserver)

void CComposedParamsSetGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iqtgui::CWidgetUpdateBlocker blocker(m_guiContainerPtr);

	iprm::IParamsSet* paramsSetPtr = GetObjectPtr();
	I_ASSERT(paramsSetPtr != NULL);

	int guiMode = *m_designTypeAttrPtr;

	int elementsCount = qMin(m_observersCompPtr.GetCount(), m_idsAttrPtr.GetCount());
	for (int i = 0; i < elementsCount; ++i){
		const QByteArray& paramId = m_idsAttrPtr[i];

		bool keepVisible = true;

		imod::IModel* parameterModelPtr = GetModelPtr();
		if (!paramId.isEmpty() && (paramId != "*")){
			iser::ISerializable* parameterPtr = paramsSetPtr->GetEditableParameter(paramId);
			parameterModelPtr = dynamic_cast<imod::IModel*>(parameterPtr);

			keepVisible = (parameterPtr != NULL);
		}

		imod::IObserver* observerPtr = m_observersCompPtr[i];

		if ((parameterModelPtr != NULL) && (observerPtr != NULL) && parameterModelPtr->AttachObserver(observerPtr)){
			imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
			if (editorPtr != NULL){
				m_connectedEditorsMap[editorPtr] = false;
			}
		}

		iqtgui::IGuiObject* guiObject = dynamic_cast<iqtgui::IGuiObject*>(m_observersCompPtr[i]);
		if (guiObject){
			iprm::IParamsSet::Ids::const_iterator iter;

			bool addSpacer = false;

			// add or remove gui items to the container
			if (keepVisible){
				QString name = m_guiNames[guiObject];
				QWidget* panelPtr;
				if (guiMode == 2){
					panelPtr = new QWidget(m_guiContainerPtr);
					new QVBoxLayout(panelPtr);
					QTabWidget* tabWidgetPtr = static_cast<QTabWidget*> (m_guiContainerPtr);
					tabWidgetPtr->addTab(panelPtr, name);

					addSpacer = true;
				}
				else if (guiMode == 1){
					panelPtr = new QWidget(m_guiContainerPtr);
					QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
					panelLayoutPtr->setContentsMargins(6, 0, 6, 0);
					QToolBox* toolBoxPtr = static_cast<QToolBox*> (m_guiContainerPtr);
					toolBoxPtr->addItem(panelPtr, name);

					addSpacer = true;
				}
				else{
					if (!name.isEmpty()){
						panelPtr = new QGroupBox(name, m_guiContainerPtr);
						new QVBoxLayout(panelPtr);
					}
					else{
						panelPtr = new QWidget(m_guiContainerPtr);
						QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
						panelLayoutPtr->setContentsMargins(0, 0, 0, 0);
					}
					QLayout* parentLayoutPtr = m_guiContainerPtr->layout();
					if (parentLayoutPtr != NULL){
						parentLayoutPtr->addWidget(panelPtr);
					}
				}

				if (guiObject->GetWidget()){
					QLayout* panelLayoutPtr = panelPtr->layout();
					if (panelLayoutPtr != NULL){
						panelLayoutPtr->addWidget(guiObject->GetWidget());
					}
					else{
						guiObject->GetWidget()->setParent(panelPtr);
					}

					if (addSpacer){
						QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
						panelLayoutPtr->addItem(spacerPtr);
					}
				}
				else{
					guiObject->CreateGui(panelPtr);
				}
			}
			else if (guiObject->GetWidget()){
				QWidget* guiWidgetPtr = guiObject->GetWidget();
				QWidget* framePtr = guiWidgetPtr->parentWidget();
				if (framePtr){
					if (guiMode == 2){
						QTabWidget* tabWidgetPtr = static_cast<QTabWidget*> (m_guiContainerPtr);
						int index = tabWidgetPtr->indexOf(framePtr);
						if (index >= 0){
							tabWidgetPtr->removeTab(index);
						}
					}
					else if (guiMode == 1){
						QToolBox* toolBoxPtr = static_cast<QToolBox*> (m_guiContainerPtr);
						int index = toolBoxPtr->indexOf(framePtr);
						if (index >= 0){
							toolBoxPtr->removeItem(index);
						}
					}
					else{
						m_guiContainerPtr->layout()->removeWidget(framePtr);
						framePtr->setParent(NULL);
					}
				}
			}
		}
	}

	// make use of the lastSelectedIndex property
	if (guiMode == 2){
		QTabWidget* tabWidgetPtr = static_cast<QTabWidget*> (m_guiContainerPtr);
		tabWidgetPtr->setCurrentIndex(m_currentGuiIndex);
	}
	else if (guiMode == 1){
		QToolBox* toolBoxPtr = static_cast<QToolBox*> (m_guiContainerPtr);
		toolBoxPtr->setCurrentIndex(m_currentGuiIndex);
	}
}


void CComposedParamsSetGuiComp::OnGuiModelDetached()
{
	iprm::IParamsSet* paramsSetPtr = GetObjectPtr();
	I_ASSERT(paramsSetPtr != NULL);

	iqtgui::CWidgetUpdateBlocker blocker(m_guiContainerPtr);

	// clear the gui container
	int guiMode = *m_designTypeAttrPtr;
	if (guiMode == 2){
		QTabWidget* tabWidget = static_cast<QTabWidget*> (m_guiContainerPtr);
		for (int i = tabWidget->count() - 1; i >= 0; i--){
			tabWidget->removeTab(i);
		}
	}
	else if (guiMode == 1){
		QToolBox* toolBox = static_cast<QToolBox*> (m_guiContainerPtr);
		for (int i = toolBox->count() - 1; i >= 0; i--){
			toolBox->removeItem(i);
		}
	}
	else{
		QLayout* layout = m_guiContainerPtr->layout();
		for (int i = layout->count() - 1; i >= 0; i--){
			layout->removeItem(layout->itemAt(i));
		}
	}

	m_connectedEditorsMap.clear();

	int elementsCount = qMin(m_observersCompPtr.GetCount(), m_idsAttrPtr.GetCount());
	for (int i = 0; i < elementsCount; ++i){
		const QByteArray& paramId = m_idsAttrPtr[i];

		imod::IModel* parameterModelPtr = GetModelPtr();
		if (!paramId.isEmpty() && (paramId != "*")){
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
	iprm::IParamsSet* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		m_paramsLoaderCompPtr->LoadFromFile(*objectPtr);
	}
}


void CComposedParamsSetGuiComp::on_SaveParamsButton_clicked()
{
	iprm::IParamsSet* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		m_paramsLoaderCompPtr->SaveToFile(*objectPtr);
	}
}


} // namespace iqtprm


