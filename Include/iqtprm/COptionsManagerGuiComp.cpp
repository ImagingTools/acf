#include "iqtprm/COptionsManagerGuiComp.h"


// Qt includes
#include <QtGui/QLineEdit>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iprm/ISelectionConstraints.h"


namespace iqtprm
{


// public methods

COptionsManagerGuiComp::COptionsManagerGuiComp()
	:m_isEditingFlag(false)
{
}

// reimplemented (imod::IModelEditor)

void COptionsManagerGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	if (m_isEditingFlag){
		return;
	}

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

	iprm::IOptionsManager* optionManagerPtr = CompCastPtr<iprm::IOptionsManager>(GetObjectPtr());
	if (optionManagerPtr != NULL){
		Selector->setEditable(true);

		connect(Selector->lineEdit(), SIGNAL(editingFinished()), this, SLOT(OnEditingFinished()));
		connect(Selector->lineEdit(), SIGNAL(textChanged(const QString&)), this, SLOT(OnTextChanged(const QString&)));
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

	UpdateComboBox();
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
		selectorLabelPtr->setText(*m_optionsLabelAttrPtr);

		selectorLayoutPtr->addWidget(selectorLabelPtr);
		selectorLayoutPtr->addWidget(SelectorLayout->parentWidget());
	}

	QObject::connect(Selector, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelectionChanged(int)));
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
	if (DoUpdateModel()){
		UpdateDescriptionFrame();
	}

	m_isEditingFlag = false;
}


void COptionsManagerGuiComp::OnEditingFinished()
{
	QString newOptionName = Selector->lineEdit()->text();
	if (!newOptionName.isEmpty()){
		bool addNewOption = true;
		iprm::ISelectionParam* selectionParamsPtr = GetObjectPtr();
		if (selectionParamsPtr != NULL){
			const iprm::ISelectionConstraints* constraintsPtr = selectionParamsPtr->GetSelectionConstraints();
			if (constraintsPtr != NULL){
				int optionsCount = constraintsPtr->GetOptionsCount();

				for (int i = 0; i < optionsCount; ++i){
					QString name = constraintsPtr->GetOptionName(i);

					if (newOptionName == name){
						selectionParamsPtr->SetSelectedOptionIndex(i);

						addNewOption = false;

						break;
					}
				}
			}
		}

		iprm::IOptionsManager* optionManagerPtr = CompCastPtr<iprm::IOptionsManager>(GetObjectPtr());
		if (optionManagerPtr != NULL && addNewOption){
			optionManagerPtr->InsertOption(newOptionName, newOptionName.toLocal8Bit());
		}
	}

	m_isEditingFlag = false;
}


void COptionsManagerGuiComp::OnTextChanged(const QString& /*text*/)
{
	m_isEditingFlag = true;
}


// private methods

void COptionsManagerGuiComp::UpdateComboBox()
{
	Selector->clear();
	
	iprm::ISelectionParam* selectionParamsPtr = GetObjectPtr();
	if (selectionParamsPtr != NULL){
		const iprm::ISelectionConstraints* constraintsPtr = selectionParamsPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			int optionsCount = constraintsPtr->GetOptionsCount();

			for (int i = 0; i < optionsCount; ++i){
				QString name = constraintsPtr->GetOptionName(i);

				Selector->addItem(name);
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

			QString optionDescription = constraintsPtr->GetOptionDescription(selectedIndex);
			DescriptionLabel->setText(optionDescription);

			if (!optionDescription.isEmpty()){
				DescriptionFrame->setVisible(true);
			}
		}
	}
}


} // namespace iqtprm


