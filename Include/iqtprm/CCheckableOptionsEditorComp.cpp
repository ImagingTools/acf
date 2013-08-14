#include "iqtprm/CCheckableOptionsEditorComp.h"


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QBoxLayout>
#else
#include <QtGui/QCheckBox>
#include <QtGui/QBoxLayout>
#endif

// ACF includes
#include "iprm/IOptionsList.h"


namespace iqtprm
{


// public methods

// reimplemented (imod::IModelEditor)

void CCheckableOptionsEditorComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	iprm::IOptionsManager* managerPtr = GetObjectPtr();
	if (managerPtr == NULL){
		return;
	}

	istd::TChangeNotifier<iprm::IOptionsManager> updatePtr(managerPtr);

	int optionsCount = managerPtr->GetOptionsCount();
	QWidget* widget = GetQtWidget();
	if (widget != NULL){
		for (int optionsIndex = 0; optionsIndex < optionsCount; optionsIndex++){
			QCheckBox* box = widget->findChild<QCheckBox *>(managerPtr->GetOptionName(optionsIndex));
			if (box != NULL){
				managerPtr->SetOptionEnabled(optionsIndex, box->isChecked());
			}
		}
	}
}


// protected slots

void CCheckableOptionsEditorComp::OnParameterChanged()
{
	DoUpdateModel();
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CCheckableOptionsEditorComp::UpdateGui(int updateFlags)
{
	Q_ASSERT(IsGuiCreated());

	iprm::IOptionsManager* managerPtr = GetObjectPtr();
	if (managerPtr == NULL){
		return;
	}

	QWidget* widgetPtr = GetQtWidget();

	if (widgetPtr == NULL){
		return;
	}

	bool added = updateFlags & iprm::IOptionsManager::CF_OPTION_ADDED;
	bool removed = updateFlags & iprm::IOptionsManager::CF_OPTION_REMOVED;
	bool changed = updateFlags & iprm::IOptionsManager::CF_OPTIONS_CHANGED;
	bool renamed = updateFlags & iprm::IOptionsManager::CF_OPTION_RENAMED;
	bool init = updateFlags & imod::IModelEditor::CF_INIT_EDITOR;

	if (added || removed || changed || renamed || init){
		delete widgetPtr->layout();

		int optionCount = managerPtr->GetOptionsCount();

		QBoxLayout* layoutPtr = NULL;

		if (*m_useHorizontalLayoutAttrPtr){
			layoutPtr = new QHBoxLayout(widgetPtr);
		}
		else{
			layoutPtr = new QVBoxLayout(widgetPtr);
		}

		for (int optionIndex = 0; optionIndex < optionCount; optionIndex++){
			QCheckBox* checkBoxPtr = new QCheckBox(widgetPtr);
			checkBoxPtr->setObjectName(managerPtr->GetOptionName(optionIndex));

			checkBoxPtr->setText(managerPtr->GetOptionName(optionIndex));
			bool checkedState = managerPtr->IsOptionEnabled(optionIndex);
			checkBoxPtr->setChecked(checkedState);
			connect(checkBoxPtr, SIGNAL(stateChanged(int)), this, SLOT(OnParameterChanged()));

			layoutPtr->addWidget(checkBoxPtr);
		}
	}
	else{
		int optionsCount = managerPtr->GetOptionsCount();
		if (widgetPtr != NULL){
			for (int optionsIndex = 0; optionsIndex < optionsCount; optionsIndex++){
				QCheckBox* box = widgetPtr->findChild<QCheckBox *>(managerPtr->GetOptionName(optionsIndex));
				if (box != NULL){
					bool checkedState = managerPtr->IsOptionEnabled(optionsIndex);
					box->setChecked(checkedState);
				}
			}
		}
	}
}


} // namespace iqtprm


