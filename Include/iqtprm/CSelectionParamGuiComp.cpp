#include <iqtprm/CSelectionParamGuiComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iprm/IOptionsList.h>
#include <iqt/CSignalBlocker.h>


namespace iqtprm
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CSelectionParamGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iprm::ISelectionParam* selectionParamsPtr = GetObservedObject();
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


void CSelectionParamGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

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

	if (!m_infoLabelAttrPtr.IsValid() || (*m_uiModeAttrPtr > UM_COMBOBOX)){
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

	if (m_infoLabelAttrPtr.IsValid()){
		InfoLabel->setText(*m_infoLabelAttrPtr);

		SetupInfoLabelIcon(*InfoIcon);
	}

	UpdateSelectorLabel();

	UpdateDescriptionFrame();
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

void CSelectionParamGuiComp::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	if (!IsUpdateBlocked() && IsModelAttached()){
		UpdateBlocker updateBlocker(this);

		UpdateCompletionModel();
		
		UpdateGui(istd::IChangeable::GetAllChanges());
	}
}


// protected slots

void CSelectionParamGuiComp::OnSelectionChanged(int /*index*/)
{
	Q_ASSERT(IsGuiCreated());

	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		iprm::ISelectionParam* selectionPtr = GetObservedObject();
		if (selectionPtr != NULL){
			int switchesCount = m_comboBoxes.GetCount();

			for (		int switchIndex = 0;
						(selectionPtr != NULL) && switchIndex < switchesCount;
						++switchIndex){
				QComboBox* switchBoxPtr = m_comboBoxes.GetAt(switchIndex);
				Q_ASSERT(switchBoxPtr != NULL);

				int currentIndex = switchBoxPtr->currentIndex();
				int selectonIndex = switchBoxPtr->itemData(currentIndex, Qt::UserRole).toInt();
				if (selectonIndex < 0){
					switchBoxPtr->setCurrentIndex(-1);
				}

				if (!selectionPtr->SetSelectedOptionIndex(selectonIndex) && (switchIndex == 0)){
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
	Q_ASSERT(senderPtr != NULL);

	iprm::ISelectionParam* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);
	if (objectPtr != NULL){
		bool needUpdateGui = false;

		if (!IsUpdateBlocked()){
			UpdateBlocker updateBlocker(this);

			for (int selectionIndex = 0; selectionIndex < m_radioButtons.GetCount(); selectionIndex++){
				if (senderPtr == m_radioButtons.GetAt(selectionIndex)->GetRadioButton()){
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


void CSelectionParamGuiComp::OnResetButtonClicked()
{
	iprm::ISelectionParam* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);
	if (objectPtr != NULL){
		objectPtr->SetSelectedOptionIndex(-1);
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

	int switchIndex = 0;
	for (		iprm::ISelectionParam* selectionPtr = GetObservedObject();
				selectionPtr != NULL;
				selectionPtr = GetActiveSubselection(selectionPtr), ++switchIndex){
		QComboBox* switchBoxPtr = NULL;
		if (switchIndex < m_comboBoxes.GetCount()){
			switchBoxPtr = m_comboBoxes.GetAt(switchIndex);
		}
		else{
			switchBoxPtr = new QComboBox(SelectionFrame);
			switchBoxPtr->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

			if (*m_useCompleterAttrPtr){
				switchBoxPtr->setEditable(true);

				// Create filtered element model (based on completion model provided by caller of this method) used in the combo box:
				QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(switchBoxPtr);
				proxyModel->setSourceModel(&m_completionModel);

				QCompleter* completer = new QCompleter(switchBoxPtr);
				completer->setModel(proxyModel);
				completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

				switchBoxPtr->lineEdit()->setCompleter(completer);

				BaseClass::connect(switchBoxPtr->lineEdit(), &QLineEdit::textEdited, this, &CSelectionParamGuiComp::ApplyFilterToProxyModel);
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

			int selectedIndex = selectionPtr->GetSelectedOptionIndex();

			for (int i = 0, itemIndex = 0; i < optionsCount; ++i){
				if (constraintsPtr->IsOptionEnabled(i) || *m_fillWithDisabledOptionsEnabledAttrPtr){
					QString name = constraintsPtr->GetOptionName(i);

					switchBoxPtr->addItem(name);
					switchBoxPtr->setItemData(itemIndex++, i);
				}
				else if (i == selectedIndex){
					selectedIndex = -1;
				}
			}

			if (*m_noSelectionAllowedAttrPtr){
				switchBoxPtr->insertSeparator(optionsCount);
				switchBoxPtr->addItem(tr("Reset"));
				switchBoxPtr->setItemData(optionsCount+1, -1);
			}

			switchBoxPtr->setCurrentIndex(selectedIndex);

			if (*m_disableWhenEmptyAttrPtr && optionsCount == 0){
				switchBoxPtr->setEnabled(false);
			}
		}
	}

	m_comboBoxes.SetCount(switchIndex);

	UpdateDescriptionFrame();
}


void CSelectionParamGuiComp::UpdateRadioButtonView()
{
	bool useVerticalLayout = (*m_uiModeAttrPtr == UM_RADIOBUTTON_VERTICAL);

	m_radioButtons.Reset();
	m_radioButtonGroups.Reset();
	m_radioButtonFramePtr.Reset();

	for (		iprm::ISelectionParam* selectionPtr = GetObservedObject();
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
			bool isOptionEnabled = (selectedIndex >= 0 && constraintsPtr->IsOptionEnabled(selectedIndex)) || (selectedIndex < 0);

			QButtonGroup* groupPtr = new QButtonGroup;
			m_radioButtonGroups.PushBack(groupPtr);

			for (int i = 0; i < optionsCount; ++i){
				QString name = constraintsPtr->GetOptionName(i);
				QString description = constraintsPtr->GetOptionDescription(i);

				RadioButtonWidget* radioButtonWidget = new RadioButtonWidget(
							GetInfoIcon(),
							name,
							description,
							groupPtr,
							*m_radioButtonFramePtr.GetPtr());

				buttonLayoutPtr->addWidget(radioButtonWidget);

				m_radioButtons.PushBack(radioButtonWidget);

				radioButtonWidget->setEnabled(isOptionEnabled);
				if (selectedIndex == i){
					selectedButtonPtr = radioButtonWidget->GetRadioButton();
				}

				QObject::connect(radioButtonWidget->GetRadioButton(), SIGNAL(toggled(bool)), this, SLOT(OnRadioButtonSelectionChanged(bool)));
			}

			if (*m_noSelectionAllowedAttrPtr){
				m_resetButtonWidget = new QPushButton(tr("Reset"));
				m_resetButtonWidget->setToolTip("Reset current selection");
				buttonLayoutPtr->addWidget(m_resetButtonWidget);

				QObject::connect(m_resetButtonWidget, SIGNAL(clicked()), this, SLOT(OnResetButtonClicked()));
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

	if (*m_uiModeAttrPtr > UM_COMBOBOX){
		return;
	}

	iprm::ISelectionParam* selectionPtr = GetObservedObject();
	if (selectionPtr != NULL){
		int selectedIndex = selectionPtr->GetSelectedOptionIndex();
		
		const iprm::IOptionsList* constraintsPtr = selectionPtr->GetSelectionConstraints();
		if ((constraintsPtr != NULL) && (selectedIndex >= 0) && (selectedIndex < constraintsPtr->GetOptionsCount())){

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
	m_radioButtonGroups.Reset();
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


void CSelectionParamGuiComp::SetupInfoLabelIcon(QLabel& label)
{
	int iconSize = 32;
	if (m_iconSizeAttrPtr.IsValid()){
		iconSize = *m_iconSizeAttrPtr;
	}

	if (m_infoIconProviderCompPtr.IsValid() && m_infoIconProviderCompPtr->GetIconCount() > 0){
		label.setScaledContents(true);
		label.setMaximumWidth(iconSize);
		label.setMaximumHeight(iconSize);
		label.setMinimumWidth(iconSize);
		label.setMinimumHeight(iconSize);
	}

	label.setPixmap(GetInfoIcon());
}


QPixmap CSelectionParamGuiComp::GetInfoIcon() const
{
	QPixmap infoIconPixmap;

	int iconSize = 32;
	if (m_iconSizeAttrPtr.IsValid()){
		iconSize = *m_iconSizeAttrPtr;
	}

	if (m_infoIconProviderCompPtr.IsValid() && m_infoIconProviderCompPtr->GetIconCount() > 0){
		infoIconPixmap = m_infoIconProviderCompPtr->GetIcon(0).pixmap(iconSize, iconSize, QIcon::Normal, QIcon::On);
	}

	return infoIconPixmap;
}


void CSelectionParamGuiComp::UpdateCompletionModel()
{
	m_completionModel.clear();
	m_completionModel.setColumnCount(1);

	iprm::ISelectionParam* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		const iprm::IOptionsList* constraintsPtr = objectPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			int optionsCount = constraintsPtr->GetOptionsCount();
			for (int i = 0; i < optionsCount; ++i){
				QStandardItem* item = new QStandardItem;
				item->setColumnCount(1);
				item->setData(constraintsPtr->GetOptionName(i), Qt::EditRole);
				item->setData(constraintsPtr->GetOptionName(i), Qt::DisplayRole);

				m_completionModel.appendRow(item);
			}
		}
	}
}


CSelectionParamGuiComp::RadioButtonWidget::RadioButtonWidget(
			const QPixmap& infoIcon,
			const QString& optionName, 
			const QString& optionDescription,
			QButtonGroup* buttonGroupPtr,
			QWidget& parentFrame)
	:BaseClass(&parentFrame),
	m_buttonGroupPtr(buttonGroupPtr)
{
	setFrameShape(QFrame::NoFrame);

	QVBoxLayout* mainLayoutPtr = new QVBoxLayout(this);
	mainLayoutPtr->setMargin(0);

	// Radio button
	m_radioButtonPtr = new QRadioButton(this);
	m_radioButtonPtr->setText(optionName);

	// Insert widget into the main layout:
	mainLayoutPtr->addWidget(m_radioButtonPtr);

	if (!optionDescription.isEmpty()){
		// Description [Icon Text]
		QFrame* descriptionFramePtr = new QFrame(this);
		QHBoxLayout* descriptionFrameLayoutPtr = new QHBoxLayout(descriptionFramePtr);
		descriptionFrameLayoutPtr->setMargin(0);

		QLabel* infoIconLabelPtr = new QLabel(descriptionFramePtr);
		infoIconLabelPtr->setPixmap(infoIcon);
		infoIconLabelPtr->setSizePolicy(QSizePolicy::Maximum, infoIconLabelPtr->sizePolicy().verticalPolicy());
		QLabel* descriptionLabelPtr = new QLabel(descriptionFramePtr);
		descriptionLabelPtr->setText(optionDescription);
		descriptionFrameLayoutPtr->addSpacing(5);
		descriptionFrameLayoutPtr->addWidget(infoIconLabelPtr);
		descriptionFrameLayoutPtr->addWidget(descriptionLabelPtr);

		// Insert widget into the main layout:
		mainLayoutPtr->addWidget(descriptionFramePtr);
	}

	// Add radio button to the exclusive button group:
	m_buttonGroupPtr->addButton(m_radioButtonPtr);
}


QRadioButton* CSelectionParamGuiComp::RadioButtonWidget::GetRadioButton() const
{
	return m_radioButtonPtr;
}

void CSelectionParamGuiComp::ApplyFilterToProxyModel(const QString& /*filter*/) const
{
	QLineEdit* signalSenderPtr = dynamic_cast<QLineEdit*>(BaseClass::sender());
	if (signalSenderPtr != NULL){
		QComboBox* comboBoxPtr = dynamic_cast<QComboBox*>(signalSenderPtr->parentWidget());
		if (comboBoxPtr != NULL){
			QSortFilterProxyModel* modelPtr = dynamic_cast<QSortFilterProxyModel*>(comboBoxPtr->lineEdit()->completer()->model());
			if (modelPtr != NULL) {
				QString stringFilter = comboBoxPtr->lineEdit()->text();
				// some recover special symbols in string, may be should add another translate for special symbols
				stringFilter.replace("*", ".*");
				modelPtr->blockSignals(true);
				modelPtr->setFilterRegExp(QRegExp(stringFilter, Qt::CaseInsensitive));
				modelPtr->blockSignals(false);
			}
		}
	}
}

} // namespace iqtprm


