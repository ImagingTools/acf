#include "iqtprm/CSelectionParamGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iprm/IOptionsList.h"

#include "iqt/CSignalBlocker.h"


namespace iqtprm
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CSelectionParamGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iprm::ISelectionParam* selectionParamsPtr = GetObjectPtr();
	if (selectionParamsPtr != NULL){
		const iprm::IOptionsList* constraintsPtr = selectionParamsPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			const imod::IModel* constraintsModelPtr = dynamic_cast<const imod::IModel*>(constraintsPtr);
			if (constraintsModelPtr != NULL){
				RegisterModel(const_cast<imod::IModel*>(constraintsModelPtr));
			}
		}
	}
}


void CSelectionParamGuiComp::OnGuiModelDetached()
{
	UnregisterAllModels();

	ResetWidgets();

	BaseClass::OnGuiModelDetached();
}


void CSelectionParamGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	UpdateSelectorLabel();

	int uiMode = *m_uiModeAttrPtr;

	switch (uiMode){
		case UM_COMBOBOX:
			UpdateComboBoxesView();
			break;

		case UM_RADIOBUTTON_HORIZONTAL:
		case UM_RADIOBUTTON_VERTICAL:
			UpdateRadioButtonView();
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
}


void CSelectionParamGuiComp::OnGuiDestroyed()
{
	ResetWidgets();

	BaseClass::OnGuiDestroyed();
}


void CSelectionParamGuiComp::OnGuiShown()
{
	BaseClass::OnGuiShown();

	int iconSize = 32;
	if (m_iconSizeAttrPtr.IsValid()){
		iconSize = *m_iconSizeAttrPtr;
	}

	if (m_infoLabelAttrPtr.IsValid()){
		InfoLabel->setText(*m_infoLabelAttrPtr);

		InfoIcon->setScaledContents(true);
		InfoIcon->setMaximumWidth(iconSize);
		InfoIcon->setMaximumHeight(iconSize);
		InfoIcon->setMinimumWidth(iconSize);
		InfoIcon->setMinimumHeight(iconSize);
		if (m_infoIconProviderCompPtr.IsValid() && m_infoIconProviderCompPtr->GetIconCount() > 0){
			InfoIcon->setPixmap(m_infoIconProviderCompPtr->GetIcon(0).pixmap(iconSize, iconSize, QIcon::Normal, QIcon::On));
		}
		else{
			InfoIcon->setPixmap(QPixmap(":/Icons/About"));
		}
	}

	UpdateSelectorLabel();
}


void CSelectionParamGuiComp::OnGuiRetranslate()
{
	UpdateBlocker updateBlocker(this);

	BaseClass::OnGuiRetranslate();

	if (m_infoLabelAttrPtr.IsValid()){
		InfoLabel->setText(*m_infoLabelAttrPtr);
	}

	UpdateSelectorLabel();

	UpdateDescriptionFrame();
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CSelectionParamGuiComp::OnModelChanged(int /*modelId*/, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	I_ASSERT(IsGuiCreated());

	if (!IsUpdateBlocked() && IsModelAttached()){
		UpdateBlocker updateBlocker(this);
		
		UpdateGui();
	}
}


// protected slots

void CSelectionParamGuiComp::OnSelectionChanged(int /*index*/)
{
	I_ASSERT(IsGuiCreated());

	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		iprm::ISelectionParam* selectionPtr = GetObjectPtr();
		if (selectionPtr != NULL){
			int switchesCount = m_comboBoxes.GetCount();

			for (		int switchIndex = 0;
						(selectionPtr != NULL) && switchIndex < switchesCount;
						++switchIndex){
				const QComboBox* switchBoxPtr = m_comboBoxes.GetAt(switchIndex);
				if (!selectionPtr->SetSelectedOptionIndex(switchBoxPtr->currentIndex()) && (switchIndex == 0)){
					UpdateComboBoxesView();

					return;
				}

				selectionPtr = GetActiveSubselection(selectionPtr);
			}

			UpdateDescriptionFrame();
		}
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
		bool needUpdateGui = false;

		if (!IsUpdateBlocked()){
			UpdateBlocker updateBlocker(this);

			for (int selectionIndex = 0; selectionIndex < m_radioButtons.GetCount(); selectionIndex++){
				if (senderPtr == m_radioButtons.GetAt(selectionIndex)){
					needUpdateGui = !objectPtr->SetSelectedOptionIndex(selectionIndex);

					break;
				}
			}
		}

		if (needUpdateGui){
			UpdateRadioButtonView();
		}
		else{
			UpdateDescriptionFrame();
		}
	}
}


