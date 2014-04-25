#include "iqtprm/CExtParamsManagerGuiComp.h"


// Qt includes
#include <QtGui/QMessageBox>
#include <QtGui/QStylePainter>

// ACF includes
#include <istd/TChangeNotifier.h>
#include <iprm/IOptionsList.h>
#include <iprm/TParamsPtr.h>
#include <iprm/CParamsSet.h>
#include <iqtgui/CGuiComponentDialog.h>


namespace iqtprm
{


// protected methods

int CExtParamsManagerGuiComp::GetParameterTypeIndexById(const QByteArray& parameterTypeId) const
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const iprm::IOptionsList* parameterTypeConstraintsPtr = objectPtr->GetParamsTypeConstraints();
		if (parameterTypeConstraintsPtr != NULL){
			for (int typeIndex = 0; typeIndex < parameterTypeConstraintsPtr->GetOptionsCount(); ++ typeIndex){
				if (parameterTypeConstraintsPtr->GetOptionId(typeIndex) == parameterTypeId){
					return typeIndex;
				}
			}
		}
	}

	return -1;
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CExtParamsManagerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	ElementList->setItemDelegate(new CElementItemDelegate());
	ElementList->setIconSize(QSize(32,32));

	CloneButton->setEnabled(false);
	EditButton->setEnabled(false);
	RemoveButton->setEnabled(false);
	ConfigureButton->setEnabled(false);

	// Build mapping between parameter editors and worker type IDs:
	int editorsCount = qMin(_parameterEditorIdsAttrPtr.GetCount(), _parameterGuisCompPtr.GetCount());
	for (int i = 0; i < editorsCount; ++i){
		ParameterEditor parameterEditor;
		parameterEditor.guiPtr = _parameterGuisCompPtr[i];

		if (i < _parameterObserversCompPtr.GetCount()){
			parameterEditor.observerPtr = _parameterObserversCompPtr[i];
		}

		if (i < _parameterEditorsCompPtr.GetCount()){
			parameterEditor.editorPtr = _parameterEditorsCompPtr[i];
		}

		if ( (parameterEditor.guiPtr != NULL) && (parameterEditor.observerPtr != NULL) && (parameterEditor.editorPtr != NULL)){
			_parameterEditorMap[_parameterEditorIdsAttrPtr[i]] = parameterEditor;
		}
	}
}


void CExtParamsManagerGuiComp::UpdateGui(int /*updateFlags = 0*/)
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		ElementList->clear();
		ElementTypeSelector->clear();

		const iprm::IOptionsList* parameterTypeConstraintsPtr = objectPtr->GetParamsTypeConstraints();
		if (parameterTypeConstraintsPtr != NULL){
			for (int typeIndex = 0; typeIndex < parameterTypeConstraintsPtr->GetOptionsCount(); ++ typeIndex){
				ElementTypeSelector->addItem(parameterTypeConstraintsPtr->GetOptionName(typeIndex), parameterTypeConstraintsPtr->GetOptionId(typeIndex));
			}
		}
		else{
			ElementTypeSelector->hide();
		}

		const iprm::IOptionsList* elementListPtr = objectPtr->GetSelectionConstraints();
		if (elementListPtr == NULL){
			return;
		}

		int itemsCount = elementListPtr->GetOptionsCount();

		for (int i = 0; i < itemsCount; i++){
			const QString name = elementListPtr->GetOptionName(i);
			QString description = elementListPtr->GetOptionDescription(i);

			QListWidgetItem* itemPtr = new QListWidgetItem(name, ElementList);
			itemPtr->setData(DR_DESCRIPTION_ROLE, description);

			const iprm::IParamsSet* parameterSetPtr = objectPtr->GetParamsSet(i);
			if (parameterSetPtr != NULL){
				itemPtr->setData(DR_PARAMETER_TYPE_ROLE, parameterSetPtr->GetFactoryId());
			}

			itemPtr->setData(DR_INDEX_ROLE, QVariant::fromValue(i));
		}
	}
	else{
		AddButton->setEnabled(false);
		CloneButton->setEnabled(false);
		EditButton->setEnabled(false);
		RemoveButton->setEnabled(false);
		ConfigureButton->setEnabled(false);
	}

	ElementTypeSelector->setCurrentIndex(0);
}


// protected slots

