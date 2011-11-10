#include "iqtprm/CSelectionParamGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iprm/ISelectionConstraints.h"

#include "iqt/CSignalBlocker.h"


namespace iqtprm
{


// reimplemented (imod::IModelEditor)

void CSelectionParamGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	iprm::ISelectionParam* selectionPtr = GetObjectPtr();
	I_ASSERT(selectionPtr != NULL);

	int switchesCount = m_comboBoxes.GetCount();

	for (		int switchIndex = 0;
				(selectionPtr != NULL) && switchIndex < switchesCount;
				++switchIndex){
		const QComboBox* switchBoxPtr = m_comboBoxes.GetAt(switchIndex);
		selectionPtr->SetSelectedOptionIndex(switchBoxPtr->currentIndex());

		selectionPtr = selectionPtr->GetActiveSubselection();
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CSelectionParamGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	int uiMode = 0;
	if (m_uiModeAttrPtr.IsValid()){
		uiMode = *m_uiModeAttrPtr;
	}

	switch (uiMode){
		case UM_COMBOBOX:
			UpdateComboBoxesView();
			break;

		case UM_RADIOBUTTON_HORIZONTAL:
			UpdateRadioButtonView(false);
			break;

		case UM_RADIOBUTTON_VERTICAL:
			UpdateRadioButtonView(true);
			break;
	}

	UpdateDescriptionFrame();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CSelectionParamGuiComp::OnGuiCreated()
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

		QLabel* selectorLabelPtr = new QLabel(SelectionFrame);
		selectorLabelPtr->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
		selectorLabelPtr->setText(iqt::GetQString(*m_optionsLabelAttrPtr));

		QLayout* selectorLayoutPtr = NULL;
		I_ASSERT(SelectionFrame->layout() == NULL);

		if (labelPosition == LP_LEFT){
			selectorLayoutPtr = new QHBoxLayout(SelectionFrame);
			
		}
		else{
			selectorLayoutPtr = new QVBoxLayout(SelectionFrame);			
		}

		selectorLayoutPtr->setMargin(0);
		selectorLayoutPtr->addWidget(selectorLabelPtr);
	}

}


void CSelectionParamGuiComp::OnGuiDestroyed()
{
	m_comboBoxes.Reset();
	m_radioButtons.Reset();
	m_radioButtonFramePtr.Reset();

	BaseClass::OnGuiDestroyed();
}


