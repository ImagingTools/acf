#ifndef iqt2d_TPolygonBasedParamsGuiComp_included
#define iqt2d_TPolygonBasedParamsGuiComp_included


// Qt includes
#include <QtGui/QItemDelegate>
#include <QtGui/QLineEdit>
#include <QtGui/QDoubleValidator>
#include <QtGui/QClipboard>
#include <QtCore/QMimeData>
#include <QtCore/QByteArray>
#include <QtGui/QMenu>

// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/CMemoryWriteArchive.h"
#include "iser/CMemoryReadArchive.h"

#include "i2d/CPolygon.h"
#include "i2d/CPolyline.h"

#include "iqt/CSignalBlocker.h"

#include "iqt2d/TShapeParamsGuiCompBase.h"
#include "iqt2d/Generated/ui_CPolygonParamsGuiComp.h"


namespace iqt2d
{


template <class PolygonBasedShape, class PolygonBasedModel>
class TPolygonBasedParamsGuiComp: public iqt2d::TShapeParamsGuiCompBase<
			Ui::CPolygonParamsGuiComp,
			PolygonBasedShape,
			PolygonBasedModel>
{
public:

	/**
		Cell index
	 */
	enum CellIndex
	{
		/**
			Index of the table cell for the X coordinate of the polygon node
		 */
		CI_X,

		/**
			Index of the table cell for the X coordinate of the polygon node
		 */
		CI_Y,

		/**
			Last used cell index
		 */
		CI_LAST = CI_Y
	};

	typedef iqt2d::TShapeParamsGuiCompBase<
				Ui::CPolygonParamsGuiComp,
				PolygonBasedShape,
				PolygonBasedModel> BaseClass;

	I_BEGIN_COMPONENT(TPolygonBasedParamsGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	/**
		Get the table with the node data.
	 */
	QTableWidget* GetNodeTable();

	virtual void OnInsertNode();
	virtual void OnRemoveNode();
	virtual void OnCopyData();
	virtual void OnPasteData();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	/**
		Enable or disable the Tools button depending on model type and dynamically build its menu.
	 */
	void UpdateToolsMenuButton();

	void OnToolsButtonMenuActionTriggered(QAction* action);

protected:
	using BaseClass::GetObjectPtr;
	using BaseClass::DoUpdateModel;
	using BaseClass::NodeParamsTable;
	using BaseClass::tr;

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
	}; // CPolygonParamsGuiItemDelegate


	enum MenuAction
	{
		ActionFlipHorizontally, ActionFlipVertically, ActionRotateClockwise, ActionRotateCounterclockwise, ActionReverseLine
	};
};


// public methods

// reimplemented (imod::IModelEditor)

template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	i2d::CPolygon* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	istd::CChangeNotifier changePtr(objectPtr);

	int count = NodeParamsTable->rowCount();
	objectPtr->Clear();

	for (int i = 0; i < count; i++){
		double x = NodeParamsTable->item(i, CI_X)->text().toDouble();
		double y = NodeParamsTable->item(i, CI_Y)->text().toDouble();

		objectPtr->InsertNode(i2d::CVector2d(x, y));
	}
}


// protected methods

