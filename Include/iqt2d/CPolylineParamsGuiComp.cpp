#include "iqt2d/CPolylineParamsGuiComp.h"


namespace iqt2d
{


// protected slots

void CPolylineParamsGuiComp::OnParamsChanged()
{
	DoUpdateModel();
}


void CPolylineParamsGuiComp::on_InsertButton_clicked()
{
	OnInsertNode();
}


void CPolylineParamsGuiComp::on_RemoveButton_clicked()
{
	OnRemoveNode();
}


void CPolylineParamsGuiComp::on_CopyButton_clicked()
{
	OnCopyData();
}


void CPolylineParamsGuiComp::on_PasteButton_clicked()
{
	OnPasteData();
}


void CPolylineParamsGuiComp::on_NodeParamsTable_itemSelectionChanged()
{
	CloseLineCheckBox->setDisabled(NodeParamsTable->currentRow() < 0);
}


void CPolylineParamsGuiComp::on_CloseLineCheckBox_stateChanged(int state)
{
	i2d::CPolyline* polylinePtr = dynamic_cast<i2d::CPolyline*>(GetModelPtr());
	if (polylinePtr != NULL){
		istd::CChangeNotifier notifier(polylinePtr);

		if (state == Qt::Checked){
			if (!polylinePtr->IsClosed()){
				polylinePtr->SetClosed(true);
			}
		}
		else if (state == Qt::Unchecked){
			// open a line at selected point
			if (polylinePtr->IsClosed()){
				// get selected node coordinates from the table
				int row = NodeParamsTable->currentRow();
				if (row >= 0){
					double x = NodeParamsTable->item(row, 0)->data(Qt::DisplayRole).toDouble();
					double y = NodeParamsTable->item(row, 1)->data(Qt::DisplayRole).toDouble();

					// offset line nodes so that selected point is the last
					// copy the nodes and find selected node
					std::vector<i2d::CVector2d> nodes;
					int offset = 0;
					int count = polylinePtr->GetNodesCount();

					for (int i = 0; i < count; i++){
						const i2d::CVector2d& node = polylinePtr->GetNode(i);
						nodes.push_back(node);
						if (node.GetX() == x && node.GetY() == y){
							offset = i;
						}
					}

					// replace existing nodes with offset nodes
					polylinePtr->Clear();
					polylinePtr->SetClosed(false);

					for (int i = 0; i < count; i++){
						int offsetI = (i + offset) % count;
						polylinePtr->InsertNode(nodes[offsetI]);
					}
				}
			}
		}
	}
}


} // namespace iqt2d
