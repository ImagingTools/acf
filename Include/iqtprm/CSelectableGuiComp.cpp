#include "iqtprm/CSelectableGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iprm/ISelectionConstraints.h"


namespace iqtprm
{


// reimplemented (imod::IModelEditor)

void CSelectableGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());
}


void CSelectableGuiComp::UpdateEditor(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	iprm::ISelectionParam* selectionPtr = GetObjectPtr();
	if (selectionPtr != NULL){
		int selectedIndex = selectionPtr->GetSelectedOptionIndex();

		SelectionStack->setCurrentIndex(selectedIndex + 1);
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CSelectableGuiComp::OnGuiModelAttached()
{
	iprm::ISelectionParam* selectionPtr = GetObjectPtr();
	if (selectionPtr != NULL && m_slaveGuisCompPtr.IsValid()){
		const iprm::ISelectionConstraints* constraintsPtr = selectionPtr->GetConstraints();
		if (constraintsPtr != NULL){
			int optionsCont = constraintsPtr->GetOptionsCount();

			int guisCount = istd::Min(m_slaveGuisCompPtr.GetCount(), optionsCont);

			for (int i = 0; i < guisCount; i++){
				QWidget* newPage = new QWidget();
				QVBoxLayout* pageLayoutPtr = new QVBoxLayout(newPage);
				pageLayoutPtr->setMargin(0);

				iqtgui::IGuiObject* guiPtr = m_slaveGuisCompPtr[i];
				I_ASSERT(guiPtr);

				if (guiPtr->CreateGui(newPage)){
					SelectionStack->insertWidget(i + 1, newPage);

					m_widgetToGuiMap[newPage] = guiPtr;
				}
			}
		}
	}

	BaseClass::OnGuiModelAttached();
}


void CSelectableGuiComp::OnGuiModelDetached()
{
	for (int pageIndex = 1; pageIndex < SelectionStack->count(); pageIndex++){
		QWidget* pagePtr = SelectionStack->widget(pageIndex);

		WidgetGuiMap::iterator foundGuiIter = m_widgetToGuiMap.find(pagePtr);
		I_ASSERT(foundGuiIter != m_widgetToGuiMap.end());
		if (foundGuiIter != m_widgetToGuiMap.end()){
			foundGuiIter->second->DestroyGui();
		}

		SelectionStack->removeWidget(pagePtr);
	}

	m_widgetToGuiMap.clear();

	BaseClass::OnGuiModelDetached();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CSelectableGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_noSelectionLabelAttrPtr.IsValid()){
		NoSelectionLabel->setText(iqt::GetQString(*m_noSelectionLabelAttrPtr));
	}

	if (m_noSelectionIconAttrPtr.IsValid()){
		NoSelectionLabel->setPixmap(QPixmap(iqt::GetQString(*m_noSelectionIconAttrPtr)));
	}	
}


} // namespace iqtprm