// private methods

void CSelectionParamGuiComp::UpdateComboBoxesView()
{
	QLayout* mainLayoutPtr = SelectionFrame->layout();
	if (mainLayoutPtr == NULL){
		mainLayoutPtr = new QHBoxLayout(SelectionFrame);

		mainLayoutPtr->setMargin(0);
	}

	QFont f;
	bool setFontSize = false;
	if (m_fontSizeAttrPtr.IsValid()){
		f.setPointSize(*m_fontSizeAttrPtr);
		setFontSize = true;
	}

	int switchIndex = 0;
	for (		iprm::ISelectionParam* selectionPtr = GetObjectPtr();
				selectionPtr != NULL;
				selectionPtr = GetActiveSubselection(selectionPtr), ++switchIndex){
		QComboBox* switchBoxPtr = NULL;
		if (switchIndex < m_comboBoxes.GetCount()){
			switchBoxPtr = m_comboBoxes.GetAt(switchIndex);
		}
		else{
			switchBoxPtr = new QComboBox(SelectionFrame);
			switchBoxPtr->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

			if (setFontSize){
				switchBoxPtr->setFont(f);
			}

			m_comboBoxes.PushBack(switchBoxPtr);

			mainLayoutPtr->addWidget(switchBoxPtr);

			QObject::connect(switchBoxPtr, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelectionChanged(int)));
		}

		iqt::CSignalBlocker blocker(switchBoxPtr);

		while (switchBoxPtr->count() > 0){
			switchBoxPtr->removeItem(0);
		}

		const iprm::IOptionsList* constraintsPtr = selectionPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			int optionsCount = constraintsPtr->GetOptionsCount();

			for (int i = 0; i < optionsCount; ++i){
				QString name = constraintsPtr->GetOptionName(i);

				switchBoxPtr->addItem(name);
			}

			int selectedIndex = selectionPtr->GetSelectedOptionIndex();
			switchBoxPtr->setCurrentIndex(selectedIndex);
		}
	}

	m_comboBoxes.SetCount(switchIndex);

	UpdateDescriptionFrame();
}


