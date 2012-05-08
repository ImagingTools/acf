#include "iqt2d/CPolygonParamsGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqt2d
{


// public methods

// reimplemented (imod::IModelEditor)

void CPolygonParamsGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	i2d::CPolygon* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	istd::CChangeNotifier changePtr(objectPtr);

	int count = CoordsTable->rowCount();
	objectPtr->Clear();

	for (int i = 0; i < count; i++){
		double x = CoordsTable->item(i, 0)->text().toDouble();
		double y = CoordsTable->item(i, 1)->text().toDouble();
	
		objectPtr->InsertNode(i2d::CVector2d(x, y));
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CPolygonParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	CoordsTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	CPolygonParamsGuiItemDelegate* columnDelegate = new CPolygonParamsGuiItemDelegate();
	CoordsTable->setItemDelegateForColumn(0, columnDelegate);
	CoordsTable->setItemDelegateForColumn(1, columnDelegate);
}


void CPolygonParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	QObject::connect(CoordsTable, SIGNAL(cellChanged(int,int)), this, SLOT(OnParamsChanged()));
}


void CPolygonParamsGuiComp::OnGuiModelDetached()
{
	CoordsTable->disconnect();

	BaseClass::OnGuiModelDetached();
}


void CPolygonParamsGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	i2d::CPolygon* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		CoordsTable->clearContents();

		int count = objectPtr->GetNodesCount();
		CoordsTable->setRowCount(count);

		for (int i = 0; i < count; i++){
			i2d::CVector2d coord = objectPtr->GetNode(i);
			CoordsTable->setItem(i, 0, new QTableWidgetItem(QString::number(coord.GetX())));
			CoordsTable->setItem(i, 1, new QTableWidgetItem(QString::number(coord.GetY())));
		}
	}
}


// protected slots

void CPolygonParamsGuiComp::OnParamsChanged()
{
	DoUpdateModel();
}


void CPolygonParamsGuiComp::on_AppendButton_clicked()
{
	iqt::CSignalBlocker block(CoordsTable);

	int row = CoordsTable->rowCount();
	CoordsTable->setRowCount(row + 1);
	CoordsTable->setItem(row, 0, new QTableWidgetItem(QString::number(0)));
	CoordsTable->setItem(row, 1, new QTableWidgetItem(QString::number(0)));
	CoordsTable->setCurrentCell(row, 0);

	OnParamsChanged();
}


void CPolygonParamsGuiComp::on_InsertButton_clicked()
{
	iqt::CSignalBlocker block(CoordsTable);

	int row = CoordsTable->currentRow();
	CoordsTable->insertRow(row);
	CoordsTable->setItem(row, 0, new QTableWidgetItem(QString::number(0)));
	CoordsTable->setItem(row, 1, new QTableWidgetItem(QString::number(0)));
	CoordsTable->setCurrentCell(row, 0);

	OnParamsChanged();
}


void CPolygonParamsGuiComp::on_RemoveButton_clicked()
{
	int row = CoordsTable->currentRow();
	if (row >= 0){
		iqt::CSignalBlocker block(CoordsTable);

		CoordsTable->removeRow(row);

		OnParamsChanged();
	}
}


} // namespace iqt2d
