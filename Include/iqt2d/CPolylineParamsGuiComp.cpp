#include "iqt2d/CPolylineParamsGuiComp.h"


namespace iqt2d
{


// reimplemented (iview::IShapeFactory)

iview::IShape* CPolylineParamsGuiComp::CreateShape(const i2d::IObject2d* objectPtr, bool connectToModel) const
{
	iview::CPolylineShape* shapePtr = new iview::CPolylineShape();

	if (*m_showOrientationAttrPtr){
		shapePtr->SetOrientationVisible(true);
	}

	if (connectToModel){
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(const_cast<i2d::IObject2d*>(objectPtr));
		if (modelPtr != NULL){
			if (modelPtr->AttachObserver(shapePtr)){
				shapePtr->SetVisible(true);
			}
		}
	}

	return shapePtr;
}


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
				int row = qMax(NodeParamsTable->currentRow(), 0);
				if (row > 0){
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

					for (int i = 0; i < count; i++){
						int insertIndex = (i + offset) % count;
						polylinePtr->InsertNode(nodes[insertIndex]);
					}
				}

				polylinePtr->SetClosed(false);
			}
		}
	}
}


void CPolylineParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	UpdateClosedLineCheckBox(false, false);
}


void CPolylineParamsGuiComp::OnToolsButtonMenuActionTriggered(QAction* action)
{
	BaseClass::OnToolsButtonMenuActionTriggered(action);
}


void CPolylineParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	UpdateClosedLineCheckBox(false, false);
}


void CPolylineParamsGuiComp::OnGuiModelDetached()
{
	UpdateClosedLineCheckBox(false, true);
	
	BaseClass::OnGuiModelDetached();
}


void CPolylineParamsGuiComp::UpdateClosedLineCheckBox(bool /*forceEnabled*/, bool forceHidden)
{
	CloseLineCheckBox->setHidden(true);

	i2d::CPolyline* polylinePtr = dynamic_cast<i2d::CPolyline*>(GetModelPtr());
	if (!forceHidden && polylinePtr != NULL){
		CloseLineCheckBox->setChecked(polylinePtr->IsClosed());
		CloseLineCheckBox->setHidden(false);
	}
}


} // namespace iqt2d
