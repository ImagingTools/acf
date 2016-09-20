#include <iqtprm/COptionsManagerGuiComp.h>


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QLineEdit>
#else
#include <QtGui/QLineEdit>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iprm/IOptionsList.h>
#include <iprm/INameParam.h>


namespace iqtprm
{


// public methods

COptionsManagerGuiComp::COptionsManagerGuiComp()
	:m_isEditingFlag(false),
	m_isUpdateBlocked(false)
{
}


// reimplemented (imod::IModelEditor)

void COptionsManagerGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	if (m_isEditingFlag){
		return;
	}

	iprm::ISelectionParam* selectionPtr = CompCastPtr<iprm::ISelectionParam>(GetObservedObject());
	if (selectionPtr != NULL){
		selectionPtr->SetSelectedOptionIndex(Selector->currentIndex());
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void COptionsManagerGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iprm::ISelectionParam* selectionPtr = CompCastPtr<iprm::ISelectionParam>(GetObservedObject());
	if (selectionPtr != NULL){
		const iprm::IOptionsList* constraintsPtr = selectionPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			const imod::IModel* constraintsModelPtr = dynamic_cast<const imod::IModel*>(constraintsPtr);
			if (constraintsModelPtr != NULL){
				RegisterModel(const_cast<imod::IModel*>(constraintsModelPtr));
			}
		}
	}

	iprm::IOptionsManager* optionManagerPtr = GetObservedObject();
	if (optionManagerPtr != NULL){
		Selector->setEditable(true);

		QLineEdit* editorPtr = Selector->lineEdit();
		Q_ASSERT(editorPtr != NULL);

		connect(editorPtr, SIGNAL(editingFinished()), this, SLOT(OnEditingFinished()));
		connect(editorPtr, SIGNAL(textEdited(const QString&)), this, SLOT(OnTextEdited(const QString&)));
		connect(editorPtr, SIGNAL(textChanged(const QString&)), this, SLOT(OnTextChanged(const QString&)));
	}
}


void COptionsManagerGuiComp::OnGuiModelDetached()
{
	UnregisterAllModels();

	m_selectorLabelPtr.Reset();

	BaseClass::OnGuiModelDetached();
}


void COptionsManagerGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
{
	Q_ASSERT(IsGuiCreated());

	iprm::IOptionsManager* optionManagerPtr = GetObservedObject();
	if (optionManagerPtr != NULL){
		int supplortedFlags = optionManagerPtr->GetOptionOperationFlags();

		Selector->setEditable(supplortedFlags & iprm::IOptionsManager::OOF_SUPPORT_INSERT);
	}

	if (		changeSet.Contains(iprm::IOptionsManager::CF_OPTION_ADDED) ||
				changeSet.Contains(iprm::IOptionsManager::CF_OPTION_REMOVED) ||
				changeSet.Contains(iprm::IOptionsManager::CF_OPTIONS_CHANGED) ||
				changeSet.Contains(iprm::IOptionsManager::CF_OPTION_RENAMED) ||
				changeSet.Contains(imod::IModelEditor::CF_INIT_EDITOR)){
		UpdateComboBox();
	}
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
		Q_ASSERT(SelectionFrame->layout() == NULL);

		if (labelPosition == LP_LEFT){
			selectorLayoutPtr = new QHBoxLayout(SelectionFrame);

		}
		else{
			selectorLayoutPtr = new QVBoxLayout(SelectionFrame);
		}

		selectorLayoutPtr->setContentsMargins(0, 0, 0, 0);

		m_selectorLabelPtr = new QLabel(SelectionFrame);
		m_selectorLabelPtr->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
		m_selectorLabelPtr->setText(*m_optionsLabelAttrPtr);

		selectorLayoutPtr->addWidget(m_selectorLabelPtr.GetPtr());
		selectorLayoutPtr->addWidget(SelectorLayout->parentWidget());
	}

	QObject::connect(Selector, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelectionChanged(int)));
}


void COptionsManagerGuiComp::OnGuiDestroyed()
{
	m_selectorLabelPtr.Reset();

	BaseClass::OnGuiDestroyed();
}


void COptionsManagerGuiComp::OnGuiShown()
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
}


void COptionsManagerGuiComp::OnGuiRetranslate()
{
	UpdateBlocker updateBlocker(this);

	BaseClass::OnGuiRetranslate();

	if (m_infoLabelAttrPtr.IsValid()){
		InfoLabel->setText(*m_infoLabelAttrPtr);
	}

	if (m_optionsLabelAttrPtr.IsValid()){
		m_selectorLabelPtr->setText(*m_optionsLabelAttrPtr);
	}

	UpdateDescriptionFrame();
}


// reimplemented (imod::CMultiModelDispatcherBase)

void COptionsManagerGuiComp::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& changeSet)
{
	Q_ASSERT(IsGuiCreated());

	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		UpdateGui(changeSet);
	}
}


// protected slots

void COptionsManagerGuiComp::OnSelectionChanged(int /*index*/)
{
	if (m_isUpdateBlocked){
		return;
	}

	if (DoUpdateModel()){
		UpdateDescriptionFrame();
	}

	m_isEditingFlag = false;
}


void COptionsManagerGuiComp::OnEditingFinished()
{
	if (m_isUpdateBlocked){
		return;
	}

	QString newOptionName;
	iprm::INameParam* nameParamsPtr = CompCastPtr<iprm::INameParam>(GetObservedObject());
	if (nameParamsPtr != NULL){
		newOptionName = nameParamsPtr->GetName();
	}
	else
	{
		newOptionName = Selector->lineEdit()->text();
	}

	bool addNewOption = true;
	iprm::ISelectionParam* selectionPtr = CompCastPtr<iprm::ISelectionParam>(GetObservedObject());
	if (selectionPtr != NULL){
		const iprm::IOptionsList* constraintsPtr = selectionPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			int optionsCount = constraintsPtr->GetOptionsCount();

			for (int i = 0; i < optionsCount; ++i){
				QString name = constraintsPtr->GetOptionName(i);

				if (newOptionName == name){
					selectionPtr->SetSelectedOptionIndex(i);

					addNewOption = false;

					break;
				}
			}
		}

		iprm::IOptionsManager* optionManagerPtr = GetObservedObject();
		if ((optionManagerPtr != NULL) && addNewOption){
			optionManagerPtr->InsertOption(newOptionName, newOptionName.toLocal8Bit());
		}
	}

	m_isEditingFlag = false;
}


void COptionsManagerGuiComp::OnTextChanged(const QString& text)
{
	if (m_isUpdateBlocked){
		return;
	}

	iprm::INameParam* nameParamsPtr = CompCastPtr<iprm::INameParam>(GetObservedObject());
	if (nameParamsPtr != NULL){
		UpdateBlocker updateBlocker(this);

		nameParamsPtr->SetName(text);
	}
}


void COptionsManagerGuiComp::OnTextEdited(const QString& /*text*/)
{
	if (m_isUpdateBlocked){
		return;
	}

	m_isEditingFlag = true;
}


// private methods

void COptionsManagerGuiComp::UpdateComboBox()
{
	Selector->clear();

	iprm::ISelectionParam* selectionPtr = CompCastPtr<iprm::ISelectionParam>(GetObservedObject());
	if (selectionPtr != NULL){
		m_isUpdateBlocked = true;

		int selectedIndex = selectionPtr->GetSelectedOptionIndex();
		const iprm::IOptionsList* constraintsPtr = selectionPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			int optionsCount = constraintsPtr->GetOptionsCount();
			for (int i = 0; i < optionsCount; ++i){
				const QString& name = constraintsPtr->GetOptionName(i);

				Selector->addItem(name);
			}
		}
		else{
			selectedIndex = -1;
		}
		Selector->setCurrentIndex(selectedIndex);

		m_isUpdateBlocked = false;

		OnTextChanged((selectedIndex >= 0) ? Selector->itemText(selectedIndex) : QString());
	}

	UpdateDescriptionFrame();	
}


void COptionsManagerGuiComp::UpdateDescriptionFrame()
{
	DescriptionFrame->setVisible(false);

	iprm::ISelectionParam* selectionPtr = CompCastPtr<iprm::ISelectionParam>(GetObservedObject());
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


} // namespace iqtprm


