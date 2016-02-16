#include "iqt2d/CPolylineParamsGuiComp.h"


namespace iqt2d
{


CPolylineParamsGuiComp::CPolylineParamsGuiComp()
:	m_openCloseAction(QIcon(":/Icons/OpenCloseLine"), "", this)
{
}


// reimplemented (iview::IShapeFactory)

iview::IShape* CPolylineParamsGuiComp::CreateShape(const istd::IChangeable* objectPtr, bool connectToModel) const
{
	iview::CPolylineShape* shapePtr = new iview::CPolylineShape();

	if (*m_showOrientationAttrPtr){
		shapePtr->SetOrientationVisible(true);
	}

	if (connectToModel){
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(const_cast<istd::IChangeable*>(objectPtr));
		if (modelPtr != NULL){
			if (modelPtr->AttachObserver(shapePtr)){
				shapePtr->SetVisible(true);
			}
		}
	}

	return shapePtr;
}


// reimplemented (imod::IModelEditor)

void CPolylineParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(BaseClass::IsGuiCreated());

	i2d::CPolyline* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	bool isClosed = objectPtr->IsClosed();

	i2d::CPolyline editorObject;
	if (GetObjectFromEditor(editorObject)){
	
		editorObject.SetClosed(isClosed);

		if (!objectPtr->IsEqual(editorObject)){
			istd::CChangeNotifier changePtr(objectPtr);

			objectPtr->CopyFrom(editorObject);
		}
	}
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


// reimplemented from TPolygonBasedParamsGuiComp

bool CPolylineParamsGuiComp::PopulateActions(CActionAdapter& host, imod::IModel* modelPtr)
{
	if (BaseClass::PopulateActions(host, modelPtr)){
		host.AddAction(m_openCloseAction);

		return true;
	}

	return false;
}


// reimplemented (iqtgui::CGuiComponentBase)

void CPolylineParamsGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	m_openCloseAction.setText(QCoreApplication::translate("iqt2d", "Open/close line"));
}


// pseudo slots

void CPolylineParamsGuiComp::OnActionTriggered(QAction* actionPtr)
{
	if (actionPtr == &m_openCloseAction){
		i2d::CPolyline* polylinePtr = GetObjectPtr();
		if (polylinePtr){
			polylinePtr->SetClosed(!polylinePtr->IsClosed());
		}

		return;
	}

	BaseClass::OnActionTriggered(actionPtr);
}


} // namespace iqt2d
