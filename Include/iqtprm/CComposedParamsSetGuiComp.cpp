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


namespace iqtprm
{

CComposedParamsSetGuiComp::CComposedParamsSetGuiComp()
:	m_currentGuiIndex(-1)
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
		if (m_connectedEditors.find(editorPtr) == m_connectedEditors.end()){
			continue;
		}
		I_ASSERT(editorPtr != NULL);	// only not NULL editors are stored in m_connectedEditors

		editorPtr->UpdateModel();
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
			if (editorPtr == NULL){
				continue;
			}

			editorPtr->UpdateEditor(updateFlags);
		}
	}
}


// reimplemented (imod::IObserver)

bool CComposedParamsSetGuiComp::OnAttached(imod::IModel* modelPtr)
{
	if (!BaseClass::OnAttached(modelPtr)){
		return false;
	}

	iprm::IParamsSet* paramsSetPtr = GetObjectPtr();
	I_ASSERT(paramsSetPtr != NULL);

	int elementsCount = qMin(m_observersCompPtr.GetCount(), m_idsAttrPtr.GetCount());
	for (int i = 0; i < elementsCount; ++i){
		const QString& paramId = m_idsAttrPtr[i];

		imod::IModel* parameterModelPtr = GetModelPtr();
		if (!paramId.isEmpty() && (paramId != "*")){
			parameterModelPtr = dynamic_cast<imod::IModel*>(paramsSetPtr->GetEditableParameter(paramId.toStdString()));
		}

		imod::IObserver* observerPtr = m_observersCompPtr[i];

		if ((parameterModelPtr != NULL) && (observerPtr != NULL) && parameterModelPtr->AttachObserver(observerPtr)){
			imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
			if (editorPtr != NULL){
				m_connectedEditors.insert(editorPtr);
			}
		}
	}

	return true;
}


bool CComposedParamsSetGuiComp::OnDetached(imod::IModel* modelPtr)
{
	iprm::IParamsSet* paramsSetPtr = GetObjectPtr();
	I_ASSERT(paramsSetPtr != NULL);

	m_connectedEditors.clear();

	int elementsCount = qMin(m_observersCompPtr.GetCount(), m_idsAttrPtr.GetCount());
	for (int i = 0; i < elementsCount; ++i){
		const QString& paramId = m_idsAttrPtr[i];

		imod::IModel* parameterModelPtr = dynamic_cast<imod::IModel*>(paramsSetPtr->GetEditableParameter(paramId.toStdString()));
		imod::IObserver* observerPtr = m_observersCompPtr[i];

		if ((parameterModelPtr != NULL) && (observerPtr != NULL) && parameterModelPtr->IsAttached(observerPtr)){
			parameterModelPtr->DetachObserver(observerPtr);
		}
	}

	return BaseClass::OnDetached(modelPtr);
}


// reimplemented (iqtgui::CGuiComponentBase)

void CComposedParamsSetGuiComp::OnGuiCreated()
{
	QLayout* layoutPtr = ParamsFrame->layout();
	if (layoutPtr == NULL){
		if (*m_useHorizontalLayoutAttrPtr){
			layoutPtr = new QHBoxLayout(ParamsFrame);
		}
		else{
			layoutPtr = new QVBoxLayout(ParamsFrame);
		}
	}

	if (!m_paramsLoaderCompPtr.IsValid()){
		LoaderFrame->hide();
	}

	layoutPtr->setMargin(0);

	if (*m_designTypeAttrPtr == 1){
		ParamsFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

		QToolBox* toolBoxPtr = new QToolBox(ParamsFrame);
		int elementsCount = m_guisCompPtr.GetCount();
		for (int i = 0; i < elementsCount; ++i){
			iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];

			if (guiPtr != NULL){
				QWidget* panelPtr = new QWidget(ParamsFrame);
				QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
				panelLayoutPtr->setContentsMargins(6, 0, 6, 0);
				QString name;
				if (i < m_namesAttrPtr.GetCount()){
					name = m_namesAttrPtr[i];
				}

				guiPtr->CreateGui(panelPtr);

				toolBoxPtr->addItem(panelPtr, name);

				QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

				panelLayoutPtr->addItem(spacerPtr);
			}
		}

		QObject::connect(toolBoxPtr, SIGNAL(currentChanged(int)), this, SLOT(OnEditorChanged(int)));

		layoutPtr->addWidget(toolBoxPtr);

		m_currentGuiIndex = 0;
	}
	else if (*m_designTypeAttrPtr == 2){
		ParamsFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

		QTabWidget* tabWidgetPtr = new QTabWidget(ParamsFrame);
		int elementsCount = m_guisCompPtr.GetCount();
		for (int i = 0; i < elementsCount; ++i){
			iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];

			if (guiPtr != NULL){
				QWidget* panelPtr = new QWidget(tabWidgetPtr);
				QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
				QString name;
				if (i < m_namesAttrPtr.GetCount()){
					name = m_namesAttrPtr[i];
				}

				guiPtr->CreateGui(panelPtr);

				tabWidgetPtr->addTab(panelPtr, name);

				QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

				panelLayoutPtr->addItem(spacerPtr);
			}
		}

		QObject::connect(tabWidgetPtr, SIGNAL(currentChanged(int)), this, SLOT(OnEditorChanged(int)));

		layoutPtr->addWidget(tabWidgetPtr);

		m_currentGuiIndex = 0;
	}
	else{
		int elementsCount = m_guisCompPtr.GetCount();
		for (int i = 0; i < elementsCount; ++i){
			iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];

			QWidget* elementParentPtr = ParamsFrame;

			if (i < m_namesAttrPtr.GetCount()){
				QLayout* parentLayoutPtr = elementParentPtr->layout();

				elementParentPtr = new QGroupBox(m_namesAttrPtr[i], elementParentPtr);
				new QVBoxLayout(elementParentPtr);

				if (parentLayoutPtr != NULL){
					parentLayoutPtr->addWidget(elementParentPtr);
				}
			}

			if (guiPtr != NULL){
				guiPtr->CreateGui(elementParentPtr);
			}
		}

		m_currentGuiIndex = -1;
	}

	if(*m_useVerticalSpacerAttrPtr){
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


// protected slots

void CComposedParamsSetGuiComp::OnEditorChanged(int index)
{
	if (index != m_currentGuiIndex){
		for (		ConnectedSceneFlags::const_iterator iter = m_connectedSceneFlags.begin();
					iter != m_connectedSceneFlags.end();
					++iter){
			iqt2d::IViewProvider* providerPtr = iter.key();

			DetachFromScene(providerPtr);
		}

		m_currentGuiIndex = index;

		for (		ConnectedSceneFlags::const_iterator iter = m_connectedSceneFlags.begin();
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