void CExtParamsManagerGuiComp::on_AddButton_clicked()
{
	if (ElementTypeSelector->currentIndex() < 0){
		return;
	}

	QByteArray parameterTypeId = ElementTypeSelector->itemData(ElementTypeSelector->currentIndex(), Qt::UserRole).toByteArray();

	int parameterTypeIndex = GetParameterTypeIndexById(parameterTypeId);

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int newIndex = objectPtr->InsertParamsSet(parameterTypeIndex);

		ElementList->setCurrentRow(newIndex);

		// change defaults as well
		on_EditButton_clicked();
	}
}


void CExtParamsManagerGuiComp::on_ConfigureButton_clicked()
{
	iprm::IParamsManager* paramsManagerPtr = GetObjectPtr();
	if (paramsManagerPtr == NULL){
		return;
	}

	QList<QListWidgetItem*> selectedItems = ElementList->selectedItems();
	if (!selectedItems.isEmpty()){
		int elementIndex = selectedItems.at(0)->data(DR_INDEX_ROLE).toInt();
		QByteArray parameterTypeId = selectedItems.at(0)->data(DR_PARAMETER_TYPE_ROLE).toByteArray();

		iprm::IParamsSet* parameterSetPtr = const_cast<iprm::IParamsSet*>(paramsManagerPtr->GetParamsSet(elementIndex));
		imod::IModel* parameterSetModelPtr = dynamic_cast<imod::IModel*>(parameterSetPtr);
		if (parameterSetModelPtr != NULL){
			// Exists an editor for the worker type:
			if (_parameterEditorMap.contains(parameterTypeId)){
				ParameterEditor& editor = _parameterEditorMap[parameterTypeId];

				// ...then connect the parameter model to the editor and shows it:
				if (!parameterSetModelPtr->IsAttached(editor.observerPtr)){
					if (parameterSetModelPtr->AttachObserver(editor.observerPtr)){
						iqtgui::CGuiComponentDialog dialog(_parameterEditorMap[parameterTypeId].guiPtr, QDialogButtonBox::Close, true, GetWidget());
						dialog.setWindowTitle(tr("Configuration"));
						dialog.resize(480, 320);

						dialog.exec();

						parameterSetModelPtr->DetachObserver(editor.observerPtr);
					}
				}
			}
		}
	}
}


void CExtParamsManagerGuiComp::on_CloneButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int index = ElementList->currentIndex().row();
		if (index >= 0){
			const iprm::IParamsSet* sourceParamsSetPtr = objectPtr->GetParamsSet(index);
			if (sourceParamsSetPtr != NULL){

				istd::CChangeNotifier changePtr(objectPtr);

				int insertedIndex = objectPtr->InsertParamsSet(GetParameterTypeIndexById(sourceParamsSetPtr->GetFactoryId()), index + 1);
				if (insertedIndex >= 0){
					iprm::IParamsSet* targetParamsSetPtr = objectPtr->GetParamsSet(insertedIndex);
					Q_ASSERT(targetParamsSetPtr != NULL);

					targetParamsSetPtr->CopyFrom(*sourceParamsSetPtr);

					const iprm::IOptionsList* elementListPtr = objectPtr->GetSelectionConstraints();
					Q_ASSERT(elementListPtr != NULL);

					// Restore element index here:
					ElementList->setCurrentRow(elementListPtr->GetOptionsCount() - 1);
				}
			}
		}
	}
}


void CExtParamsManagerGuiComp::on_EditButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int index = ElementList->currentIndex().row();
		if (index >= 0){
			const iprm::IOptionsList* elementListPtr = objectPtr->GetSelectionConstraints();
			Q_ASSERT(elementListPtr != NULL);

			QString name = elementListPtr->GetOptionName(index);
			QString description = elementListPtr->GetOptionDescription(index);

			QString oldName(name);

			CElementEditorDialog elementEditor;
			if (elementEditor.Execute(name, description)){
				istd::CChangeNotifier changePtr(objectPtr);

				if (!objectPtr->SetParamsSetName(index, name)){
					QMessageBox::critical(
								NULL,
								tr("Rename error"),
								tr("Parameter set '%1' can not be renamed to '%2'").arg(oldName, name));

					// restore element index here 
					ElementList->setCurrentRow(index);

					return;
				}

				objectPtr->SetParamsSetDescription(index, description);
			}
			else{
				return;
			}

			// restore element index here 
			ElementList->setCurrentRow(index);
		}
	}
}


