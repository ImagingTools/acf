#include <iqtprm/CSelectionParamGuiComp.h>

// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QListView>
#else
#include <QtGui/QButtonGroup>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iprm/IOptionsList.h>
#include <iqt/CSignalBlocker.h>
#include <iwidgets/iwidgets.h>


namespace iqtprm
{


CSelectionParamGuiComp::CSelectionParamGuiComp()
:	m_resetButtonWidgetPtr(NULL)
{
}


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
		case UM_COMBOBOX_VERTICAL:
			UpdateComboBoxesView();
			break;

		case UM_RADIOBUTTON_HORIZONTAL:
		case UM_RADIOBUTTON_VERTICAL:
			UpdateRadioButtonView();
			break;
		default:
			Q_ASSERT(false);
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CSelectionParamGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

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
}


void CSelectionParamGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	UpdateBlocker updateBlocker(this);

	if (m_infoLabelAttrPtr.IsValid()){
		InfoLabel->setText(*m_infoLabelAttrPtr);
	}

	/// \todo move to new method
	if (m_optionsLabelListAttrPtr.IsValid()){
		int optionLabelCount = m_optionsLabelList.count();
		int labelsCount = m_optionsLabelList.count();
		for (int optionLabelIndex = 0; optionLabelIndex < qMin(optionLabelCount, labelsCount); ++optionLabelIndex){
			QString optionLabelText = m_optionsLabelListAttrPtr[optionLabelIndex];
			if (!optionLabelText.isEmpty()){
				m_optionsLabelList.at(optionLabelIndex)->setText(optionLabelText);
			}
		}
	}

	if (m_infoLabelListAttrPtr.IsValid()){
		int infoLabelCount = m_infoLabelListAttrPtr.GetCount();
		int labelsCount = m_optionInfoLabelList.count();
		for (int infoLabelIndex = 0; infoLabelIndex < qMin(infoLabelCount, labelsCount); ++infoLabelIndex){
			QString infoText = m_infoLabelListAttrPtr[infoLabelIndex];
			if (!infoText.isEmpty()){
				m_optionInfoLabelList.at(infoLabelIndex)->setText(infoText);
			}
		}
	}

	UpdateSelectorLabel();
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
		}

		UpdateComboBoxesView();
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


void CSelectionParamGuiComp::OnFilterTextEdited(const QString& text)
{
	QLineEdit* signalSenderPtr = dynamic_cast<QLineEdit*>(BaseClass::sender());
	if (signalSenderPtr != NULL){
		QComboBox* comboBoxPtr = dynamic_cast<QComboBox*>(signalSenderPtr->parentWidget());
		if (comboBoxPtr != NULL){
			QSortFilterProxyModel* modelPtr = dynamic_cast<QSortFilterProxyModel*>(comboBoxPtr->lineEdit()->completer()->model());
			if (modelPtr != NULL) {
				QString stringFilter = text;
				// some recover special symbols in string, may be should add another translate for special symbols
				stringFilter.replace("*", ".*");
				modelPtr->blockSignals(true);
				modelPtr->setFilterRegularExpression(QRegularExpression(stringFilter, QRegularExpression::CaseInsensitiveOption));
				modelPtr->blockSignals(false);
			}
		}
	}
}


// private methods

