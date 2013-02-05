#include "iqtprm/CButtonBasedSelectionParamGuiComp.h"


// ACF includes
#include "iprm/IOptionsList.h"
#include "iqt/CSignalBlocker.h"


namespace iqtprm
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CButtonBasedSelectionParamGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	const iprm::ISelectionParam* paramPtr = GetObjectPtr();
	if (paramPtr != NULL){
		const iprm::IOptionsList* constraintsPtr = paramPtr->GetSelectionConstraints();
		if (constraintsPtr != NULL){
			int columns = *m_layoutTypeAttrPtr;
			if (columns < 1){
				SelectionFrame->setLayout(new QHBoxLayout());
			} 
			else if (columns == 1){
				SelectionFrame->setLayout(new QVBoxLayout());
			} 
			else{
				SelectionFrame->setLayout(new QGridLayout());
			}

			QLayout* layoutPtr = SelectionFrame->layout();
			layoutPtr->setContentsMargins(0,0,0,0);
			layoutPtr->setSpacing(1);

			int totalButtons = 0;
			CreateButtons(paramPtr, NULL, totalButtons);

			connect(&m_buttonsGroup, SIGNAL(buttonClicked(int)), this, SLOT(OnButtonClicked(int)));
		}
	}
}


void CButtonBasedSelectionParamGuiComp::OnGuiModelDetached()
{
	QList<QAbstractButton*> buttons = m_buttonsGroup.buttons();
	for (int i = 0; i < buttons.count(); i++){
		QAbstractButton* buttonPtr = buttons.at(i);

		SelectionFrame->layout()->removeWidget(buttonPtr);

		m_buttonsGroup.removeButton(buttonPtr);

		delete buttonPtr;
	}

	m_selectionInfos.clear();

	qDeleteAll(m_allSelectionInfos);
	m_allSelectionInfos.clear();

	BaseClass::OnGuiModelDetached();
}


void CButtonBasedSelectionParamGuiComp::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());

	bool isCompactDescription = (*m_descriptionTypeAttrPtr == 2);
	bool useHorizontalLayout = (*m_layoutTypeAttrPtr) < 1;
	int minWidth = *m_iconSizeAttrPtr + 24;

	QList<QAbstractButton*> buttons = m_buttonsGroup.buttons();
	for (int i = 0; i < buttons.count(); i++){
		bool isChecked = true;

		SelectionInfo* selectionInfo = m_selectionInfos.at(i);
		while (selectionInfo != NULL){
			Q_ASSERT(selectionInfo->paramPtr != NULL);

			if (selectionInfo->paramPtr->GetSelectedOptionIndex() != selectionInfo->index){
				isChecked = false;
				break;
			}

			selectionInfo = selectionInfo->parentInfoPtr;
		}

		// restore selectionInfo
		selectionInfo = m_selectionInfos.at(i);

		QCommandLinkButton* buttonPtr = static_cast<QCommandLinkButton*>(buttons.at(i));

		Q_ASSERT(buttonPtr != NULL && selectionInfo != NULL);

		// should this button be checked?
		if (isChecked){
			buttonPtr->setChecked(true);

			if (isCompactDescription){
				const iprm::IOptionsList* constraintsPtr = selectionInfo->paramPtr->GetSelectionConstraints();
				if (constraintsPtr != NULL){
					buttonPtr->setDescription(constraintsPtr->GetOptionDescription(selectionInfo->index));
				}

				if (useHorizontalLayout){
					QSizePolicy sizePolicy = buttonPtr->sizePolicy();
					sizePolicy.setHorizontalStretch(0);
					buttonPtr->setSizePolicy(sizePolicy);
					buttonPtr->setMinimumWidth(minWidth);
				}
			}
			else{
				break;
			}
		}
		else{
			buttonPtr->setChecked(false);

			if (isCompactDescription){
				buttonPtr->setDescription(QString::null);

				if (useHorizontalLayout){
					QSizePolicy sizePolicy = buttonPtr->sizePolicy();
					sizePolicy.setHorizontalStretch(1);
					buttonPtr->setSizePolicy(sizePolicy);
					buttonPtr->setMinimumWidth(
						minWidth + 
						QFontMetrics(buttonPtr->font()).width(buttonPtr->text()));						
				}
			}
		}
	}
}


// protected slots

void CButtonBasedSelectionParamGuiComp::OnButtonClicked(int index)
{
	iprm::ISelectionParam* paramPtr = GetObjectPtr();
	if (paramPtr != NULL){
		UpdateBlocker updateBlocker(this);

		SelectionInfo* selectionInfo = m_selectionInfos.at(index);
		while (selectionInfo != NULL){
			selectionInfo->paramPtr->SetSelectedOptionIndex(selectionInfo->index);
			selectionInfo = selectionInfo->parentInfoPtr;
		}
	}

	UpdateGui(0);
}


// private methods

void CButtonBasedSelectionParamGuiComp::CreateButtons(
			const iprm::ISelectionParam* paramPtr, 
			SelectionInfo* parentSelectionInfoPtr,
			int& totalButtons)
{
	int columns = *m_layoutTypeAttrPtr;

	bool useTooltip = *m_showTooltipAttrPtr;

	QFont buttonFont(SelectionFrame->font().family(), *m_fontSizeAttrPtr);
	bool setFont = m_fontSizeAttrPtr.IsValid();

	int iconsCount = m_iconProviderCompPtr.IsValid() ? m_iconProviderCompPtr->GetIconCount() : 0;
	QSize iconSize(*m_iconSizeAttrPtr, *m_iconSizeAttrPtr);

	int minWidth = qMax(0, *m_minWidthAttrPtr);

	QLayout* layoutPtr = SelectionFrame->layout();

	const iprm::IOptionsList* constraintsPtr = paramPtr->GetSelectionConstraints();
	if (constraintsPtr != NULL){
		int count = constraintsPtr->GetOptionsCount();
		for (int i = 0; i < count; i++){
			SelectionInfo* selectionInfo = new SelectionInfo();
			selectionInfo->paramPtr = const_cast<iprm::ISelectionParam*>(paramPtr);
			selectionInfo->index = i;
			selectionInfo->parentInfoPtr = parentSelectionInfoPtr;
			m_allSelectionInfos.append(selectionInfo);

			iprm::ISelectionParam* subParamPtr = paramPtr->GetSubselection(i);
			if (subParamPtr != NULL){
				CreateButtons(subParamPtr, selectionInfo, totalButtons);

				continue;
			}

			QAbstractButton* buttonPtr = CreateButton(SelectionFrame, constraintsPtr->GetOptionDescription(i));
			buttonPtr->setText(constraintsPtr->GetOptionName(i));
			buttonPtr->setCheckable(true);
			buttonPtr->setEnabled(constraintsPtr->IsOptionEnabled(i));

			if (totalButtons < iconsCount){
				buttonPtr->setIcon(m_iconProviderCompPtr->GetIcon(totalButtons));
				buttonPtr->setIconSize(iconSize);
			}

			buttonPtr->setMinimumWidth(minWidth);

			if (setFont){
				buttonPtr->setFont(buttonFont);
			}

			if (useTooltip){
				buttonPtr->setToolTip(constraintsPtr->GetOptionDescription(i));
			}

			if (columns > 1){
				((QGridLayout*)layoutPtr)->addWidget(buttonPtr, totalButtons / columns, totalButtons % columns);
			} 
			else {
				layoutPtr->addWidget(buttonPtr);
			}

			m_buttonsGroup.addButton(buttonPtr, totalButtons);

			m_selectionInfos.append(selectionInfo);

			totalButtons++;
		}
	}
}


QAbstractButton* CButtonBasedSelectionParamGuiComp::CreateButton(QWidget* parentPtr, const QString& description)
{
	QCommandLinkButton* commandButtonPtr = new QCommandLinkButton(parentPtr);
	switch (*m_descriptionTypeAttrPtr){
		case 1:
			commandButtonPtr->setDescription(description);
			break;

		default:
		{
			QSizePolicy sizePolicy = commandButtonPtr->sizePolicy();
			sizePolicy.setHorizontalStretch(0);
			commandButtonPtr->setSizePolicy(sizePolicy);

			int minWidth = *m_iconSizeAttrPtr + 24;
			commandButtonPtr->setMinimumWidth(minWidth);
		}
	}

	return commandButtonPtr;
}


} // namespace iqtprm


