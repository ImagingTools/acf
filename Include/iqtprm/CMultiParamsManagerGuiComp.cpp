#include "iqtprm/CMultiParamsManagerGuiComp.h"


// ACF includes
#include <iprm/IParamsSet.h>


namespace iqtprm
{


// public methods
	
// reimplemented (iqt2d::IViewExtender)

void CMultiParamsManagerGuiComp::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	Q_ASSERT(providerPtr != NULL);

	m_connectedSceneFlags[providerPtr] = flags;

	iqt2d::IViewExtender* extenderPtr = GetCurrentViewExtenderPtr();
	if (extenderPtr != NULL){
		extenderPtr->AddItemsToScene(providerPtr, flags);
	}
}


void CMultiParamsManagerGuiComp::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	Q_ASSERT(providerPtr != NULL);

	iqt2d::IViewExtender* extenderPtr = GetCurrentViewExtenderPtr();
	if (extenderPtr != NULL){
		extenderPtr->RemoveItemsFromScene(providerPtr);
	}

	m_connectedSceneFlags.remove(providerPtr);
}


// protected methods

// reimplemented (CMultiParamsManagerGuiCompBase)

imod::IObserver* CMultiParamsManagerGuiComp::GetObserverPtr(const iprm::IParamsSet* paramsSetPtr) const
{
	if (paramsSetPtr == NULL){
		return NULL;
	}

	QByteArray paramTypeId = paramsSetPtr->GetFactoryId();

	int foundTypeIndex = -1;
	int registeredTypesCount = m_paramsSetTypeIdsAttrPtr.GetCount();

	for (int typeIndex = 0; typeIndex < registeredTypesCount; typeIndex++){
		QByteArray typeId = m_paramsSetTypeIdsAttrPtr[typeIndex];

		if (typeId == paramTypeId){
			foundTypeIndex = typeIndex;
			break;
		}
	}

	if ((foundTypeIndex < 0) || (foundTypeIndex >= m_paramsObserverCompPtr.GetCount())){
		return NULL;
	}

	return m_paramsObserverCompPtr[foundTypeIndex];
}


iqtgui::IGuiObject* CMultiParamsManagerGuiComp::GetEditorGuiPtr(const iprm::IParamsSet* paramsSetPtr) const
{
	if (paramsSetPtr == NULL){
		return NULL;
	}

	QByteArray paramTypeId = paramsSetPtr->GetFactoryId();

	int foundTypeIndex = -1;
	int registeredTypesCount = m_paramsSetTypeIdsAttrPtr.GetCount();

	for (int typeIndex = 0; typeIndex < registeredTypesCount; typeIndex++){
		QByteArray typeId = m_paramsSetTypeIdsAttrPtr[typeIndex];

		if (typeId == paramTypeId){
			foundTypeIndex = typeIndex;
			break;
		}
	}

	if ((foundTypeIndex < 0) || (foundTypeIndex >= m_paramsGuiCompPtr.GetCount())){
		return NULL;
	}

	return m_paramsGuiCompPtr[foundTypeIndex];
}


void CMultiParamsManagerGuiComp::UpdateParamsView(int selectedIndex)
{
	BaseClass::UpdateParamsView(selectedIndex);

	QByteArray typeId = GetSelectedParamsSetTypeId();

	int stackPosition = m_typeToStackPositionMap.value(typeId, -1);

	m_parameterEditorStack->setCurrentIndex(stackPosition);
}


// reimplemented (imod::CSingleModelObserverBase)

void CMultiParamsManagerGuiComp::BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if ((updateFlags & iprm::ISelectionParam::CF_SELECTION_CHANGED) != 0){
		DetachCurrentExtender();
	}

	BaseClass::BeforeUpdate(modelPtr, updateFlags, updateParamsPtr);
}


void CMultiParamsManagerGuiComp::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if ((updateFlags & iprm::ISelectionParam::CF_SELECTION_CHANGED) != 0){
		AttachCurrentExtender();
	}

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


// reimplemented (iqtgui::CComponentBase)

void CMultiParamsManagerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	m_parameterEditorStack = new QStackedWidget(ParamsFrame);
	QLayout* layoutPtr = ParamsFrame->layout();
	if (layoutPtr != NULL){
		layoutPtr->addWidget(m_parameterEditorStack);
	}

	int editorsCount = qMin(m_paramsGuiCompPtr.GetCount(), m_paramsSetTypeIdsAttrPtr.GetCount());
	for (int editorIndex = 0; editorIndex < editorsCount; editorIndex++){
		iqtgui::IGuiObject* guiPtr = m_paramsGuiCompPtr[editorIndex];
		if (guiPtr != NULL){
			QWidget* stackPage = new QWidget(m_parameterEditorStack);
			QVBoxLayout* pageLayoutPtr = new QVBoxLayout(stackPage);
			pageLayoutPtr->setMargin(0);

			if (guiPtr->CreateGui(stackPage)){
				m_parameterEditorStack->insertWidget(editorIndex + 1, stackPage);

				m_widgetToGuiMap[stackPage] = guiPtr;

				QByteArray typeId = m_paramsSetTypeIdsAttrPtr[editorIndex];
				m_typeToStackPositionMap[typeId] = editorIndex;
			}
		}
	}

	int viewExtendersCount = qMin(m_viewExtendersCompPtr.GetCount(), m_paramsSetTypeIdsAttrPtr.GetCount());
	for (int viewExtendersIndex = 0; viewExtendersIndex < viewExtendersCount; viewExtendersIndex++){
		QByteArray typeId = m_paramsSetTypeIdsAttrPtr[viewExtendersIndex];
		iqt2d::IViewExtender* extenderPtr = m_viewExtendersCompPtr[viewExtendersIndex];
		
		m_typeToExtenderMap[typeId] = extenderPtr;
	}

}


void CMultiParamsManagerGuiComp::OnGuiDestroyed()
{
	int pagesCount = m_parameterEditorStack->count();

	for (int pageIndex = 1; pageIndex < pagesCount; pageIndex++){
		QWidget* pagePtr = m_parameterEditorStack->widget(pageIndex);

		WidgetGuiMap::iterator foundGuiIter = m_widgetToGuiMap.find(pagePtr);
		Q_ASSERT(foundGuiIter != m_widgetToGuiMap.end());
		if (foundGuiIter != m_widgetToGuiMap.end()){
			foundGuiIter.value()->DestroyGui();
		}
	}

	while (m_parameterEditorStack->count() > 1){
		m_parameterEditorStack->removeWidget(m_parameterEditorStack->widget(m_parameterEditorStack->count() - 1));
	}

	m_widgetToGuiMap.clear();

	BaseClass::OnGuiDestroyed();
}


// private methods

iqt2d::IViewExtender* CMultiParamsManagerGuiComp::GetCurrentViewExtenderPtr() const
{
	QByteArray selectedParameterTypeId = GetSelectedParamsSetTypeId();
	
	return m_typeToExtenderMap.value(selectedParameterTypeId, NULL);
}


void CMultiParamsManagerGuiComp::AttachCurrentExtender()
{
	DetachCurrentExtender();

	iqt2d::IViewExtender* extenderPtr = GetCurrentViewExtenderPtr();
	if (extenderPtr != NULL){
		for (		ConnectedSceneFlags::const_iterator sceneIter = m_connectedSceneFlags.begin();
					sceneIter != m_connectedSceneFlags.end();
					++sceneIter){
			iqt2d::IViewProvider* providerPtr = sceneIter.key();
			Q_ASSERT(providerPtr != NULL);

			extenderPtr->AddItemsToScene(providerPtr, sceneIter.value());
		}
	}
}


void CMultiParamsManagerGuiComp::DetachCurrentExtender()
{
	iqt2d::IViewExtender* extenderPtr = GetCurrentViewExtenderPtr();
	if (extenderPtr != NULL){
		for (		ConnectedSceneFlags::const_iterator sceneIter = m_connectedSceneFlags.begin();
					sceneIter != m_connectedSceneFlags.end();
					++sceneIter){
			iqt2d::IViewProvider* providerPtr = sceneIter.key();
			Q_ASSERT(providerPtr != NULL);

			extenderPtr->RemoveItemsFromScene(providerPtr);
		}
	}
}

} // namespace iqtprm