void CExtParamsManagerGuiComp::on_RemoveButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const iprm::IOptionsList* elementListPtr = objectPtr->GetSelectionConstraints();
		Q_ASSERT(elementListPtr != NULL);

		int index = ElementList->currentIndex().row();
		if (index >= 0){
			int result = QMessageBox::question(NULL, 
						tr("Remove parameter set"), 
						tr("Are you sure to remove the parameter set '%1'?").arg(elementListPtr->GetOptionName(index)),
						QMessageBox::Ok | QMessageBox::Cancel, 
						QMessageBox::Cancel);
			
			if (result == QMessageBox::Ok){
				if (objectPtr->RemoveParamsSet(index)){
					ElementList->setCurrentRow(qMin(index, elementListPtr->GetOptionsCount()-1));
				}
			}
		}
	}
}


void CExtParamsManagerGuiComp::on_ElementList_currentRowChanged(int index)
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		AddButton->setEnabled(true);
		CloneButton->setEnabled(index >= 0);
		EditButton->setEnabled(index >= 0);
		RemoveButton->setEnabled(index >= 0);
		ConfigureButton->setEnabled(index >= 0);
	}
	else{
		AddButton->setEnabled(false);
		CloneButton->setEnabled(false);
		EditButton->setEnabled(false);
		RemoveButton->setEnabled(false);
		ConfigureButton->setEnabled(false);
	}
}


void CExtParamsManagerGuiComp::on_ElementList_doubleClicked(const QModelIndex &index)
{
	if (index.isValid()){
		on_EditButton_clicked();
	}
}


// protected members

// internal class CElementItemDelegate

void CExtParamsManagerGuiComp::CElementItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItemV4 itemOption;
	initStyleOption(&itemOption, index);

	int iconWidth = qMax(0, option.decorationSize.width());

	bool isSelected = (option.state & QStyle::State_HasFocus) | (option.state & QStyle::State_Selected);
	if (isSelected){
		painter->setBrush(itemOption.palette.highlight());
	}
	else{
		painter->setBrush(itemOption.backgroundBrush);
	}

	painter->setPen(option.palette.highlight().color());

	QRect itemRect(option.rect.adjusted(-1,-1,0,-1));
	painter->drawRect(itemRect);
	
	// icon (if available)
	if (iconWidth > 0){
		QPixmap icon = itemOption.icon.pixmap(iconWidth);
		if (!icon.isNull()){
			painter->drawPixmap(option.rect.left() + 4, option.rect.top() + 4, icon);
		}
		else{
			iconWidth = 0;
		}
	}

	// text
	QRect textRect(option.rect.adjusted(iconWidth > 0 ? iconWidth + 8 : 4, 4, -4, -4));

	if (isSelected)
	{
		painter->setPen(option.palette.highlightedText().color());
	}
	else
	{
		painter->setPen(option.palette.text().color());
	}

	QFont fnt(option.font);
	fnt.setPointSize(12);
	painter->setFont(fnt);

	QByteArray parameterType = index.data(DR_PARAMETER_TYPE_ROLE).toByteArray();

	fnt.setPointSize(8);
	QString version = QString("%1").arg(parameterType.constData());

	int versionWidth = QFontMetrics(fnt).width(version);
	
	QString textToPaint(painter->fontMetrics().elidedText(itemOption.text, option.textElideMode, textRect.width() - versionWidth));
	painter->drawText(textRect, Qt::AlignLeft | Qt::AlignTop, textToPaint);

	// description
	if (!isSelected){
		painter->setPen(option.palette.highlight().color());
	}

	fnt.setPointSize(8);
	painter->setFont(fnt);

	QString descriptionToPaint(painter->fontMetrics().elidedText(index.data(DR_DESCRIPTION_ROLE).toString(), option.textElideMode, textRect.width()));
	painter->drawText(textRect, Qt::AlignLeft | Qt::AlignBottom, descriptionToPaint);

	// version
	painter->drawText(textRect, Qt::AlignRight | Qt::AlignTop, version);
}


QSize CExtParamsManagerGuiComp::CElementItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return QSize(QStyledItemDelegate::sizeHint(option, index).width(), 48);
}


// internal class CElementEditorDialog

CExtParamsManagerGuiComp::CElementEditorDialog::CElementEditorDialog()
{
	_ui.setupUi(this);
}


bool CExtParamsManagerGuiComp::CElementEditorDialog::Execute(QString &name, QString &description)
{
	_ui.NameEdit->setText(name);
	_ui.NameEdit->selectAll();
	_ui.NameEdit->setFocus();

	_ui.DescriptionEdit->setText(description);

	if (exec() == QDialog::Accepted){
		name = _ui.NameEdit->text();
		description = _ui.DescriptionEdit->text();

		return true;
	}

	return false;
}


} // namespace iqtprm


