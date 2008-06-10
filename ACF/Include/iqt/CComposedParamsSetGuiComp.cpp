#include "iqt/CComposedParamsSetGuiComp.h"


// Qt includes
#include <QVBoxLayout>

#include "imod/IModel.h"
#include "imod/IObserver.h"


namespace iqt
{


// reimplemented (imod::IModelEditor)

void CComposedParamsSetGuiComp::UpdateModel() const
{
	int editorsCount = m_editorsCompPtr.GetCount();
	for (int i = 0; i < editorsCount; ++i){
		imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
		if (editorPtr != NULL){
			editorPtr->UpdateModel();
		}
	}
}


void CComposedParamsSetGuiComp::UpdateEditor()
{
	int editorsCount = m_editorsCompPtr.GetCount();
	for (int i = 0; i < editorsCount; ++i){
		imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
		if (editorPtr != NULL){
			editorPtr->UpdateEditor();
		}
	}
}


// reimplemented (iqt::TGuiObserverWrap)

void CComposedParamsSetGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	I_ASSERT(m_guisCompPtr.GetCount() == m_observersCompPtr.GetCount());	// both multi-references use the same ID.

	QWidget* widgetPtr = GetQtWidget();
	I_ASSERT(widgetPtr != NULL);

	iprm::IParamsSet* paramsSetPtr = GetObjectPtr();
	I_ASSERT(paramsSetPtr != NULL);

	int elementsCount = istd::Min(m_guisCompPtr.GetCount(), m_idsAttrPtr.GetCount());
	for (int i = 0; i < elementsCount; ++i){
		istd::CString paramId = m_idsAttrPtr[i];
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(paramsSetPtr->GetEditableParameter(paramId.ToString()));
		imod::IObserver* observerPtr = m_observersCompPtr[i];
		iqt::IGuiObject* guiPtr = m_guisCompPtr[i];
		if ((modelPtr != NULL) && (observerPtr != NULL)){
			modelPtr->AttachObserver(observerPtr);
		}

		guiPtr->CreateGui(widgetPtr);
	}
}


void CComposedParamsSetGuiComp::OnGuiModelDetached()
{
	I_ASSERT(m_guisCompPtr.GetCount() == m_observersCompPtr.GetCount());	// both multi-references use the same ID.

	I_IF_DEBUG(
		QWidget* widgetPtr = GetQtWidget();
		I_ASSERT(widgetPtr != NULL);
	)

	iprm::IParamsSet* paramsSetPtr = GetObjectPtr();
	I_ASSERT(paramsSetPtr != NULL);

	int elementsCount = istd::Min(m_guisCompPtr.GetCount(), m_idsAttrPtr.GetCount());
	for (int i = 0; i < elementsCount; ++i){
		istd::CString paramId = m_idsAttrPtr[i];
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(paramsSetPtr->GetEditableParameter(paramId.ToString()));
		imod::IObserver* observerPtr = m_observersCompPtr[i];
		iqt::IGuiObject* guiPtr = m_guisCompPtr[i];
		if ((modelPtr != NULL) && (observerPtr != NULL) && modelPtr->IsAttached(observerPtr)){
			modelPtr->DetachObserver(observerPtr);
		}

		guiPtr->DestroyGui();
	}

	BaseClass::OnGuiModelDetached();
}


// reimplemented (iqt::CGuiComponentBase)

void CComposedParamsSetGuiComp::OnGuiCreated()
{
	QWidget* widgetPtr = GetQtWidget();
	if (widgetPtr != NULL){
		QLayout* layoutPtr = new QVBoxLayout;
		layoutPtr->setMargin(0);
		widgetPtr->setLayout(layoutPtr);
	}

	BaseClass::OnGuiCreated();
}


void CComposedParamsSetGuiComp::OnGuiDestroyed()
{
	QWidget* widgetPtr = GetQtWidget();
	if (widgetPtr != NULL){
		QLayout* layoutPtr = widgetPtr->layout();
		widgetPtr->setLayout(NULL);
		delete layoutPtr;
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqt


