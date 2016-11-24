#include <iqtprm/CExtParamsManagerGuiComp.h>


// Qt includes
#include <QtGui/QPainter>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QMessageBox>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iprm/IOptionsList.h>
#include <iprm/TParamsPtr.h>
#include <iprm/CParamsSet.h>
#include <iqtgui/CGuiComponentDialog.h>


namespace iqtprm
{


// protected methods

void CExtParamsManagerGuiComp::UpdateCommands()
{
	QList<QListWidgetItem*> selectedItems = ElementList->selectedItems();

	CloneButton->setEnabled(!selectedItems.isEmpty());
	EditButton->setEnabled(!selectedItems.isEmpty());
	RemoveButton->setEnabled(!selectedItems.isEmpty());
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CExtParamsManagerGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	QByteArray lastSelectedParamsSetId = m_lastSelectedParameterSetId;

	iprm::IParamsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		ElementList->clear();
		ParameterTypeSelector->clear();

		const iprm::IOptionsList* paramsSetTypeListPtr = objectPtr->GetParamsTypeConstraints();
		if (paramsSetTypeListPtr != NULL)
		{
			for (int typeIndex = 0; typeIndex < paramsSetTypeListPtr->GetOptionsCount(); ++ typeIndex)
			{
				if (m_parameterEditorMap.contains(paramsSetTypeListPtr->GetOptionId(typeIndex)))
				{
					ParameterTypeSelector->addItem(paramsSetTypeListPtr->GetOptionName(typeIndex), paramsSetTypeListPtr->GetOptionId(typeIndex));
				}
			}
		}
		else
		{
			ParameterTypeSelector->setVisible(false);
		}

		int itemsCount = objectPtr->GetParamsSetsCount();

		const iprm::IOptionsList* elementListPtr = dynamic_cast<const iprm::IOptionsList*>(objectPtr);

		for (int i = 0; i < itemsCount; i++){
			const iprm::IParamsSet* paramsSetPtr = objectPtr->GetParamsSet(i);

			if ((paramsSetPtr == NULL) || m_parameterEditorMap.contains(paramsSetPtr->GetFactoryId()))
			{
				QString name = objectPtr->GetParamsSetName(i);
				QString description = objectPtr->GetParamsSetDescription(i);

				QListWidgetItem* itemPtr = new QListWidgetItem(name, ElementList);
				itemPtr->setData(DR_DESCRIPTION, description);

				if (paramsSetPtr != NULL)
				{
					itemPtr->setData(DR_PARAMETER_TYPE, paramsSetPtr->GetFactoryId());
				}

				itemPtr->setData(DR_INDEX, QVariant::fromValue(i));

				if (elementListPtr != NULL){
					QByteArray templateId = elementListPtr->GetOptionId(i);
					itemPtr->setData(DR_UUID, templateId);
					itemPtr->setSelected(lastSelectedParamsSetId == templateId);
				}
			}
		}

		UpdateCommands();
	}
	else
	{
		AddButton->setEnabled(false);
		CloneButton->setEnabled(false);
		EditButton->setEnabled(false);
		RemoveButton->setEnabled(false);
	}

	ParameterTypeSelector->setVisible(ParameterTypeSelector->count() > 1);
}


// reimplemented (iqtgui::CGuiComponentBase)

void CExtParamsManagerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	ElementList->setItemDelegate(new CElementItemDelegate());
	ElementList->setIconSize(QSize(32,32));

	// Build mapping between parameter editors and worker type IDs:
	int editorsCount = qMin(m_parameterEditorIdsAttrPtr.GetCount(), m_parameterGuisCompPtr.GetCount());
	for (int i = 0; i < editorsCount; ++i)
	{
		ParameterEditor parameterEditor;
		parameterEditor.guiPtr = m_parameterGuisCompPtr[i];

		if (i < m_parameterObserversCompPtr.GetCount())
		{
			parameterEditor.observerPtr = m_parameterObserversCompPtr[i];
		}

		if (i < m_parameterEditorsCompPtr.GetCount())
		{
			parameterEditor.editorPtr = m_parameterEditorsCompPtr[i];
		}

		if ( (parameterEditor.guiPtr != NULL) && (parameterEditor.observerPtr != NULL) && (parameterEditor.editorPtr != NULL))
		{
			m_parameterEditorMap[m_parameterEditorIdsAttrPtr[i]] = parameterEditor;
		}
	}
}


// protected slots

void CExtParamsManagerGuiComp::on_AddButton_clicked()
{
	int parameterTypeIndex = ParameterTypeSelector->currentIndex();

	iprm::IParamsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int newParamsSetIndex = objectPtr->InsertParamsSet(parameterTypeIndex);
		if (newParamsSetIndex >= 0){
			const iprm::IOptionsList* paramSetListPtr = objectPtr->GetSelectionConstraints();
			if (paramSetListPtr != NULL){
				QByteArray newParameterId = paramSetListPtr->GetOptionId(newParamsSetIndex);

				if (!newParameterId.isEmpty()){
					for (int i = 0; i < ElementList->count(); ++i)
					{
						QListWidgetItem* newItemPtr = ElementList->item(i);

						if (newItemPtr->data(DR_UUID).toByteArray() == newParameterId)
						{
							newItemPtr->setSelected(true);
							break;
						}
					}

					on_EditButton_clicked();
				}
			}
		}
	}
}


void CExtParamsManagerGuiComp::on_CloneButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		QList<QListWidgetItem*> selectedItems = ElementList->selectedItems();
		if (!selectedItems.isEmpty()){
			int paramSetIndex = selectedItems.at(0)->data(DR_INDEX).toInt();
			if (paramSetIndex >= 0){
				const iprm::IParamsSet* sourceParamsPtr = objectPtr->GetParamsSet(paramSetIndex);
				Q_ASSERT(sourceParamsPtr != NULL);

				int parameterTypeIndex = -1;

				QByteArray sourceTypeId = sourceParamsPtr->GetFactoryId();
				if (!sourceTypeId.isEmpty()){
					const iprm::IOptionsList* typeConstraintsPtr = objectPtr->GetParamsTypeConstraints();
					if (typeConstraintsPtr != NULL){
						for (int i = 0; i < typeConstraintsPtr->GetOptionsCount(); ++i){
							if (typeConstraintsPtr->GetOptionId(i) == sourceTypeId){
								parameterTypeIndex = i;
								break;
							}
						}
					}
				}

				istd::IChangeable::ChangeSet changeSet(iprm::IParamsManager::CF_SET_INSERTED);
				istd::CChangeNotifier changeNotifier(objectPtr, &changeSet);

				int newParamsSetIndex = objectPtr->InsertParamsSet(parameterTypeIndex);
				if (newParamsSetIndex >= 0){
					iprm::IParamsSet* targetParamsPtr = objectPtr->GetParamsSet(paramSetIndex);
					Q_ASSERT(targetParamsPtr != NULL);

					if (targetParamsPtr->CopyFrom(*sourceParamsPtr)){
						const iprm::IOptionsList* paramSetListPtr = objectPtr->GetSelectionConstraints();
						if (paramSetListPtr != NULL){
							QByteArray newParameterId = paramSetListPtr->GetOptionId(newParamsSetIndex);

							if (!newParameterId.isEmpty()){
								for (int i = 0; i < ElementList->count(); ++i)
								{
									QListWidgetItem* newItemPtr = ElementList->item(i);

									if (newItemPtr->data(DR_UUID).toByteArray() == newParameterId)
									{
										newItemPtr->setSelected(true);
										break;
									}
								}
							}
						}
					}
					else{
						objectPtr->RemoveParamsSet(newParamsSetIndex);
					}
				}
			}
		}
	}
}


void CExtParamsManagerGuiComp::on_EditButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObservedObject();
	if (objectPtr == NULL)
	{
		return;
	}

	QList<QListWidgetItem*> selectedItems = ElementList->selectedItems();
	if (!selectedItems.isEmpty())
	{
		int paramSetIndex = selectedItems.at(0)->data(DR_INDEX).toInt();
		QByteArray parameterTypeId = selectedItems.at(0)->data(DR_PARAMETER_TYPE).toByteArray();

		QString name = objectPtr->GetParamsSetName(paramSetIndex);
		QString description = objectPtr->GetParamsSetDescription(paramSetIndex);

		QString oldName(name);

		iprm::IParamsSet* paramsSetPtr = objectPtr->GetParamsSet(paramSetIndex);
		if (paramsSetPtr != NULL){
			// Create working instance of the Parameter set parameters:
			istd::TDelPtr<iprm::IParamsSet> workingParametersPtr(objectPtr->CreateParameterSet(-1, paramSetIndex));
			if (workingParametersPtr.IsValid()){
				imod::IModel* paramsSetModelPtr = dynamic_cast<imod::IModel*>(workingParametersPtr.GetPtr());
				if (paramsSetModelPtr == NULL){
					return;
				}

				// Editor for the worker type does exist?
				if (m_parameterEditorMap.contains(parameterTypeId))
				{
					ParameterEditor& editor = m_parameterEditorMap[parameterTypeId];

					// ...then connect the parameter model to the editor and show it:
					if (!paramsSetModelPtr->IsAttached(editor.observerPtr))
					{
						if (paramsSetModelPtr->AttachObserver(editor.observerPtr))
						{
							CElementEditorDialog editDialog;
							editDialog.setWindowTitle(tr("Edit Parameters"));

							if (editDialog.Execute(name, description, m_parameterEditorMap[parameterTypeId].guiPtr) == QDialog::Accepted)
							{
								istd::CChangeNotifier updatePtr(objectPtr);

								if (!objectPtr->SetParamsSetName(paramSetIndex, name))
								{
									QMessageBox::critical(
										NULL,
										tr("Rename error"),
										tr("Parameter set '%1' can not be renamed to '%2'").arg(oldName, name));
								}

								objectPtr->SetParamsSetDescription(paramSetIndex, description);

								if (!paramsSetPtr->CopyFrom(*workingParametersPtr.GetPtr())){
									QMessageBox::critical(
												NULL,
												tr("Parameter error"),
												tr("Parameter set '%1'can not be changed").arg(oldName));
								}
							}

							paramsSetModelPtr->DetachObserver(editor.observerPtr);
						}
					}
				}
			}
		}
	}
}


