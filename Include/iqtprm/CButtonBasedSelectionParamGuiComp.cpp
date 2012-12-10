#include "iqtprm/CButtonBasedSelectionParamGuiComp.h"


// ACF includes
#include "iprm/ISelectionConstraints.h"


namespace iqtprm
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CButtonBasedSelectionParamGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	const iprm::ISelectionParam* paramPtr = GetObjectPtr();
	if (paramPtr != NULL){
		const iprm::ISelectionConstraints* constraintsPtr = paramPtr->GetSelectionConstraints();
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

			bool useCompactDescription = *m_compactDescriptionAttrPtr;

			QFont buttonFont(SelectionFrame->font().family(), *m_fontSizeAttrPtr);
			bool setFont = m_fontSizeAttrPtr.IsValid();

			int iconsCount = m_iconProviderCompPtr.IsValid() ? m_iconProviderCompPtr->GetIconCount() : 0;
			QSize iconSize(*m_iconSizeAttrPtr, *m_iconSizeAttrPtr);

			int count = constraintsPtr->GetOptionsCount();
			for (int i = 0; i < count; i++){
				QCommandLinkButton* buttonPtr = new QCommandLinkButton(
							constraintsPtr->GetOptionName(i),
							useCompactDescription ? "" : constraintsPtr->GetOptionDescription(i),
							SelectionFrame);

				buttonPtr->setCheckable(true);

				if (i < iconsCount){
					buttonPtr->setIcon(m_iconProviderCompPtr->GetIcon(i));
					buttonPtr->setIconSize(iconSize);
				}

				if (setFont){
					buttonPtr->setFont(buttonFont);
				}

				m_buttonsGroup.addButton(buttonPtr, i);

				if (columns > 1){
					((QGridLayout*)layoutPtr)->addWidget(buttonPtr, i / columns, i % columns);
				} 
				else {
					layoutPtr->addWidget(buttonPtr);
				}
			}

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

	BaseClass::OnGuiModelDetached();
}


void CButtonBasedSelectionParamGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	const iprm::ISelectionParam* paramPtr = GetObjectPtr();
	if (paramPtr != NULL){
		int selectedIndex = paramPtr->GetSelectedOptionIndex();

		if (*m_compactDescriptionAttrPtr){
			bool useHorizontalLayout = (*m_layoutTypeAttrPtr) < 1;
			int minWidth = *m_iconSizeAttrPtr + 24;

			const iprm::ISelectionConstraints* constraintsPtr = paramPtr->GetSelectionConstraints();
			if (constraintsPtr != NULL){
				int count = constraintsPtr->GetOptionsCount();
				for (int i = 0; i < count; i++){
					QCommandLinkButton* buttonPtr = static_cast<QCommandLinkButton*>(m_buttonsGroup.button(i));
					if (i == selectedIndex){
						buttonPtr->setChecked(true);
						buttonPtr->setDescription(constraintsPtr->GetOptionDescription(i));

						if (useHorizontalLayout){
							QSizePolicy sizePolicy = buttonPtr->sizePolicy();
							sizePolicy.setHorizontalStretch(0);
							buttonPtr->setSizePolicy(sizePolicy);
							buttonPtr->setMinimumWidth(minWidth);
						}
					}
					else{
						buttonPtr->setChecked(false);
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
		else{
			QAbstractButton* buttonPtr = m_buttonsGroup.button(selectedIndex);
			if (buttonPtr != NULL){
				buttonPtr->setChecked(true);
			}
		}
	}
}


// protected slots

void CButtonBasedSelectionParamGuiComp::OnButtonClicked(int index)
{
	iprm::ISelectionParam* paramPtr = GetObjectPtr();
	if (paramPtr != NULL){
		paramPtr->SetSelectedOptionIndex(index);
	}
}


} // namespace iqtprm


