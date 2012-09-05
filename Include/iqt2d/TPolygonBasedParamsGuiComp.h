#ifndef iqt2d_TPolygonBasedParamsGuiComp_included
#define iqt2d_TPolygonBasedParamsGuiComp_included


// Qt includes
#include <QtGui/QItemDelegate>
#include <QtGui/QLineEdit>
#include <QtGui/QDoubleValidator>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "i2d/CPolygon.h"

#include "iqt/CSignalBlocker.h"

#include "iqt2d/TShapeParamsGuiCompBase.h"
#include "iqt2d/Generated/ui_CPolygonParamsGuiComp.h"


namespace iqt2d
{


template <class PolygonBasedShape, class PolygonBasedModel>
class TPolygonBasedParamsGuiComp:
	public iqt2d::TShapeParamsGuiCompBase<
				Ui::CPolygonParamsGuiComp,
				PolygonBasedShape,
				PolygonBasedModel>
{
public:
	typedef iqt2d::TShapeParamsGuiCompBase<
				Ui::CPolygonParamsGuiComp,
				PolygonBasedShape,
				PolygonBasedModel> BaseClass;

	I_BEGIN_COMPONENT(TPolygonBasedParamsGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	virtual void OnInsertNode();
	virtual void OnRemoveNode();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

protected:
    using BaseClass::GetObjectPtr;
    using BaseClass::DoUpdateModel;
    using Ui::CPolygonParamsGuiComp::CoordsTable;

	/** 
		Internal item delegate class for input validation
	*/
	class CPolygonParamsGuiItemDelegate: public QItemDelegate
	{
	public:
		virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const
		{
			QLineEdit* editorPtr = new QLineEdit(parent);
			editorPtr->setValidator(new QDoubleValidator());
			return editorPtr;
		}
	};
};



// public methods

// reimplemented (imod::IModelEditor)

template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::UpdateModel() const
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

template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnInsertNode()
{
	iqt::CSignalBlocker block(CoordsTable);

	int row = CoordsTable->currentRow();
	if (row < 0){
		row = CoordsTable->rowCount();
		
		CoordsTable->setRowCount(row + 1);
	}
	else{
		CoordsTable->insertRow(row);
	}

	CoordsTable->setItem(row, 0, new QTableWidgetItem(QString::number(0)));
	CoordsTable->setItem(row, 1, new QTableWidgetItem(QString::number(0)));
	CoordsTable->setCurrentCell(row, 0);

	DoUpdateModel();
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnRemoveNode()
{
	int row = CoordsTable->currentRow();
	if (row >= 0){
		iqt::CSignalBlocker block(CoordsTable);

		CoordsTable->removeRow(row);

		DoUpdateModel();
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	QObject::connect(CoordsTable, SIGNAL(cellChanged(int,int)), this, SLOT(OnParamsChanged()));
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnGuiModelDetached()
{
	CoordsTable->disconnect();

	BaseClass::OnGuiModelDetached();
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::UpdateGui(int /*updateFlags*/)
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


// reimplemented (iqtgui::CGuiComponentBase)

template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	CoordsTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	CPolygonParamsGuiItemDelegate* columnDelegate = new CPolygonParamsGuiItemDelegate();
	CoordsTable->setItemDelegateForColumn(0, columnDelegate);
	CoordsTable->setItemDelegateForColumn(1, columnDelegate);
}


} // namespace iqt2d


#endif // !iqt2d_TPolygonBasedParamsGuiComp_included