void CSelectionParamGuiComp::UpdateRadioButtonView()
{
	bool useVerticalLayout = (*m_uiModeAttrPtr == UM_RADIOBUTTON_VERTICAL);

	m_radioButtons.Reset();
	m_radioButtonFramePtr.Reset();

	for (		iprm::ISelectionParam* selectionPtr = GetObjectPtr();
				selectionPtr != NULL;
				selectionPtr = GetActiveSubselection(selectionPtr)){
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
		const iprm::IOptionsList* constraintsPtr = selectionPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			int optionsCount = constraintsPtr->GetOptionsCount();
			int selectedIndex = selectionPtr->GetSelectedOptionIndex();
			bool isOptionEnabled = constraintsPtr->IsOptionEnabled(selectedIndex);

			for (int i = 0; i < optionsCount; ++i){
				QString name = constraintsPtr->GetOptionName(i);

				QRadioButton* radioButtonPtr = new QRadioButton(SelectionFrame);
				if (selectedIndex == i){
					selectedButtonPtr = radioButtonPtr;
				}

				radioButtonPtr->setText(name);
				radioButtonPtr->setEnabled(isOptionEnabled);
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

	UpdateDescriptionFrame();
}


void CSelectionParamGuiComp::UpdateDescriptionFrame()
{
	DescriptionFrame->setVisible(false);

	iprm::ISelectionParam* selectionPtr = GetObjectPtr();
	if (selectionPtr != NULL){
		int selectedIndex = selectionPtr->GetSelectedOptionIndex();
		
		const iprm::IOptionsList* constraintsPtr = selectionPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL && selectedIndex >= 0){

			QString optionDescription = constraintsPtr->GetOptionDescription(selectedIndex);
			DescriptionLabel->setText(optionDescription);

			if (!optionDescription.isEmpty()){
				DescriptionFrame->setVisible(true);
			}
		}
	}
}


void CSelectionParamGuiComp::UpdateSelectorLabel()
{
	if (m_optionsLabelAttrPtr.IsValid()){
		if (m_selectorLabelPtr == NULL){
			int labelPosition = LP_LEFT;

			if (m_labelPositionAttrPtr.IsValid()){
				labelPosition = *m_labelPositionAttrPtr;
			}

			QLayout* selectorLayoutPtr = SelectionFrame->layout();
			if (selectorLayoutPtr == NULL){
				if (labelPosition == LP_LEFT){
					selectorLayoutPtr = new QHBoxLayout(SelectionFrame);
				}
				else{
					selectorLayoutPtr = new QVBoxLayout(SelectionFrame);			
				}
			}

			selectorLayoutPtr->setContentsMargins(0, 0, 0, 0);

			m_selectorLabelPtr = new QLabel(SelectionFrame);
			m_selectorLabelPtr->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
			m_selectorLabelPtr->setText(*m_optionsLabelAttrPtr);

			if (m_labelWidthAttrPtr.IsValid()){
				int width = *m_labelWidthAttrPtr;
				if (width > 0){
					m_selectorLabelPtr->setFixedWidth(width);
				}
			}

			if (m_labelAlignAttrPtr.IsValid()){
				int align = *m_labelAlignAttrPtr;
				if (align >= LA_LEFT_TOP && align <= LA_RIGHT_BOTTOM){
					Qt::Alignment qtAlign = 0;

					switch (align){
					case LA_LEFT_TOP:	
						qtAlign = Qt::AlignLeft | Qt::AlignTop;
						break;

					case LA_CENTER_TOP:	
						qtAlign = Qt::AlignHCenter | Qt::AlignTop;
						break;

					case LA_RIGHT_TOP:	
						qtAlign = Qt::AlignRight | Qt::AlignTop;
						break;

					case LA_LEFT_CENTER:	
						qtAlign = Qt::AlignLeft | Qt::AlignVCenter;
						break;

					case LA_CENTER:	
						qtAlign = Qt::AlignCenter;
						break;

					case LA_RIGHT_CENTER:	
						qtAlign = Qt::AlignRight | Qt::AlignVCenter;
						break;

					case LA_LEFT_BOTTOM:	
						qtAlign = Qt::AlignLeft | Qt::AlignBottom;
						break;

					case LA_CENTER_BOTTOM:	
						qtAlign = Qt::AlignHCenter | Qt::AlignBottom;
						break;

					case LA_RIGHT_BOTTOM:	
						qtAlign = Qt::AlignRight | Qt::AlignBottom;
						break;
					}

					m_selectorLabelPtr->setAlignment(qtAlign);
				}
			}

			selectorLayoutPtr->addWidget(m_selectorLabelPtr.GetPtr());
		}
		else{
			m_selectorLabelPtr->setText(*m_optionsLabelAttrPtr);
		}

		m_selectorLabelPtr->setVisible(!m_selectorLabelPtr->text().isEmpty());
	}
}


void CSelectionParamGuiComp::ResetWidgets()
{
	m_comboBoxes.Reset();
	m_radioButtons.Reset();
	m_radioButtonFramePtr.Reset();
	m_selectorLabelPtr.Reset();
}


iprm::ISelectionParam* CSelectionParamGuiComp::GetActiveSubselection(const iprm::ISelectionParam* selectionPtr) const
{
	if (selectionPtr == NULL){
		return NULL;
	}

	return selectionPtr->GetSubselection(selectionPtr->GetSelectedOptionIndex());
}


} // namespace iqtprm


