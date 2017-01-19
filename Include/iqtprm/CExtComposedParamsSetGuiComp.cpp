#include <iqtprm/CExtComposedParamsSetGuiComp.h>


// ACF includes
#include <iqt/CSignalBlocker.h>
#include <iview/IShapeView.h>
#include <iqt2d/IViewProvider.h>


namespace iqtprm
{


// public methods

// reimplemented (imod::IModelEditor)

void CExtComposedParamsSetGuiComp::UpdateEditor(const istd::IChangeable::ChangeSet& changeSet)
{
	Q_ASSERT(IsGuiCreated());

	if (!changeSet.Contains(istd::IChangeable::CF_DELEGATED)){
		int editorsCount = qMin(GetPagesCount(), m_editorsCompPtr.GetCount());
		for (int i = 0; i < editorsCount; ++i){
			iqtgui::IGuiObject* guiPtr = GetPageGuiComponent(i);
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
}


void CExtComposedParamsSetGuiComp::SetReadOnly(bool state)
{
	int editorsCount = m_editorsCompPtr.GetCount();
	for (int i = 0; i < editorsCount; ++i){
		imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
		if (m_connectedEditorsMap.contains(editorPtr)){
			Q_ASSERT(editorPtr != NULL); // only not NULL editors are stored in m_connectedEditorsMap

			editorPtr->SetReadOnly(state);
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CExtComposedParamsSetGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();
}


void CExtComposedParamsSetGuiComp::OnGuiDestroyed()
{
	BaseClass::OnGuiDestroyed();
}


// reimplemented (iqt2d::IViewExtender)

void CExtComposedParamsSetGuiComp::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	Q_ASSERT(providerPtr != NULL);

	m_connectedSceneFlags[providerPtr] = flags;

	AttachToScene(providerPtr, flags);
}


void CExtComposedParamsSetGuiComp::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	Q_ASSERT(providerPtr != NULL);

	DetachFromScene(providerPtr);

	m_connectedSceneFlags.remove(providerPtr);
}


// protected methods

void CExtComposedParamsSetGuiComp::AttachToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	Q_ASSERT(m_showAllShapesAttrPtr.IsValid());

	int elementsCount = m_extendersCompPtr.GetCount();

	iview::IShapeView* viewPtr = NULL;

	int currentPageIndex = m_pageModel.GetSelectedOptionIndex();

	if ((currentPageIndex >= 0) && !*m_showAllShapesAttrPtr){
		if (currentPageIndex < elementsCount){
			iqt2d::IViewExtender* extenderPtr = m_extendersCompPtr[currentPageIndex];
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


void CExtComposedParamsSetGuiComp::DetachFromScene(iqt2d::IViewProvider* providerPtr)
{
	Q_ASSERT(providerPtr != NULL);

	int elementsCount = m_extendersCompPtr.GetCount();

	int currentPageIndex = m_pageModel.GetSelectedOptionIndex();

	iview::IShapeView* viewPtr = NULL;

	if ((currentPageIndex >= 0) && !*m_showAllShapesAttrPtr){
		if (currentPageIndex < elementsCount){
			iqt2d::IViewExtender* extenderPtr = m_extendersCompPtr[currentPageIndex];
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


// reimplemented (iqtgui::CMultiPageGuiCompBase)

void CExtComposedParamsSetGuiComp::CreatePages()
{
	// Preserve creation of container pages during UI intialization.
}


void CExtComposedParamsSetGuiComp::OnPageChanged(int widgetIndex)
{
	int pageIndex = GetLogicalPageIndex(widgetIndex);
	int currentPageIndex = m_pageModel.GetSelectedOptionIndex();

	if (pageIndex != currentPageIndex){
		for (ConnectedSceneFlags::const_iterator iter = m_connectedSceneFlags.begin();
				iter != m_connectedSceneFlags.end();
				++iter){
			iqt2d::IViewProvider* providerPtr = iter.key();

			DetachFromScene(providerPtr);
		}

		m_pageModel.SetSelectedOptionIndex(pageIndex);

		for (ConnectedSceneFlags::const_iterator iter = m_connectedSceneFlags.begin();
				iter != m_connectedSceneFlags.end();
				++iter){
			iqt2d::IViewProvider* providerPtr = iter.key();
			int flags = iter.value();

			AttachToScene(providerPtr, flags);
		}
	}
}


// reimplemented (imod::IObserver)

void CExtComposedParamsSetGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iqt::CSignalBlocker blocker(GetPageContainerWidget());

	iprm::IParamsSet* paramsSetPtr = GetObservedObject();
	Q_ASSERT(paramsSetPtr != NULL);

	int currentPageIndex = m_pageModel.GetSelectedOptionIndex();

	bool keepGlobalVisible = false;

	int pagesCount = BaseClass::GetPagesCount();
	for (int i = 0; i < pagesCount; ++i){
		bool keepVisible = true;

		QByteArray paramId;
		if (i < m_idsAttrPtr.GetCount()){
			paramId = m_idsAttrPtr[i];
		}

		if (!paramId.isEmpty()){
			imod::IModel* parameterModelPtr = GetObservedModel();
			if (paramId != "*"){
				iser::ISerializable* parameterPtr = paramsSetPtr->GetEditableParameter(paramId);
				parameterModelPtr = dynamic_cast<imod::IModel*>(parameterPtr);

				keepVisible = (parameterPtr != NULL);
			}

			imod::IObserver* observerPtr = NULL;
			if (i < m_observersCompPtr.GetCount()){
				observerPtr = m_observersCompPtr[i];
			}

			if ((parameterModelPtr != NULL) && (observerPtr != NULL) && !parameterModelPtr->IsAttached(observerPtr)){
				if (parameterModelPtr->AttachObserver(observerPtr)){
					imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
					if (editorPtr != NULL){
						m_connectedEditorsMap[editorPtr] = false;
					}
				}
				else{
					qDebug(qPrintable(QString("Parameter with ID '%1' could not be connected to the editor").arg(paramId.constData())));
				}
			}
		}

		// add or remove gui items to the container
		if (keepVisible){
			CreatePage(i);

			EnsurePageInitialized(i);
		}
		else{
			RemovePage(i);
		}

		keepGlobalVisible = keepGlobalVisible || keepVisible;
	}

	// Restore selection:
	SetCurrentPage(currentPageIndex);

	GetWidget()->setVisible(keepGlobalVisible);

	UpdateVisualElements();
}


void CExtComposedParamsSetGuiComp::OnGuiModelDetached()
{
	iprm::IParamsSet* paramsSetPtr = GetObservedObject();
	Q_ASSERT(paramsSetPtr != NULL);

	iqt::CSignalBlocker blocker(GetPageContainerWidget());

	ResetPages();

	m_connectedEditorsMap.clear();

	int elementsCount = qMin(m_observersCompPtr.GetCount(), m_idsAttrPtr.GetCount());
	for (int i = 0; i < elementsCount; ++i){
		const QByteArray& paramId = m_idsAttrPtr[i];

		if (!paramId.isEmpty()){
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
	}

	BaseClass::OnGuiModelDetached();
}


void CExtComposedParamsSetGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated() && (GetObservedObject() != NULL));

	int editorsCount = qMin(GetPagesCount(), m_editorsCompPtr.GetCount());
	for (int i = 0; i < editorsCount; ++i){
		iqtgui::IGuiObject* guiPtr = GetPageGuiComponent(i);
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


} // namespace iqtprm


