#include "iqtipr/CValueSupplierGuiComp.h"


// Qt includes
#include <QMessageBox>

#include "iqt2d/CPosition2dShape.h"


namespace iqtipr
{


// reimplemented (imod::IModelEditor)

void CValueSupplierGuiComp::UpdateModel() const
{
}


void CValueSupplierGuiComp::UpdateEditor()
{
	iipr::IValueSupplier* supplierPtr = GetObjectPtr();
	iproc::IIdManager* idManagerPtr = GetIdManager();
	I_DWORD objectId;
	if (		(supplierPtr != NULL) &&
				(idManagerPtr != NULL) &&
				idManagerPtr->GetCurrentId(objectId)){
		imath::CVarVector position = supplierPtr->GetValue(objectId);
		if (position.GetElementsCount() >= 2){
			m_foundPosition.SetCenter(i2d::CVector2d(position[0], position[1]));

			if (IsGuiCreated()){
				PositionLabel->setText(tr("(%1, %2)").arg(position[0]).arg(position[1]));
			}
		}
		else{
			m_foundPosition.SetCenter(i2d::CVector2d(0, 0));

			if (IsGuiCreated()){
				PositionLabel->setText("No position");
			}
		}
	}
}


// protected slots

void CValueSupplierGuiComp::on_TestButton_clicked()
{
	DoTest();
}


void CValueSupplierGuiComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CValueSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


// protected methods

// reimplemented (iqtproc::TSupplierGuiCompBase)

QWidget* CValueSupplierGuiComp::GetParamsWidget() const
{
	I_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CValueSupplierGuiComp::CreateShapes(int /*sceneId*/, bool /*inactiveOnly*/, Shapes& result)
{
	iqt2d::CPosition2dShape* shapePtr = new iqt2d::CPosition2dShape;
	if (shapePtr != NULL){
		shapePtr->setZValue(2);

		m_foundPosition.AttachObserver(shapePtr);

		result.PushBack(shapePtr);
	}
}


// reimplemented (iqt::TGuiObserverWrap)

void CValueSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());
	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


} // namespace iqtipr


