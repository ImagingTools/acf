#include "iqtprm/CVariableParamGuiComp.h"


#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"
#include "imod/IObserver.h"

#include "iqt/CSignalBlocker.h"


namespace iqtprm
{


CVariableParamGuiComp::CVariableParamGuiComp()
:	m_lastParamModelPtr(NULL),
	m_currentTypeIndex(-1)
{
}


// reimplemented (imod::IModelEditor)

void CVariableParamGuiComp::UpdateModel() const
{
}


void CVariableParamGuiComp::UpdateEditor(int updateFlags)
{
	I_ASSERT(m_currentTypeIndex < m_typeIdsAttrPtr.GetCount());

	if (!IsGuiCreated()){
		return;
	}

	iprm::IVariableParam* variableParamPtr = GetObjectPtr();
	if (variableParamPtr == NULL){
		return;
	}

	std::string currentId;
	if (m_currentTypeIndex >= 0){
		currentId = m_typeIdsAttrPtr[m_currentTypeIndex].ToString();
	}

	std::string typeId = variableParamPtr->GetParameterTypeId();
	if (typeId != currentId){
		DetachCurrentType();

		int typeIdsCount = m_typeIdsAttrPtr.GetCount();
		for (int i = 0; i < typeIdsCount; ++i){
			if (typeId == m_typeIdsAttrPtr[i].ToString()){
				m_currentTypeIndex = i;

				AttachCurrentType();

				break;
			}
		}

		TypeSelectorCB->setCurrentIndex(m_currentTypeIndex);
	}

	if (		(m_lastParamModelPtr != NULL) &&
				(m_currentTypeIndex >= 0) &&
				(m_currentTypeIndex < m_editorsCompPtr.GetCount())){
		imod::IModelEditor* editorPtr = m_editorsCompPtr[m_currentTypeIndex];
		if (editorPtr != NULL){
			editorPtr->UpdateEditor(updateFlags);
		}
	}

	if (IsGuiCreated()){
		TypeSelectorCB->setVisible(m_currentTypeIndex < 0);
	}
}


// reimplemented (imod::IObserver)

bool CVariableParamGuiComp::OnDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnDetached(modelPtr)){
		DetachCurrentType();

		return true;
	}

	return false;
}


// reimplemented (iqtgui::CGuiComponentBase)

void CVariableParamGuiComp::OnGuiCreated()
{
	iqt::CSignalBlocker block(TypeSelectorCB);

	TypeSelectorCB->clear();

	int typeIdsCount = m_typeIdsAttrPtr.GetCount();
	for (int i = 0; i < typeIdsCount; ++i){
		istd::CString typeName = (i < m_typeNamesAttrPtr.GetCount())? m_typeNamesAttrPtr[i]: m_typeIdsAttrPtr[i];
		TypeSelectorCB->addItem(iqt::GetQString(typeName));
	}

	TypeSelectorCB->setCurrentIndex(m_currentTypeIndex);

	BaseClass::OnGuiCreated();
}


void CVariableParamGuiComp::OnGuiDestroyed()
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


// reimplemented (iqt2d::ISceneExtender)

void CVariableParamGuiComp::AddItemsToScene(iqt2d::ISceneProvider* providerPtr, int flags)
{
	I_ASSERT(providerPtr != NULL);

	m_connectedSceneFlags[providerPtr] = flags;

	if ((m_currentTypeIndex >= 0) && (m_currentTypeIndex < m_extendersCompPtr.GetCount())){
		iqt2d::ISceneExtender* extenderPtr = m_extendersCompPtr[m_currentTypeIndex];
		if (extenderPtr != NULL){
			extenderPtr->AddItemsToScene(providerPtr, flags);
		}
	}
}


void CVariableParamGuiComp::RemoveItemsFromScene(iqt2d::ISceneProvider* providerPtr)
{
	I_ASSERT(providerPtr != NULL);

	if ((m_currentTypeIndex >= 0) && (m_currentTypeIndex < m_extendersCompPtr.GetCount())){
		iqt2d::ISceneExtender* extenderPtr = m_extendersCompPtr[m_currentTypeIndex];
		if (extenderPtr != NULL){
			extenderPtr->RemoveItemsFromScene(providerPtr);
		}
	}

	m_connectedSceneFlags.erase(providerPtr);
}


