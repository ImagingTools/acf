#include <iqtprm/CMultiParamsManagerGuiComp.h>


// ACF includes
#include <iprm/IParamsSet.h>


namespace iqtprm
{


CMultiParamsManagerGuiComp::CMultiParamsManagerGuiComp()
:	m_parameterEditorStackPtr(NULL)
{
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

	int typesCount = qMin(m_paramsSetTypeIdsAttrPtr.GetCount(), m_paramsGuiCompPtr.GetCount());
	for (int typeIndex = 0; typeIndex < typesCount; typeIndex++){
		QByteArray typeId = m_paramsSetTypeIdsAttrPtr[typeIndex];

		if (typeId == paramTypeId){
			return m_paramsGuiCompPtr[typeIndex];
		}
	}

	return NULL;
}


void CMultiParamsManagerGuiComp::UpdateParamsView(int selectedIndex)
{
	BaseClass::UpdateParamsView(selectedIndex);

	QByteArray typeId = GetSelectedParamsSetTypeId();

	int stackPosition = m_typeToStackPositionMap.value(typeId, -1);

	m_parameterEditorStackPtr->setCurrentIndex(stackPosition);
}


iqt2d::IViewExtender* CMultiParamsManagerGuiComp::GetCurrentViewExtenderPtr() const
{
	QByteArray selectedParameterTypeId = GetSelectedParamsSetTypeId();

	return m_typeToExtenderMap.value(selectedParameterTypeId, NULL);
}


// reimplemented (iqtgui::CComponentBase)

void CMultiParamsManagerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	m_parameterEditorStackPtr = new QStackedWidget(ParamsFrame);
	QLayout* layoutPtr = ParamsFrame->layout();
	if (layoutPtr != NULL){
		layoutPtr->addWidget(m_parameterEditorStackPtr);
	}

	int editorsCount = qMin(m_paramsGuiCompPtr.GetCount(), m_paramsSetTypeIdsAttrPtr.GetCount());
	for (int editorIndex = 0; editorIndex < editorsCount; editorIndex++){
		iqtgui::IGuiObject* guiPtr = m_paramsGuiCompPtr[editorIndex];
		if (guiPtr != NULL){
			QWidget* stackPage = new QWidget(m_parameterEditorStackPtr);
			QVBoxLayout* pageLayoutPtr = new QVBoxLayout(stackPage);
			pageLayoutPtr->setMargin(0);

			if (guiPtr->CreateGui(stackPage)){
				m_parameterEditorStackPtr->insertWidget(editorIndex, stackPage);

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
	int pagesCount = m_parameterEditorStackPtr->count();

	for (int pageIndex = 0; pageIndex < pagesCount; pageIndex++){
		QWidget* pagePtr = m_parameterEditorStackPtr->widget(pageIndex);

		WidgetGuiMap::iterator foundGuiIter = m_widgetToGuiMap.find(pagePtr);
		Q_ASSERT(foundGuiIter != m_widgetToGuiMap.end());
		if (foundGuiIter != m_widgetToGuiMap.end()){
			foundGuiIter.value()->DestroyGui();
		}
	}

	while (m_parameterEditorStackPtr->count() > 1){
		m_parameterEditorStackPtr->removeWidget(m_parameterEditorStackPtr->widget(m_parameterEditorStackPtr->count() - 1));
	}

	m_widgetToGuiMap.clear();

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtprm


