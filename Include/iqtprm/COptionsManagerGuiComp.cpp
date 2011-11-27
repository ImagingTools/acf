#include "iqtprm/COptionsManagerGuiComp.h"


// Qt includes
#include <QLineEdit>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iprm/ISelectionConstraints.h"


namespace iqtprm
{


// reimplemented (imod::IModelEditor)

void COptionsManagerGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	iprm::ISelectionParam* selectionPtr = GetObjectPtr();
	I_ASSERT(selectionPtr != NULL);

	selectionPtr->SetSelectedOptionIndex(Selector->currentIndex());
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void COptionsManagerGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iprm::ISelectionParam* selectionParamsPtr = GetObjectPtr();
	if (selectionParamsPtr != NULL){
		const iprm::ISelectionConstraints* constraintsPtr = selectionParamsPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			const imod::IModel* constraintsModelPtr = dynamic_cast<const imod::IModel*>(constraintsPtr);
			if (constraintsModelPtr != NULL){
				RegisterModel(const_cast<imod::IModel*>(constraintsModelPtr));
			}
		}
	}
}


void COptionsManagerGuiComp::OnGuiModelDetached()
{
	UnregisterAllModels();

	BaseClass::OnGuiModelDetached();
}


void COptionsManagerGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	UpdateComboBoxesView();

	UpdateDescriptionFrame();
}


// reimplemented (iqtgui::CGuiComponentBase)

void COptionsManagerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	DescriptionFrame->hide();

	if (!m_infoLabelAttrPtr.IsValid()){
		InfoFrame->hide();
	}

	if (m_optionsLabelAttrPtr.IsValid()){
		int labelPosition = LP_LEFT;

		if (m_labelPositionAttrPtr.IsValid()){
			labelPosition = *m_labelPositionAttrPtr;
		}

		QLayout* selectorLayoutPtr = NULL;
		I_ASSERT(SelectionFrame->layout() == NULL);

		if (labelPosition == LP_LEFT){
			selectorLayoutPtr = new QHBoxLayout(SelectionFrame);
			
		}
		else{
			selectorLayoutPtr = new QVBoxLayout(SelectionFrame);			
		}

		selectorLayoutPtr->setContentsMargins(0, 0, 0, 0);

		QLabel* selectorLabelPtr = new QLabel(SelectionFrame);
		selectorLabelPtr->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
		selectorLabelPtr->setText(iqt::GetQString(*m_optionsLabelAttrPtr));

		selectorLayoutPtr->addWidget(selectorLabelPtr);
		selectorLayoutPtr->addWidget(SelectorLayout->parentWidget());
	}

	QObject::connect(Selector, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelectionChanged(int)));

	if (!m_optionsManagerCompPtr.IsValid()){
		EditEnableButton->hide();
	}
}


void COptionsManagerGuiComp::OnGuiDestroyed()
{
	BaseClass::OnGuiDestroyed();
}


void COptionsManagerGuiComp::OnGuiShown()
{
	BaseClass::OnGuiShown();

	if (m_infoLabelAttrPtr.IsValid()){
		InfoLabel->setText(iqt::GetQString(*m_infoLabelAttrPtr));

		InfoIcon->setScaledContents(true);
		InfoIcon->setMaximumWidth(32);
		InfoIcon->setMaximumHeight(32);
		InfoIcon->setMinimumWidth(32);
		InfoIcon->setMinimumHeight(32);
		InfoIcon->setPixmap(QPixmap(":/Icons/About"));
	}
}


// reimplemented (imod::CMultiModelDispatcherBase)

void COptionsManagerGuiComp::OnModelChanged(int /*modelId*/, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	I_ASSERT(IsGuiCreated());

	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);
		
		UpdateGui();
	}
}


// protected slots

void COptionsManagerGuiComp::OnSelectionChanged(int /*index*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		UpdateModel();

		UpdateDescriptionFrame();
	}
}


void COptionsManagerGuiComp::on_EditEnableButton_toggled(bool toggled)
{
	if (!toggled && Selector->lineEdit() != NULL){
		disconnect(Selector->lineEdit(), SIGNAL(editingFinished()), this, SLOT(OnEditingFinished()));
	}

	Selector->setEditable(toggled);

	if (toggled){
		connect(Selector->lineEdit(), SIGNAL(editingFinished()), this, SLOT(OnEditingFinished()));
	}
}


void COptionsManagerGuiComp::OnEditingFinished()
{
	istd::CString newOptionName = iqt::GetCString(Selector->lineEdit()->text());
	if (!newOptionName.IsEmpty()){
		bool addNewOption = true;
		iprm::ISelectionParam* selectionParamsPtr = GetObjectPtr();
		if (selectionParamsPtr != NULL){
			const iprm::ISelectionConstraints* constraintsPtr = selectionParamsPtr->GetSelectionConstraints();
			if (constraintsPtr != NULL){
				int optionsCount = constraintsPtr->GetOptionsCount();

				for (int i = 0; i < optionsCount; ++i){
					istd::CString name = constraintsPtr->GetOptionName(i);

					if (newOptionName == name){
						selectionParamsPtr->SetSelectedOptionIndex(i);

						addNewOption = false;

						break;
					}
				}
			}
		}

		if (m_optionsManagerCompPtr.IsValid() && addNewOption){
			m_optionsManagerCompPtr->InsertOption(newOptionName, newOptionName.ToString());
		}
	}

	EditEnableButton->toggle();
}


// private methods

void COptionsManagerGuiComp::UpdateComboBoxesView()
{
	Selector->clear();
	
	iprm::ISelectionParam* selectionParamsPtr = GetObjectPtr();
	if (selectionParamsPtr != NULL){
		const iprm::ISelectionConstraints* constraintsPtr = selectionParamsPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			int optionsCount = constraintsPtr->GetOptionsCount();

			for (int i = 0; i < optionsCount; ++i){
				istd::CString name = constraintsPtr->GetOptionName(i);

				Selector->addItem(iqt::GetQString(name));
			}

			int selectedIndex = selectionParamsPtr->GetSelectedOptionIndex();

			Selector->setCurrentIndex(selectedIndex);
		}
	}

	UpdateDescriptionFrame();
}


void COptionsManagerGuiComp::UpdateDescriptionFrame()
{
	DescriptionFrame->setVisible(false);

	iprm::ISelectionParam* selectionPtr = GetObjectPtr();
	if (selectionPtr != NULL){
		int selectedIndex = selectionPtr->GetSelectedOptionIndex();
		
		const iprm::ISelectionConstraints* constraintsPtr = selectionPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL && selectedIndex >= 0){

			QString optionDescription = iqt::GetQString(constraintsPtr->GetOptionDescription(selectedIndex));
			DescriptionLabel->setText(optionDescription);

			if (!optionDescription.isEmpty()){
				DescriptionFrame->setVisible(true);
			}
		}
	}
}


} // namespace iqtprm