void CSelectionParamGuiComp::UpdateComboBoxesView()
{
	bool useVerticalLayout = (*m_uiModeAttrPtr == UM_COMBOBOX_VERTICAL);

	QLayout* mainLayoutPtr = SelectionFrame->layout();
	if (mainLayoutPtr == NULL){
		if (useVerticalLayout){
			mainLayoutPtr = new QVBoxLayout(SelectionFrame);
		}
		else{
			mainLayoutPtr = new QHBoxLayout(SelectionFrame);
		}

		mainLayoutPtr->setContentsMargins(0,0,0,0);
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
			QLayout* comboLayoutPtr;
			if (m_labelPositionAttrPtr.IsValid() && *m_labelPositionAttrPtr == LP_TOP){
				comboLayoutPtr = new QVBoxLayout(SelectionFrame);
			}
			else {
				comboLayoutPtr = new QHBoxLayout(SelectionFrame);
			}
			comboLayoutPtr->setContentsMargins(0,0,0,0);

			QString optionLabelText;
			if (m_optionsLabelListAttrPtr.IsValid() && switchIndex < m_optionsLabelListAttrPtr.GetCount()){
				optionLabelText = m_optionsLabelListAttrPtr[switchIndex];
			}

			QLabel* groupNameLabelPtr = new QLabel(optionLabelText, SelectionFrame);
			m_optionsLabelList.push_back(groupNameLabelPtr);
			if (!optionLabelText.isEmpty()){
				comboLayoutPtr->addWidget(groupNameLabelPtr);
			}

			QVBoxLayout* comboBoxDescriptionInfoLayoutPtr = new QVBoxLayout(SelectionFrame);
			switchBoxPtr = new QComboBox(SelectionFrame);
			switchBoxPtr->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
			switchBoxPtr->setView(new QListView()); // Qt bug workaround to set up the style from css
			comboBoxDescriptionInfoLayoutPtr->addWidget(switchBoxPtr);

			if (m_infoLabelListAttrPtr.IsValid() && switchIndex < m_infoLabelListAttrPtr.GetCount()){
				QString infoText = m_infoLabelListAttrPtr[switchIndex];
				QLabel* infoLabel = new QLabel(infoText, SelectionFrame);
				if (!infoText.isEmpty()){
					comboBoxDescriptionInfoLayoutPtr->addWidget(infoLabel);
				}
				m_optionInfoLabelList.push_back(infoLabel);
			}

			const iprm::IOptionsList* constraintsPtr = selectionPtr->GetSelectionConstraints();
			if (constraintsPtr != nullptr && selectionPtr->GetSelectedOptionIndex() > -1){
				QString descriptionText = constraintsPtr->GetOptionDescription(selectionPtr->GetSelectedOptionIndex());
				QLabel* descriptionLabel = new QLabel(descriptionText, SelectionFrame);
				if (!descriptionText.isEmpty()){
					comboBoxDescriptionInfoLayoutPtr->addWidget(descriptionLabel);
				}
				m_descriptionLabelList.push_back(descriptionLabel);
			}

			{
				QVBoxLayout* vLayoutPtr = dynamic_cast<QVBoxLayout*>(comboLayoutPtr);
				QHBoxLayout* hLayoutPtr = dynamic_cast<QHBoxLayout*>(comboLayoutPtr);
				if (vLayoutPtr != nullptr){
					vLayoutPtr->addLayout(comboBoxDescriptionInfoLayoutPtr);
				}
				else{
					hLayoutPtr->addLayout(comboBoxDescriptionInfoLayoutPtr);
				}
			}

			if (*m_useCompleterAttrPtr){
				switchBoxPtr->setEditable(true);

				// Create filtered element model (based on completion model provided by caller of this method) used in the combo box:
				QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(switchBoxPtr);
				proxyModel->setSourceModel(&m_completionModel);

				QCompleter* completer = new QCompleter(switchBoxPtr);
				completer->setModel(proxyModel);
				completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

				switchBoxPtr->lineEdit()->setCompleter(completer);

				BaseClass::connect(switchBoxPtr->lineEdit(), SIGNAL(textEdited(const QString&)), this, SLOT(OnFilterTextEdited(const QString&)));
			}

			m_comboBoxes.PushBack(switchBoxPtr);

			{
				QVBoxLayout* vLayoutPtr = dynamic_cast<QVBoxLayout*>(mainLayoutPtr);
				QHBoxLayout* hLayoutPtr = dynamic_cast<QHBoxLayout*>(mainLayoutPtr);
				if (vLayoutPtr != nullptr){
					vLayoutPtr->addLayout(comboLayoutPtr);
				}
				else{
					hLayoutPtr->addLayout(comboLayoutPtr);
				}
			}

			QObject::connect(switchBoxPtr, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelectionChanged(int)));
		}

		iqt::CSignalBlocker blocker(switchBoxPtr);
		Q_UNUSED(blocker);

		switchBoxPtr->clear();

		const iprm::IOptionsList* constraintsPtr = selectionPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			const int optionsCount = constraintsPtr->GetOptionsCount();
			const int selectedIndex = selectionPtr->GetSelectedOptionIndex();
			int selectedItem = -1;
			int itemIndex = 0;
			for (int i = 0; i < optionsCount; ++i){
				if (constraintsPtr->IsOptionEnabled(i) || *m_fillWithDisabledOptionsEnabledAttrPtr){
					const QString& name = constraintsPtr->GetOptionName(i);

					if (i == selectedIndex){
						selectedItem = itemIndex;
					}
					switchBoxPtr->addItem(name);
					switchBoxPtr->setItemData(itemIndex++, i);
				}
			}

			if (*m_noSelectionAllowedAttrPtr && (selectedItem >= 0)){
				switchBoxPtr->insertSeparator(itemIndex++);
				switchBoxPtr->addItem(tr("Reset"));
				switchBoxPtr->setItemData(itemIndex++, -1);
			}

			switchBoxPtr->setCurrentIndex(selectedItem);

			if (*m_disableWhenEmptyAttrPtr && (optionsCount <= 0)){
				switchBoxPtr->setEnabled(false);
			}

			if (constraintsPtr != nullptr){
				if (switchIndex < m_descriptionLabelList.count()) {
					QLabel* descriptionLabelPtr = m_descriptionLabelList.at(switchIndex);
					QString description = constraintsPtr->GetOptionDescription(selectedIndex);
					descriptionLabelPtr->setText(description);
					descriptionLabelPtr->setVisible(!description.isEmpty());
				}
			}
		}
	}

	m_comboBoxes.SetCount(switchIndex);
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

		mainLayoutPtr->setContentsMargins(0,0,0,0);

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

		const iprm::IOptionsList* constraintsPtr = selectionPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			int optionsCount = constraintsPtr->GetOptionsCount();
			int selectedIndex = selectionPtr->GetSelectedOptionIndex();
			bool isOptionEnabled = (selectedIndex >= 0 && constraintsPtr->IsOptionEnabled(selectedIndex)) || (selectedIndex < 0);

			QRadioButton* selectedButtonPtr = NULL;
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
				m_resetButtonWidgetPtr = new QPushButton(tr("Reset"));
				m_resetButtonWidgetPtr->setToolTip("Reset current selection");
				buttonLayoutPtr->addWidget(m_resetButtonWidgetPtr);

				QObject::connect(m_resetButtonWidgetPtr, SIGNAL(clicked()), this, SLOT(OnResetButtonClicked()));
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
					Qt::Alignment qtAlign = Qt::Alignment(0);

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
	mainLayoutPtr->setContentsMargins(0,0,0,0);

	// Radio button
	m_radioButtonPtr = new QRadioButton(this);
	m_radioButtonPtr->setText(optionName);

	// Insert widget into the main layout:
	mainLayoutPtr->addWidget(m_radioButtonPtr);

	if (!optionDescription.isEmpty()){
		// Description [Icon Text]
		QFrame* descriptionFramePtr = new QFrame(this);
		QHBoxLayout* descriptionFrameLayoutPtr = new QHBoxLayout(descriptionFramePtr);
		descriptionFrameLayoutPtr->setContentsMargins(0,0,0,0);

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


} // namespace iqtprm