void CSelectionParamGuiComp::OnGuiShown()
{
	BaseClass::OnGuiShown();

	if (m_optionsLabelAttrPtr.IsValid() && m_selectorLabelPtr.IsValid()){
		m_selectorLabelPtr->setText(iqt::GetQString(*m_optionsLabelAttrPtr));
	}

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


// protected slots

void CSelectionParamGuiComp::OnSelectionChanged(int /*index*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		UpdateModel();
	}
}


void CSelectionParamGuiComp::OnRadioButtonSelectionChanged(bool isSelected)
{
	if (!isSelected){
		return;
	}

	QRadioButton* senderPtr = dynamic_cast<QRadioButton*>(sender());
	I_ASSERT(senderPtr != NULL);

	iprm::ISelectionParam* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);
	if (objectPtr != NULL){
		if (!IsUpdateBlocked()){
			UpdateBlocker updateBlocker(this);

			for (int selectionIndex = 0; selectionIndex < m_radioButtons.GetCount(); selectionIndex++){
				if (senderPtr == m_radioButtons.GetAt(selectionIndex)){
					objectPtr->SetSelectedOptionIndex(selectionIndex);

					break;
				}
			}
		}
	}
}


// private methods

void CSelectionParamGuiComp::UpdateComboBoxesView()
{
	m_comboBoxes.Reset();

	for (		iprm::ISelectionParam* selectionPtr = GetObjectPtr();
				selectionPtr != NULL;
				selectionPtr = selectionPtr->GetActiveSubselection()){
		QComboBox* switchBoxPtr = new QComboBox(SelectionFrame);
		m_comboBoxes.PushBack(switchBoxPtr);
		QLayout* layoutPtr = SelectionFrame->layout();
		if (layoutPtr != NULL){
			layoutPtr->addWidget(switchBoxPtr);
		}

		QObject::connect(switchBoxPtr, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelectionChanged(int)));

		const iprm::ISelectionConstraints* constraintsPtr = selectionPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			int optionsCount = constraintsPtr->GetOptionsCount();

			for (int i = 0; i < optionsCount; ++i){
				istd::CString name = constraintsPtr->GetOptionName(i);

				switchBoxPtr->addItem(iqt::GetQString(name));
			}

			int selectedIndex = selectionPtr->GetSelectedOptionIndex();
			switchBoxPtr->setCurrentIndex(selectedIndex);
		}
	}
}


void CSelectionParamGuiComp::UpdateRadioButtonView(bool useVerticalLayout)
{
	m_radioButtons.Reset();
	m_radioButtonFramePtr.Reset();

	for (		iprm::ISelectionParam* selectionPtr = GetObjectPtr();
				selectionPtr != NULL;
				selectionPtr = selectionPtr->GetActiveSubselection()){
		QLayout* mainLayoutPtr = SelectionFrame->layout();
		if (mainLayoutPtr == NULL){
			mainLayoutPtr = new QVBoxLayout(SelectionFrame);
		}

		mainLayoutPtr->setMargin(0);

		m_radioButtonFramePtr = new QFrame(SelectionFrame);
		m_radioButtonFramePtr->setFrameShape(QFrame::NoFrame);
		mainLayoutPtr->addWidget(m_radioButtonFramePtr.GetPtr());

		QLayout* buttonLayoutPtr = NULL;
		if (useVerticalLayout){
			buttonLayoutPtr = new QVBoxLayout(m_radioButtonFramePtr.GetPtr());
		}
		else{
			buttonLayoutPtr = new QHBoxLayout(m_radioButtonFramePtr.GetPtr());
		}

		buttonLayoutPtr->setContentsMargins(0, 0, 0, 0);

		QRadioButton* selectedButtonPtr = NULL;
		const iprm::ISelectionConstraints* constraintsPtr = selectionPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			int optionsCount = constraintsPtr->GetOptionsCount();
			int selectedIndex = selectionPtr->GetSelectedOptionIndex();

			for (int i = 0; i < optionsCount; ++i){
				istd::CString name = constraintsPtr->GetOptionName(i);

				QRadioButton* radioButtonPtr = new QRadioButton(SelectionFrame);
				if (selectedIndex == i){
					selectedButtonPtr = radioButtonPtr;
				}

				radioButtonPtr->setText(iqt::GetQString(name));
				m_radioButtons.PushBack(radioButtonPtr);

				buttonLayoutPtr->addWidget(radioButtonPtr);

				QObject::connect(radioButtonPtr, SIGNAL(toggled(bool)), this, SLOT(OnRadioButtonSelectionChanged(bool)));
			}

			QSizePolicy::Policy vPolicy = useVerticalLayout ? QSizePolicy::Expanding : QSizePolicy::Preferred;
			QSizePolicy::Policy hPolicy = (useVerticalLayout == false) ? QSizePolicy::Expanding : QSizePolicy::Preferred;
			buttonLayoutPtr->addItem(new QSpacerItem(5, 5, hPolicy, vPolicy));

			if (selectedButtonPtr != NULL){
				selectedButtonPtr->setChecked(true);
			}
	
		}
	}
}


void CSelectionParamGuiComp::UpdateDescriptionFrame()
{
	DescriptionFrame->setVisible(false);

	iprm::ISelectionParam* selectionPtr = GetObjectPtr();
	if (selectionPtr != NULL){
		const iprm::ISelectionConstraints* constraintsPtr = selectionPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			int selectedIndex = selectionPtr->GetSelectedOptionIndex();

			QString optionDescription = iqt::GetQString(constraintsPtr->GetOptionDescription(selectedIndex));
			DescriptionLabel->setText(optionDescription);

			if (!optionDescription.isEmpty()){
				DescriptionFrame->setVisible(true);
			}
		}
	}
}


} // namespace iqtprm


