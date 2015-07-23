#ifndef iqt2d_TPolygonBasedParamsGuiComp_included
#define iqt2d_TPolygonBasedParamsGuiComp_included


// Qt includes
#include <QtCore/QMimeData>
#include <QtCore/QByteArray>
#include <QtGui/QDoubleValidator>
#include <QtGui/QClipboard>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QItemDelegate>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMenu>
#else
#include <QtGui/QItemDelegate>
#include <QtGui/QLineEdit>
#include <QtGui/QMenu>
#endif

// ACF includes
#include "istd/CChangeNotifier.h"
#include "iser/CMemoryWriteArchive.h"
#include "iser/CMemoryReadArchive.h"
#include "i2d/CPolygon.h"
#include "i2d/CPolyline.h"
#include "i2d/CAffineTransformation2d.h"
#include "iqt/CSignalBlocker.h"
#include "iqt2d/TShapeParamsGuiCompBase.h"

#include "GeneratedFiles/iqt2d/ui_CPolygonParamsGuiComp.h"


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
		Create the data object from the UI-editor.
	*/
	virtual bool GetObjectFromEditor(PolygonBasedModel& object) const;

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
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	/**
		Enable or disable the tools button depending on model type and dynamically build its menu.
	*/
	virtual void UpdateToolsMenuButton();

	virtual void OnToolsButtonMenuActionTriggered(QAction* action);

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
		CPolygonParamsGuiItemDelegate(QObject* parent): QItemDelegate(parent)
		{

		}

		virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const
		{
			QLineEdit* editorPtr = new QLineEdit(parent);
			editorPtr->setValidator(new QDoubleValidator(editorPtr));
			return editorPtr;
		}
	}; // CPolygonParamsGuiItemDelegate
};


// public methods

// reimplemented (imod::IModelEditor)

template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::UpdateModel() const
{
	Q_ASSERT(BaseClass::IsGuiCreated());

	i2d::CPolygon* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	PolygonBasedModel editorObject;
	if (GetObjectFromEditor(editorObject)){
		if (!objectPtr->IsEqual(editorObject)){
			istd::CChangeNotifier changePtr(objectPtr);

			objectPtr->CopyFrom(editorObject);
		}
	}
}


// protected methods
template <class PolygonBasedShape, class PolygonBasedModel>
bool TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::GetObjectFromEditor(PolygonBasedModel& object) const
{
	object.Clear();

	int count = NodeParamsTable->rowCount();
	for (int i = 0; i < count; i++){
		double x = NodeParamsTable->item(i, CI_X)->text().toDouble();
		double y = NodeParamsTable->item(i, CI_Y)->text().toDouble();

		object.InsertNode(i2d::CVector2d(x, y));
	}

	return true;
}


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
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(BaseClass::IsGuiCreated());

	i2d::CPolygon* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		NodeParamsTable->clearContents();

		int count = objectPtr->GetNodesCount();
		NodeParamsTable->setRowCount(count);

		for (int i = 0; i < count; i++){
			i2d::CVector2d coord = objectPtr->GetNode(i);
			NodeParamsTable->setItem(i, 0, new QTableWidgetItem(QString::number(coord.GetX(), 'g', 12)));
			NodeParamsTable->setItem(i, 1, new QTableWidgetItem(QString::number(coord.GetY(), 'g', 12)));
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

#if QT_VERSION < 0x050000
	NodeParamsTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
	NodeParamsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif

	CPolygonParamsGuiItemDelegate* columnDelegate = new CPolygonParamsGuiItemDelegate(NodeParamsTable);
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

			BaseClass::connect(BaseClass::ToolsButton->menu(), SIGNAL(triggered(QAction*)), this, SLOT(OnToolsButtonMenuActionTriggered(QAction*)));
		}

		QMenu& menu = *BaseClass::ToolsButton->menu();
		menu.clear();

		menu.addAction(QIcon(":/Icons/FlipHorizontal"), QCoreApplication::translate("iqt2d", "Flip horizontally"))->setData(iview::IInteractiveShape::ActionFlipHorizontally);
		menu.addAction(QIcon(":/Icons/FlipVertical"), QCoreApplication::translate("iqt2d", "Flip vertically"))->setData(iview::IInteractiveShape::ActionFlipVertically);
		menu.addAction(QIcon(":/Icons/RotateRight"), QCoreApplication::translate("iqt2d", "Rotate clockwise"))->setData(iview::IInteractiveShape::ActionRotateClockwise);
		menu.addAction(QIcon(":/Icons/RotateLeft"), QCoreApplication::translate("iqt2d", "Rotate counterclockwise"))->setData(iview::IInteractiveShape::ActionRotateCounterclockwise);
		if (polylinePtr != NULL){
			menu.addAction(QIcon(":/Icons/Reverse"), QCoreApplication::translate("iqt2d", "Reverse line"))->setData(iview::IInteractiveShape::ActionReverseLine);
		}
	}
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnToolsButtonMenuActionTriggered(QAction* action)
{
	int actionId = action->data().toInt();

	const typename BaseClass::ShapesMap& shapesMap = BaseClass::GetShapesMap();
	for (		typename BaseClass::ShapesMap::const_iterator iter = shapesMap.begin();
				iter != shapesMap.end();
				++iter){
		const typename BaseClass::Shapes& shapes = iter.value();
		int shapesCount = shapes.GetCount();
		for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
			PolygonBasedShape* shapePtr = dynamic_cast<PolygonBasedShape*>(shapes.GetAt(shapeIndex));
			if (shapePtr != NULL){
				shapePtr->ExecuteAction((iview::IInteractiveShape::ShapeAction)actionId);
			}
		}
	}
}


} // namespace iqt2d


#endif // !iqt2d_TPolygonBasedParamsGuiComp_included


