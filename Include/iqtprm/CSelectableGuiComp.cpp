#include <iqtprm/CSelectableGuiComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>

#include <iprm/IOptionsList.h>


namespace iqtprm
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CSelectableGuiComp::OnGuiModelAttached()
{
	iprm::ISelectionParam* selectionPtr = GetObservedObject();
	if (selectionPtr != NULL && m_slaveGuisCompPtr.IsValid()){
		const iprm::IOptionsList* constraintsPtr = selectionPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			int optionsCont = constraintsPtr->GetOptionsCount();

			int guisCount = qMin(m_slaveGuisCompPtr.GetCount(), optionsCont);
	
			for (int i = 0; i < guisCount; i++){
				QWidget* newPage = new QWidget();
				QVBoxLayout* pageLayoutPtr = new QVBoxLayout(newPage);
				pageLayoutPtr->setMargin(0);

				iqtgui::IGuiObject* guiPtr = m_slaveGuisCompPtr[i];
				Q_ASSERT(guiPtr);

				if (guiPtr->CreateGui(newPage)){
					SelectionStack->insertWidget(i + 1, newPage);

					m_widgetToGuiMap[newPage] = guiPtr;
				}
			}
		}
	}

	imod::IModel* selectionModelPtr = GetObservedModel();
	Q_ASSERT(selectionModelPtr != NULL);

	if (m_selectorObserverCompPtr.IsValid()){
		selectionModelPtr->AttachObserver(m_selectorObserverCompPtr.GetPtr());
	}

	BaseClass::OnGuiModelAttached();
}


void CSelectableGuiComp::OnGuiModelDetached()
{
	int pagesCount = SelectionStack->count();

	for (int pageIndex = 1; pageIndex < pagesCount; pageIndex++){
		QWidget* pagePtr = SelectionStack->widget(pageIndex);

		WidgetGuiMap::iterator foundGuiIter = m_widgetToGuiMap.find(pagePtr);
		Q_ASSERT(foundGuiIter != m_widgetToGuiMap.end());
		if (foundGuiIter != m_widgetToGuiMap.end()){
			foundGuiIter.value()->DestroyGui();
		}
	}

	while (SelectionStack->count() > 1){
		SelectionStack->removeWidget(SelectionStack->widget(SelectionStack->count() - 1));
	}

	m_widgetToGuiMap.clear();

	imod::IModel* selectionModelPtr = GetObservedModel();
	Q_ASSERT(selectionModelPtr != NULL);

	if (m_selectorObserverCompPtr.IsValid() && selectionModelPtr->IsAttached(m_selectorObserverCompPtr.GetPtr())){
		selectionModelPtr->DetachObserver(m_selectorObserverCompPtr.GetPtr());
	}

	BaseClass::OnGuiModelDetached();
}


void CSelectableGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	iprm::ISelectionParam* selectionPtr = GetObservedObject();
	if (selectionPtr != NULL){
		int selectedIndex = selectionPtr->GetSelectedOptionIndex();

		SelectionStack->setCurrentIndex(selectedIndex + 1);
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CSelectableGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_noSelectionLabelAttrPtr.IsValid()){
		NoSelectionLabel->setText(*m_noSelectionLabelAttrPtr);
	}

	if (m_noSelectionIconAttrPtr.IsValid()){
		NoSelectionLabel->setPixmap(QPixmap(*m_noSelectionIconAttrPtr));
	}

	if (m_selectorGuiCompPtr.IsValid()){
		m_selectorGuiCompPtr->CreateGui(SelectorFrame);
	}
	else{
		SelectorFrame->hide();
		LineSeparator->hide();
	}
}


void CSelectableGuiComp::OnGuiDestroyed()
{
	if (m_selectorGuiCompPtr.IsValid()){
		m_selectorGuiCompPtr->DestroyGui();
	}
	
	BaseClass::OnGuiDestroyed();
}


} // namespace iqtprm


