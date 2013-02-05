#include "iqt2d/CTubePolylineParamsGuiComp.h"


namespace iqt2d
{
	

// public methods

// reimplemented (imod::IModelEditor)

void CTubePolylineParamsGuiComp::UpdateModel() const
{
	i2d::CTubePolyline* objectPtr = dynamic_cast<i2d::CTubePolyline*>(GetObjectPtr());
	
	istd::CChangeNotifier changePtr(objectPtr);

	BaseClass::UpdateModel();

	if (objectPtr != NULL){
		int count = objectPtr->GetNodesCount();
		for (int i = 0; i < count; i++){
			istd::CRange tubeRange(
						NodeParamsTable->item(i, 2)->text().toDouble(),
						NodeParamsTable->item(i, 3)->text().toDouble());

			objectPtr->GetTNodeDataRef(i).SetTubeRange(tubeRange);
		}
	}
}


// protected slots

void CTubePolylineParamsGuiComp::OnParamsChanged()
{
	DoUpdateModel();
}


void CTubePolylineParamsGuiComp::on_InsertButton_clicked()
{
	OnInsertNode();
}


void CTubePolylineParamsGuiComp::on_RemoveButton_clicked()
{
	OnRemoveNode();
}


void CTubePolylineParamsGuiComp::on_CopyButton_clicked()
{
	OnCopyData();
}


void CTubePolylineParamsGuiComp::on_PasteButton_clicked()
{
	OnPasteData();
}


void CTubePolylineParamsGuiComp::OnToolsButtonMenuActionTriggered(QAction* action)
{
	BaseClass::OnToolsButtonMenuActionTriggered(action);
}


// protected methods

// reimplemented (iqt2d::TPolygonBasedParamsGuiComp)

void CTubePolylineParamsGuiComp::OnInsertNode()
{
	{
		iqt::CSignalBlocker block(NodeParamsTable);

		UpdateBlocker updateBlocker(this);

		BaseClass::OnInsertNode();

		int row = NodeParamsTable->currentRow();
		if (row < 0){
			row = NodeParamsTable->rowCount();
		}

		NodeParamsTable->setItem(row, 2, new QTableWidgetItem(QString::number(-10)));
		NodeParamsTable->setItem(row, 3, new QTableWidgetItem(QString::number(10)));
	}

	DoUpdateModel();
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CTubePolylineParamsGuiComp::UpdateGui(int updateFlags)
{
	BaseClass::UpdateGui(updateFlags);

	Q_ASSERT(IsGuiCreated());

	i2d::CTubePolyline* objectPtr = dynamic_cast<i2d::CTubePolyline*>(GetObjectPtr());
	if (objectPtr != NULL){
		int count = objectPtr->GetNodesCount();
		for (int i = 0; i < count; i++){
			const i2d::CTubeNode& node = objectPtr->GetTNodeData(i);
			NodeParamsTable->setItem(i, 2, new QTableWidgetItem(QString::number(node.GetTubeRange().GetMinValue())));
			NodeParamsTable->setItem(i, 3, new QTableWidgetItem(QString::number(node.GetTubeRange().GetMaxValue())));
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CTubePolylineParamsGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	QStringList currentHeaderLabels;

	int columnCount = NodeParamsTable->columnCount();

	for (int columnIndex = 0; columnIndex < columnCount; columnIndex++){
		currentHeaderLabels << NodeParamsTable->horizontalHeaderItem(columnIndex)->text();
	}

	if (NodeParamsTable->columnCount() < 4){
		NodeParamsTable->setColumnCount(4);

		currentHeaderLabels << tr("Left");
		currentHeaderLabels << tr("Right");

		NodeParamsTable->setHorizontalHeaderLabels(currentHeaderLabels);
	}
}


} // namespace iqt2d


