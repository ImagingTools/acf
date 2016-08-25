#include "iqt2d/CTubePolylineParamsGuiComp.h"


namespace iqt2d
{
	

// protected slots


void CTubePolylineParamsGuiComp::OnActionTriggered(QAction* actionPtr)
{
	BaseClass::OnActionTriggered(actionPtr);
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


// protected methods

// reimplemented (iqt2d::TPolygonBasedParamsGuiComp<iview::CTubePolylineShape, i2d::CTubePolyline>)

int CTubePolylineParamsGuiComp::columnCount(const QModelIndex& /*parent*/) const
{
	return 4;
}


QVariant CTubePolylineParamsGuiComp::data(const QModelIndex& index, int role) const
{
	if (role == Qt::DisplayRole || role == Qt::EditRole){
		i2d::CTubePolyline* objectPtr = GetObservedObject();
		if (objectPtr != NULL){
			switch (index.column()){
			case 2:
				return objectPtr->GetTNodeData(index.row()).GetTubeRange().GetMinValue();
			case 3:
				return objectPtr->GetTNodeData(index.row()).GetTubeRange().GetMaxValue();	
			default:
				break;
			}
		}
	}

	return BaseClass::data(index, role);
}


bool CTubePolylineParamsGuiComp::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (role == Qt::EditRole){
		i2d::CTubePolyline* objectPtr = GetObservedObject();
		if (objectPtr != NULL){
			istd::CChangeNotifier notifier(objectPtr);
			Q_UNUSED(notifier);

			istd::CRange range = objectPtr->GetTNodeDataRef(index.row()).GetTubeRange();
			switch (index.column()){
			case 2:
				range.SetMinValue(value.toDouble());
				objectPtr->GetTNodeDataRef(index.row()).SetTubeRange(range);
				return true;
			case 3:
				range.SetMaxValue(value.toDouble());
				objectPtr->GetTNodeDataRef(index.row()).SetTubeRange(range);
				return true;
			default:
				break;
			}
		}
	}

	return BaseClass::setData(index, value, role);
}


QVariant CTubePolylineParamsGuiComp::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole){
		if (orientation == Qt::Horizontal){
			switch (section){
			case 2:
				return tr("Left");
			case 3:
				return tr("Right");
			default:
				break;
			}
		}
	}

	return BaseClass::headerData(section, orientation, role);
}


} // namespace iqt2d


