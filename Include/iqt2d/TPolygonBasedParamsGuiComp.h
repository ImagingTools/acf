#ifndef iqt2d_TPolygonBasedParamsGuiComp_included
#define iqt2d_TPolygonBasedParamsGuiComp_included


// Qt includes
#include <QtCore/QMimeData>
#include <QtCore/QByteArray>
#include <QtCore/QAbstractTableModel>
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
	class TableModel: public QAbstractTableModel
	{
	public:
		typedef QAbstractTableModel BaseClass;

		TableModel(TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>* parentPtr);

		// reimplemented (QAbstractTableModel)
		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
		virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
		virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		virtual Qt::ItemFlags flags(const QModelIndex &index) const;

	private:
		TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>* m_parentPtr;
	};

	/**
		Get the table with the node data.
	*/
	QTableView* GetNodeTable();

	virtual void OnInsertNode();
	virtual void OnRemoveNode();
	virtual void OnCopyData();
	virtual void OnPasteData();

	// delegated from QAbstractTableModel
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiRetranslate();

	// reimplemented (iqt2d::TShapeParamsGuiCompBase)
	virtual bool PopulateActions(CActionAdapter& host, imod::IModel* modelPtr);
	virtual void OnActionTriggered(QAction* actionPtr);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

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
/*	class CPolygonParamsGuiItemDelegate: public QItemDelegate
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
*/
protected:
	I_ATTR(int, m_nodeListSizeAttrPtr);

	// actions
	QAction m_flipHorizontalAction;
	QAction m_flipVerticalAction;
	QAction m_rotateCwAction;
	QAction m_rotateCcwAction;
	QAction m_reversePolarityAction;

private:
	TableModel m_tableModel;
};


// public methods

template <class PolygonBasedShape, class PolygonBasedModel>
TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::TPolygonBasedParamsGuiComp()
:	m_flipHorizontalAction(QIcon(":/Icons/FlipHorizontal"), "", this),
	m_flipVerticalAction(QIcon(":/Icons/FlipVertical"), "", this),
	m_rotateCwAction(QIcon(":/Icons/RotateRight"), "", this),
	m_rotateCcwAction(QIcon(":/Icons/RotateLeft"), "", this),
	m_reversePolarityAction(QIcon(":/Icons/Reverse"), "", this),
	m_tableModel(this)
{
}


// reimplemented (imod::IModelEditor)

template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::UpdateModel() const
{
	Q_ASSERT(BaseClass::IsGuiCreated());
}


// protected methods

template <class PolygonBasedShape, class PolygonBasedModel>
QTableView* TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::GetNodeTable()
{
	return NodeParamsTable;
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnInsertNode()
{
	i2d::CPolygon* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		QItemSelectionModel *select = NodeParamsTable->selectionModel();
		QModelIndexList selected = select->selectedRows();

		int row = selected.isEmpty()? -1: selected.front().row();
		if (row >= 0){
			objectPtr->InsertNode(row, i2d::CVector2d::GetZero());
		}
		else{
			objectPtr->InsertNode(i2d::CVector2d::GetZero());
		}
	}
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnCopyData()
{
	PolygonBasedModel* objectPtr = GetObservedObject();
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
	PolygonBasedModel* objectPtr = GetObservedObject();
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
	i2d::CPolygon* objectPtr = GetObservedObject();
	if (objectPtr != NULL){

		QItemSelectionModel *select = NodeParamsTable->selectionModel();
		QModelIndexList selected = select->selectedRows();

		int row = selected.isEmpty()? -1: selected.front().row();
		if (row >= 0){
			objectPtr->RemoveNode(row);
		}
	}
}


// delegated from QAbstractTableModel

template <class PolygonBasedShape, class PolygonBasedModel>
int TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::rowCount(const QModelIndex& /*parent*/) const
{
	int retVal = 0;
	i2d::CPolygon* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		retVal = objectPtr->GetNodesCount();
	}

	return retVal;
}


template <class PolygonBasedShape, class PolygonBasedModel>
int TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::columnCount(const QModelIndex& /*parent*/) const
{
	return 2;
}


