#ifndef iqt2d_TPolygonBasedParamsGuiComp_included
#define iqt2d_TPolygonBasedParamsGuiComp_included


// Qt includes
#include <QtGui/QItemDelegate>
#include <QtGui/QLineEdit>
#include <QtGui/QDoubleValidator>
#include <QtGui/QClipboard>
#include <QtCore/QMimeData>
#include <QtCore/QByteArray>

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

protected:
	using BaseClass::GetObjectPtr;
	using BaseClass::DoUpdateModel;
	using BaseClass::NodeParamsTable;

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
	} ; // CPolygonParamsGuiItemDelegate

private:
	/** Change state of a line close check box: disabled and hidden, according 
	 to the attached model type (only visible for CPolyline) and list selection 
	 (uncheckable if a list element is selected, always checkable). */
	void updateClosedLineCheckBox(bool forceEnabled, bool forceHidden);

} ; // TPolygonBasedParamsGuiComp

// public methods

// reimplemented (imod::IModelEditor)

template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	i2d::CPolygon* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

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
		dataPtr->setData(typeid(PolygonBasedModel).name(), data);

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
		if (dataPtr->hasFormat(typeid(PolygonBasedModel).name())){
			QByteArray data(dataPtr->data(typeid(PolygonBasedModel).name()));

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

	updateClosedLineCheckBox(false, false);
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::OnGuiModelDetached()
{
	updateClosedLineCheckBox(false, true);

	NodeParamsTable->disconnect();

	BaseClass::OnGuiModelDetached();
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

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

	updateClosedLineCheckBox(false, false);
}


template <class PolygonBasedShape, class PolygonBasedModel>
void TPolygonBasedParamsGuiComp<PolygonBasedShape, PolygonBasedModel>::updateClosedLineCheckBox(bool forceEnabled, bool forceHidden)
{
	CloseLineCheckBox->setHidden(true);
	CloseLineCheckBox->setDisabled(true);

	i2d::CPolyline* polylinePtr = dynamic_cast<i2d::CPolyline*>(GetModelPtr());
	if (!forceHidden && polylinePtr != NULL){
		CloseLineCheckBox->setChecked(polylinePtr->IsClosed());
		CloseLineCheckBox->setHidden(false);
		CloseLineCheckBox->setEnabled(forceEnabled || !polylinePtr->IsClosed());
	}
}


} // namespace iqt2d


#endif // !iqt2d_TPolygonBasedParamsGuiComp_included


