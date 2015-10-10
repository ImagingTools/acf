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
#else
#include <QtGui/QItemDelegate>
#include <QtGui/QLineEdit>
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
class TPolygonBasedParamsGuiComp: 
			public iqt2d::TShapeParamsGuiCompBase<
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
		I_ASSIGN(m_nodeListSizeAttrPtr, "NodeListSize", "Fixed height of the node list if set (0 - hide)", false, 0);
	I_END_COMPONENT;

	TPolygonBasedParamsGuiComp();

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
	virtual void OnGuiShown();
	virtual void OnGuiHidden();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (iqt2d::TShapeParamsGuiCompBase)
	virtual bool PopulateActions(CActionAdapter& host, imod::IModel* modelPtr);
	virtual void OnActionTriggered(QAction* actionPtr);

protected:
	using BaseClass::GetObservedObject;
	using BaseClass::GetObservedModel;
	using BaseClass::DoUpdateModel;
	using BaseClass::CreateToolsMenu;
	using BaseClass::NodeParamsTable;
	using BaseClass::tr;
	using BaseClass::GetQtWidget;
	using BaseClass::RemoveButton;
	using BaseClass::InsertButton;
    using BaseClass::ToolsButton;

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

protected:
	I_ATTR(int, m_nodeListSizeAttrPtr);

	// actions
	QAction m_flipHorizontalAction;
	QAction m_flipVerticalAction;
	QAction m_rotateCwAction;
	QAction m_rotateCcwAction;
	QAction m_reversePolarityAction;
};


// public methods

template <class PolygonBasedShape, class PolygonBasedModel>
TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::TPolygonBasedParamsGuiComp()
:	m_flipHorizontalAction(QIcon(":/Icons/FlipHorizontal"), QCoreApplication::translate("iqt2d", "Flip horizontally"), this),
	m_flipVerticalAction(QIcon(":/Icons/FlipVertical"), QCoreApplication::translate("iqt2d", "Flip vertically"), this),
	m_rotateCwAction(QIcon(":/Icons/RotateRight"), QCoreApplication::translate("iqt2d", "Rotate clockwise"), this),
	m_rotateCcwAction(QIcon(":/Icons/RotateLeft"), QCoreApplication::translate("iqt2d", "Rotate counterclockwise"), this),
	m_reversePolarityAction(QIcon(":/Icons/Reverse"), QCoreApplication::translate("iqt2d", "Reverse line"), this)
{
}


// reimplemented (imod::IModelEditor)

template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::UpdateModel() const
{
	Q_ASSERT(BaseClass::IsGuiCreated());

	i2d::CPolygon* objectPtr = GetObservedObject();
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
	PolygonBasedModel* objectPtr = dynamic_cast<PolygonBasedModel*>(GetObservedObject());
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
	PolygonBasedModel* objectPtr = dynamic_cast<PolygonBasedModel*>(GetObservedObject());
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
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnGuiModelDetached()
{
	NodeParamsTable->disconnect();

	BaseClass::OnGuiModelDetached();
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnGuiShown()
{
	BaseClass::OnGuiShown();
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnGuiHidden()
{
	BaseClass::OnGuiHidden();
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(BaseClass::IsGuiCreated());

	i2d::CPolygon* objectPtr = GetObservedObject();
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

	bool isNodeTable = true;
	if (m_nodeListSizeAttrPtr.IsValid()){
		if (*m_nodeListSizeAttrPtr > 0){
			NodeParamsTable->setFixedHeight(qMax(40, *m_nodeListSizeAttrPtr));	// 40 is minimum visible size for 1 row
		}
		else{
			InsertButton->hide();
			RemoveButton->hide();
			NodeParamsTable->hide();
			isNodeTable = false;
		}

		GetQtWidget()->adjustSize();
	}

	if (isNodeTable){
#if QT_VERSION < 0x050000
		NodeParamsTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
		NodeParamsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif

		CPolygonParamsGuiItemDelegate* columnDelegate = new CPolygonParamsGuiItemDelegate(NodeParamsTable);
		NodeParamsTable->setItemDelegateForColumn(0, columnDelegate);
		NodeParamsTable->setItemDelegateForColumn(1, columnDelegate);
	}

	// tools actions
	CreateToolsMenu(ToolsButton);
}


// reimplemented (iqt2d::TShapeParamsGuiCompBase)

template <class PolygonBasedShape, class PolygonBasedModel>
bool TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::PopulateActions(CActionAdapter& host, imod::IModel* modelPtr)
{
	if (!BaseClass::PopulateActions(host, modelPtr)){
		return false;
	}

	i2d::CPolyline* polylinePtr = dynamic_cast<i2d::CPolyline*>(modelPtr);
	i2d::CPolygon* polygonPtr = dynamic_cast<i2d::CPolygon*>(modelPtr);
	if (polygonPtr == NULL && polylinePtr == NULL){
		return false;
	}

	host.AddAction(m_flipHorizontalAction);
	host.AddAction(m_flipVerticalAction);
	host.AddAction(m_rotateCwAction);
	host.AddAction(m_rotateCcwAction);

	if (polylinePtr){
		host.AddAction(m_reversePolarityAction);
	}

	return true;
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnActionTriggered(QAction* actionPtr)
{
	Q_ASSERT(actionPtr);

	PolygonBasedModel* modelPtr = dynamic_cast<PolygonBasedModel*>(BaseClass::GetObservedModel());
	Q_ASSERT(modelPtr);

	if (actionPtr == NULL || modelPtr == NULL){
		return;
	}

	if (actionPtr == &m_flipHorizontalAction){
		modelPtr->FlipByX();
	}
	else if (actionPtr == &m_flipVerticalAction){
		modelPtr->FlipByY();
	}
	else if (actionPtr == &m_rotateCwAction){
		modelPtr->Rotate(M_PI/2);
	}
	else if (actionPtr == &m_rotateCcwAction){
		modelPtr->Rotate(-M_PI/2);
	}
	else if (actionPtr == &m_reversePolarityAction){
		modelPtr->ReverseNodes();
	}
}


} // namespace iqt2d


#endif // !iqt2d_TPolygonBasedParamsGuiComp_included