void CExtParamsManagerGuiComp::on_RemoveButton_clicked()
{
	QList<QListWidgetItem*> selectedItems = ElementList->selectedItems();
	if (selectedItems.isEmpty()){
		return;
	}

	int paramSetIndex = selectedItems.at(0)->data(DR_INDEX).toInt();
	if (paramSetIndex < 0){
		return;
	}

	iprm::IParamsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int result = QMessageBox::question(NULL, 
					tr("Remove parameter set"), 
					tr("Are you sure to remove the parameter set '%1'?").arg(objectPtr->GetParamsSetName(paramSetIndex)),
					QMessageBox::Ok | QMessageBox::Cancel, 
					QMessageBox::Cancel);
			
		if (result == QMessageBox::Ok){
			if (!objectPtr->RemoveParamsSet(paramSetIndex)){
				// TODO: Show error message!
			}
		}
	}
}


void CExtParamsManagerGuiComp::on_ReloadButton_clicked()
{
}


void CExtParamsManagerGuiComp::on_ElementList_doubleClicked(const QModelIndex &index)
{
	if (index.isValid()){
		on_EditButton_clicked();
	}
}


void CExtParamsManagerGuiComp::on_ElementList_itemSelectionChanged()
{
	m_lastSelectedParameterSetId.clear();

	QList<QListWidgetItem*> selectedItems = ElementList->selectedItems();
	if (!selectedItems.isEmpty())
	{
		m_lastSelectedParameterSetId = selectedItems[0]->data(DR_UUID).toByteArray();
	}

	UpdateCommands();
}


// protected members

// internal class CElementItemDelegate

void CExtParamsManagerGuiComp::CElementItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItem itemOption;
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

	QByteArray parameterType = index.data(DR_PARAMETER_TYPE).toByteArray();

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

	QString descriptionToPaint(painter->fontMetrics().elidedText(index.data(DR_DESCRIPTION).toString(), option.textElideMode, textRect.width()));
	painter->drawText(textRect, Qt::AlignLeft | Qt::AlignBottom, descriptionToPaint);

	// version
	painter->drawText(textRect, Qt::AlignRight | Qt::AlignTop, version);
}


QSize CExtParamsManagerGuiComp::CElementItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QFontMetrics fontMetrics(option.font);

	int minHeight = 2 * fontMetrics.height() + fontMetrics.lineSpacing();

	int height = qMax(minHeight, 48);

	return QSize(QStyledItemDelegate::sizeHint(option, index).width(), height);
}


// internal class CElementEditorDialog

CExtParamsManagerGuiComp::CElementEditorDialog::CElementEditorDialog()
:	m_guiObjectPtr(NULL)
{
	m_ui.setupUi(this);

	connect(m_ui.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(m_ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));}


CExtParamsManagerGuiComp::CElementEditorDialog::~CElementEditorDialog()
{
	if ((m_guiObjectPtr != NULL) && m_guiObjectPtr->IsGuiCreated())
	{
		m_guiObjectPtr->DestroyGui();
	}
}


int CExtParamsManagerGuiComp::CElementEditorDialog::Execute(QString &name, QString &description, iqtgui::IGuiObject* paramsGuiPtr)
{
	m_guiObjectPtr = paramsGuiPtr;

	m_ui.NameEdit->setText(name);
	m_ui.NameEdit->selectAll();
	m_ui.NameEdit->setFocus();

	m_ui.DescriptionEdit->setText(description);

	bool isGuiCreated = (paramsGuiPtr != NULL) ? paramsGuiPtr->CreateGui(m_ui.ParamsFrame) : true;

	if (isGuiCreated && (exec() == QDialog::Accepted)){
		name = m_ui.NameEdit->text();
		description = m_ui.DescriptionEdit->text();

		return QDialog::Accepted;
	}

	return QDialog::Rejected;
}


} // namespace iqtprm


