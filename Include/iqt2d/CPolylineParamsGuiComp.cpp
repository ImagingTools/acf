#include "iqt2d/CPolylineParamsGuiComp.h"


namespace iqt2d
{


CPolylineParamsGuiComp::CPolylineParamsGuiComp()
:	m_openCloseAction(QIcon(":/Icons/OpenCloseLine"), "", this)
{
}


// reimplemented (imod::IModelEditor)

void CPolylineParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(BaseClass::IsGuiCreated());
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


// reimplemented (iqt2d::TShapeParamsGuiCompBase)

bool CPolylineParamsGuiComp::PopulateActions(CActionAdapter& host, imod::IModel* modelPtr)
{
	if (BaseClass::PopulateActions(host, modelPtr)){
		host.AddAction(m_openCloseAction);

		return true;
	}

	return false;
}


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


iview::CInteractiveShapeBase* CPolylineParamsGuiComp::CreateShapeInstance() const
{
	iview::CPolylineShape* shapePtr = new iview::CPolylineShape();

	if (*m_showOrientationAttrPtr){
		shapePtr->SetOrientationVisible(true);
	}

	return shapePtr;
}


// reimplemented (iqtgui::CGuiComponentBase)

void CPolylineParamsGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	m_openCloseAction.setText(tr("Open/close line"));
}


} // namespace iqt2d