// protected methods

void CVariableParamGuiComp::AttachCurrentType()
{
	if (m_currentTypeIndex >= 0){
		iprm::IVariableParam* variableParamPtr = GetObjectPtr();
		if (variableParamPtr != NULL){
			imod::IModel* paramModelPtr = dynamic_cast<imod::IModel*>(variableParamPtr->GetParameter());
			if ((paramModelPtr != NULL) && (m_currentTypeIndex < m_observersCompPtr.GetCount())){
				imod::IObserver* observerPtr = m_observersCompPtr[m_currentTypeIndex];
				if ((observerPtr != NULL) && paramModelPtr->AttachObserver(observerPtr)){
					m_lastParamModelPtr = paramModelPtr;
				}
			}
		}

		if (m_currentTypeIndex < m_guisCompPtr.GetCount()){
			iqtgui::IGuiObject* guiPtr = m_guisCompPtr[m_currentTypeIndex];
			if (guiPtr != NULL){
				guiPtr->CreateGui(ParamFrame);
			}
		}

		if (m_currentTypeIndex < m_extendersCompPtr.GetCount()){
			iqt2d::ISceneExtender* extenderPtr = m_extendersCompPtr[m_currentTypeIndex];
			if (extenderPtr != NULL){
				for (		ConnectedSceneFlags::const_iterator sceneIter = m_connectedSceneFlags.begin();
							sceneIter != m_connectedSceneFlags.end();
							++sceneIter){
					iqt2d::ISceneProvider* providerPtr = sceneIter->first;
					I_ASSERT(providerPtr != NULL);

					extenderPtr->AddItemsToScene(providerPtr, sceneIter->second);
				}
			}
		}
	}
}


void CVariableParamGuiComp::DetachCurrentType()
{
	if (m_currentTypeIndex >= 0){
		if ((m_currentTypeIndex >= 0) && (m_currentTypeIndex < m_extendersCompPtr.GetCount())){
			iqt2d::ISceneExtender* extenderPtr = m_extendersCompPtr[m_currentTypeIndex];
			if (extenderPtr != NULL){
				for (		ConnectedSceneFlags::const_iterator sceneIter = m_connectedSceneFlags.begin();
							sceneIter != m_connectedSceneFlags.end();
							++sceneIter){
					iqt2d::ISceneProvider* providerPtr = sceneIter->first;
					I_ASSERT(providerPtr != NULL);

					extenderPtr->RemoveItemsFromScene(providerPtr);
				}
			}
		}

		if (IsGuiCreated()){
			if (m_currentTypeIndex < m_guisCompPtr.GetCount()){
				iqtgui::IGuiObject* guiPtr = m_guisCompPtr[m_currentTypeIndex];
				if ((guiPtr != NULL) && guiPtr->IsGuiCreated()){
					guiPtr->DestroyGui();
				}
			}
		}

		if ((m_lastParamModelPtr != NULL) && (m_currentTypeIndex < m_observersCompPtr.GetCount())){
			imod::IObserver* observerPtr = m_observersCompPtr[m_currentTypeIndex];
			if ((observerPtr != NULL) && observerPtr->IsModelAttached(m_lastParamModelPtr)){
				m_lastParamModelPtr->DetachObserver(observerPtr);
			}
		}

		m_currentTypeIndex = -1;
		m_lastParamModelPtr = NULL;
	}
}



// protected slots

void CVariableParamGuiComp::on_TypeSelectorCB_currentIndexChanged(int index)
{
	if (index == m_currentTypeIndex){
		return;
	}

	istd::TChangeNotifier<iprm::IVariableParam> variableParamPtr(GetObjectPtr());
	if (!variableParamPtr.IsValid()){
		return;
	}

	if ((index >= 0) && (index < m_typeIdsAttrPtr.GetCount())){
		variableParamPtr->AssignTypeId(m_typeIdsAttrPtr[index].ToString());
	}
}


} // namespace iqtprm