template <class PolygonBasedShape, class PolygonBasedModel>
QTableWidget* TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::GetNodeTable()
{
	return NodeParamsTable;
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnInsertNode()
{
	iqt::CSignalBlocker block(NodeParamsTable);

	int row = NodeParamsTable->currentRow();
	if (row < 0){
		row = NodeParamsTable->rowCount();

		NodeParamsTable->setRowCount(row + 1);
	}
	else{
		NodeParamsTable->insertRow(row);
	}

	NodeParamsTable->setItem(row, CI_X, new QTableWidgetItem(QString::number(0)));
	NodeParamsTable->setItem(row, CI_Y, new QTableWidgetItem(QString::number(0)));
	NodeParamsTable->setCurrentCell(row, 0);

	DoUpdateModel();
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnCopyData()
{
	PolygonBasedModel* objectPtr = dynamic_cast<PolygonBasedModel*>(GetObjectPtr());
	if (objectPtr == NULL){
		return;
	}

	iser::CMemoryWriteArchive archive;

	if (objectPtr->Serialize(archive)){
		QByteArray data((const char*)archive.GetBuffer(), archive.GetBufferSize());

		QMimeData* dataPtr = new QMimeData();
		dataPtr->setData(typeid (PolygonBasedModel).name(), data);

		QApplication::clipboard()->setMimeData(dataPtr);
	}
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnPasteData()
{
	PolygonBasedModel* objectPtr = dynamic_cast<PolygonBasedModel*>(GetObjectPtr());
	if (objectPtr == NULL){
		return;
	}

	const QMimeData* dataPtr = QApplication::clipboard()->mimeData();
	if (dataPtr != NULL){
		if (dataPtr->hasFormat(typeid (PolygonBasedModel).name())){
			QByteArray data(dataPtr->data(typeid (PolygonBasedModel).name()));

			iser::CMemoryReadArchive archive(data.data(), data.size());

			objectPtr->Serialize(archive);
		}
	}
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnRemoveNode()
{
	int row = NodeParamsTable->currentRow();
	if (row >= 0){
		iqt::CSignalBlocker block(NodeParamsTable);

		NodeParamsTable->removeRow(row);

		DoUpdateModel();
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	QObject::connect(NodeParamsTable, SIGNAL(cellChanged(int, int)), this, SLOT(OnParamsChanged()));

	UpdateToolsMenuButton();
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnGuiModelDetached()
{
	UpdateToolsMenuButton();

	NodeParamsTable->disconnect();

	BaseClass::OnGuiModelDetached();
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());

	i2d::CPolygon* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		NodeParamsTable->clearContents();

		int count = objectPtr->GetNodesCount();
		NodeParamsTable->setRowCount(count);

		for (int i = 0; i < count; i++){
			i2d::CVector2d coord = objectPtr->GetNode(i);
			NodeParamsTable->setItem(i, 0, new QTableWidgetItem(QString::number(coord.GetX())));
			NodeParamsTable->setItem(i, 1, new QTableWidgetItem(QString::number(coord.GetY())));
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	NodeParamsTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	CPolygonParamsGuiItemDelegate* columnDelegate = new CPolygonParamsGuiItemDelegate();
	NodeParamsTable->setItemDelegateForColumn(0, columnDelegate);
	NodeParamsTable->setItemDelegateForColumn(1, columnDelegate);

	BaseClass::CloseLineCheckBox->setHidden(true);
	UpdateToolsMenuButton();
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::UpdateToolsMenuButton()
{
	BaseClass::ToolsButton->setHidden(true);
	i2d::CPolyline* polylinePtr = dynamic_cast<i2d::CPolyline*>(BaseClass::GetModelPtr());
	i2d::CPolygon* polygonPtr = dynamic_cast<i2d::CPolygon*>(BaseClass::GetModelPtr());

	if (polylinePtr != NULL || polygonPtr != NULL){
		BaseClass::ToolsButton->setHidden(false);
		if (BaseClass::ToolsButton->menu() == NULL){
			BaseClass::ToolsButton->setMenu(new QMenu(BaseClass::ToolsButton));
			BaseClass::connect(BaseClass::ToolsButton->menu(), SIGNAL(triggered(QAction*)),
					this, SLOT(OnToolsButtonMenuActionTriggered(QAction*)));
		}

		QMenu& menu = *BaseClass::ToolsButton->menu();
		menu.clear();

		menu.addAction(QIcon(":/Icons/FlipHorizontal.svg"), tr("Flip horizontally"))->setData(ActionFlipHorizontally);
		menu.addAction(QIcon(":/Icons/FlipVertical.svg"), tr("Flip vertically"))->setData(ActionFlipVertically);
		menu.addAction(QIcon(":/Icons/RotateRight.svg"), tr("Rotate clockwise"))->setData(ActionRotateClockwise);
		menu.addAction(QIcon(":/Icons/RotateLeft.svg"), tr("Rotate counterclockwise"))->setData(ActionRotateCounterclockwise);
		if (polylinePtr != NULL){
			menu.addAction(QIcon(":/Icons/Reverse.svg"), tr("Reverse line"))->setData(ActionReverseLine);
		}
	}
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnToolsButtonMenuActionTriggered(QAction* action)
{
	i2d::CPolygon* polygonPtr = dynamic_cast<i2d::CPolygon*>(BaseClass::GetModelPtr());
	if (polygonPtr != NULL){
		istd::CChangeNotifier notifier(polygonPtr);

		i2d::CVector2d center = polygonPtr->GetCenter();
		int count = polygonPtr->GetNodesCount();

		switch (action->data().toInt()){
			case ActionFlipHorizontally:
				for (int i = 0; i < count; i++){
					i2d::CVector2d node = polygonPtr->GetNode(i);
					node.SetX(center.GetX() + (center.GetX() - node.GetX()));
					polygonPtr->SetNode(i, node);
				}
				break;

			case ActionFlipVertically:
				for (int i = 0; i < count; i++){
					i2d::CVector2d node = polygonPtr->GetNode(i);
					node.SetY(center.GetY() + (center.GetY() - node.GetY()));
					polygonPtr->SetNode(i, node);
				}
				break;

			case ActionRotateClockwise:
			{
				i2d::CAffineTransformation2d translateTo00;
				translateTo00.Reset(-center);
				i2d::CAffineTransformation2d rotate;
				rotate.Reset(i2d::CVector2d(0, 0), M_PI / 2);
				i2d::CAffineTransformation2d translateBackToCenter;
				translateBackToCenter.Reset(center);
				polygonPtr->Transform(translateTo00);
				polygonPtr->Transform(rotate);
				polygonPtr->Transform(translateBackToCenter);
			}
				break;

			case ActionRotateCounterclockwise:
			{
				i2d::CAffineTransformation2d translateTo00;
				translateTo00.Reset(-center);
				i2d::CAffineTransformation2d rotate;
				rotate.Reset(i2d::CVector2d(0, 0), -M_PI / 2);
				i2d::CAffineTransformation2d translateBackToCenter;
				translateBackToCenter.Reset(center);
				polygonPtr->Transform(translateTo00);
				polygonPtr->Transform(rotate);
				polygonPtr->Transform(translateBackToCenter);
			}
				break;

			case ActionReverseLine:
				for (int i = 0; i < count / 2; i++){
					i2d::CVector2d node1 = polygonPtr->GetNode(i);
					i2d::CVector2d node2 = polygonPtr->GetNode(count - 1 - i);
					polygonPtr->SetNode(i, node2);
					polygonPtr->SetNode(count - 1 - i, node1);
				}
				break;

			default: break;
		}
	}
}



} // namespace iqt2d


#endif // !iqt2d_TPolygonBasedParamsGuiComp_included