template <class PolygonBasedShape, class PolygonBasedModel>
QVariant TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::data(const QModelIndex& index, int role) const
{
	if ((role == Qt::DisplayRole) || (role == Qt::EditRole)){
		i2d::CVector2d retVal(0, 0);
		i2d::CPolygon* objectPtr = GetObservedObject();
		if ((objectPtr != NULL) && (index.column() <= 1) && (index.column() >= 0)){
			retVal = objectPtr->GetNodePos(index.row());
			return retVal[index.column()];
		}		
	}

	return QVariant();
}


template <class PolygonBasedShape, class PolygonBasedModel>
bool TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (role == Qt::EditRole){
		i2d::CPolygon* objectPtr = GetObservedObject();
		if ((objectPtr != NULL) && (index.column() <= 1) && (index.column() >= 0)){
			i2d::CVector2d pos = objectPtr->GetNodePos(index.row());
			bool isOk = false;
			pos[index.column()] = value.toDouble(&isOk);
			if (isOk){
				objectPtr->SetNodePos(index.row(), pos);

				return true;
			}
		}
	}

	return false;
}


template <class PolygonBasedShape, class PolygonBasedModel>
QVariant TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal){
		if (role == Qt::DisplayRole){
			switch (section){
			case 0:
				return "X";

			case 1:
				return "Y";
			}
		}
	}

	return QVariant();
}


// reimplemented (iqtgui::CGuiComponentBase)

template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	NodeParamsTable->setModel(&m_tableModel);

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

//		CPolygonParamsGuiItemDelegate* columnDelegate = new CPolygonParamsGuiItemDelegate(NodeParamsTable);
//		NodeParamsTable->setItemDelegateForColumn(0, columnDelegate);
//		NodeParamsTable->setItemDelegateForColumn(1, columnDelegate);
	}

	// tools actions
	CreateToolsMenu(ToolsButton);
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	m_flipHorizontalAction.setText(QCoreApplication::translate("iqt2d", "Flip horizontally"));
	m_flipVerticalAction.setText(QCoreApplication::translate("iqt2d", "Flip vertically"));
	m_rotateCwAction.setText(QCoreApplication::translate("iqt2d", "Rotate clockwise"));
	m_rotateCcwAction.setText(QCoreApplication::translate("iqt2d", "Rotate counterclockwise"));
	m_reversePolarityAction.setText(QCoreApplication::translate("iqt2d", "Reverse line"));
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


// reimplemented (iqtgui::TGuiObserverWrap)

template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
{
	QModelIndex leftTop = m_tableModel.index(0, 0);
	QModelIndex rightBottom = m_tableModel.index(rowCount() - 1, columnCount() - 1);

	Q_EMIT m_tableModel.dataChanged(leftTop, rightBottom);
	Q_EMIT m_tableModel.layoutChanged();

	BaseClass::UpdateGui(changeSet);
}


// public methods of embedded class TableModel

template <class PolygonBasedShape, class PolygonBasedModel>
TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::TableModel::TableModel(TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>* parentPtr)
:	m_parentPtr(parentPtr)
{
}


// reimplemented (QAbstractTableModel)

template <class PolygonBasedShape, class PolygonBasedModel>
int TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::TableModel::rowCount(const QModelIndex& parent) const
{
	if (m_parentPtr != NULL){
		return m_parentPtr->rowCount(parent);
	}

	return 0;
}


template <class PolygonBasedShape, class PolygonBasedModel>
int TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::TableModel::columnCount(const QModelIndex& parent) const
{
	if (m_parentPtr != NULL){
		return m_parentPtr->columnCount(parent);
	}

	return 0;
}


template <class PolygonBasedShape, class PolygonBasedModel>
QVariant TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::TableModel::data(const QModelIndex& index, int role) const
{
	if (m_parentPtr != NULL){
		return m_parentPtr->data(index, role);
	}

	return QVariant();
}


template <class PolygonBasedShape, class PolygonBasedModel>
bool TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::TableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (m_parentPtr != NULL){
		return m_parentPtr->setData(index, value, role);
	}

	return false;
}


template <class PolygonBasedShape, class PolygonBasedModel>
QVariant TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (m_parentPtr != NULL){
		return m_parentPtr->headerData(section, orientation, role);
	}

	return QVariant();
}


template <class PolygonBasedShape, class PolygonBasedModel>
Qt::ItemFlags TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::TableModel::flags(const QModelIndex& /*index*/) const
{
	return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}


} // namespace iqt2d


#endif // !iqt2d_TPolygonBasedParamsGuiComp_included


