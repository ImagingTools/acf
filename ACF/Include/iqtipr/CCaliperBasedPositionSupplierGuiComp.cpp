#include "iqtipr/CCaliperBasedPositionSupplierGuiComp.h"


// Qt includes
#include <QMessageBox>

#include "iqt2d/CPosition2dShape.h"


namespace iqtipr
{


// reimplemented (iqt2d::ISceneExtender)

void CCaliperBasedPositionSupplierGuiComp::AddItemsToScene(iqt2d::ISceneProvider* providerPtr, int flags)
{
	BaseClass::AddItemsToScene(providerPtr, flags);

	if (m_paramsSetExtenderCompPtr.IsValid()){
		m_paramsSetExtenderCompPtr->AddItemsToScene(providerPtr, flags);
	}
}


void CCaliperBasedPositionSupplierGuiComp::RemoveItemsFromScene(iqt2d::ISceneProvider* providerPtr)
{
	if (m_paramsSetExtenderCompPtr.IsValid()){
		m_paramsSetExtenderCompPtr->RemoveItemsFromScene(providerPtr);
	}

	BaseClass::RemoveItemsFromScene(providerPtr);
}


// reimplemented (imod::IModelEditor)

void CCaliperBasedPositionSupplierGuiComp::UpdateModel() const
{
}


void CCaliperBasedPositionSupplierGuiComp::UpdateEditor()
{
	iipr::CCaliperBasedPositionSupplierComp* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		I_DWORD objectId;
		if (		(supplierPtr != NULL) &&
					m_idManagerCompPtr.IsValid() &&
					m_idManagerCompPtr->GetCurrentId(objectId)){
			const i2d::CVector2d* positionPtr = supplierPtr->GetVector2d(objectId);
			if (positionPtr != NULL){
				m_foundPosition.SetCenter(*positionPtr);

				if (IsGuiCreated()){
					PositionLabel->setText(tr("(%1, %2)").arg(positionPtr->GetX()).arg(positionPtr->GetY()));
				}
			}
			else{
				m_foundPosition.SetCenter(i2d::CVector2d(0, 0));

				if (IsGuiCreated()){
					PositionLabel->setText("");
				}
			}
		}
	}
}


// protected slots

void CCaliperBasedPositionSupplierGuiComp::on_TestButton_clicked()
{
	DoTest();
}


void CCaliperBasedPositionSupplierGuiComp::on_LoadParamsButton_clicked()
{
	iipr::CCaliperBasedPositionSupplierComp* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		iprm::IParamsSet* paramsPtr = supplierPtr->GetParamsSet();
		if (		(paramsPtr != NULL) &&
					(m_paramsLoaderCompPtr->LoadFromFile(*paramsPtr, "") == iser::IFileLoader::StateFailed)){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Cannot load parameters"));
		}
	}
}


void CCaliperBasedPositionSupplierGuiComp::on_SaveParamsButton_clicked()
{
	iipr::CCaliperBasedPositionSupplierComp* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		iprm::IParamsSet* paramsPtr = supplierPtr->GetParamsSet();
		if (		(paramsPtr != NULL) &&
					(m_paramsLoaderCompPtr->SaveToFile(*paramsPtr, "") == iser::IFileLoader::StateFailed)){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Cannot save parameters"));
		}
	}
}


// protected methods

bool CCaliperBasedPositionSupplierGuiComp::DoTest()
{
	iipr::CCaliperBasedPositionSupplierComp* supplierPtr = GetObjectPtr();
	I_DWORD objectId;
	if (		(supplierPtr != NULL) &&
				m_idManagerCompPtr.IsValid() &&
				m_idManagerCompPtr->GetCurrentId(objectId)){
		supplierPtr->BeginNextObject(objectId);
		supplierPtr->EnsureWorkFinished(objectId);

		return supplierPtr->GetWorkStatus(objectId) < iproc::ISupplier::WS_ERROR;
	}

	return false;
}


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CCaliperBasedPositionSupplierGuiComp::CreateShapes(int /*sceneId*/, bool /*inactiveOnly*/, Shapes& result)
{
	iqt2d::CPosition2dShape* shapePtr = new iqt2d::CPosition2dShape;
	if (shapePtr != NULL){
		m_foundPosition.AttachObserver(shapePtr);

		result.PushBack(shapePtr);
	}
}


// reimplemented (iqt::TGuiObserverWrap)

void CCaliperBasedPositionSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iipr::CCaliperBasedPositionSupplierComp* supplierPtr = GetObjectPtr();
	I_ASSERT(supplierPtr != NULL);	// model must be attached

	iprm::IParamsSet* paramsPtr = const_cast<iprm::IParamsSet*>(supplierPtr->GetParamsSet());
	imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(paramsPtr);

	bool areParamsEditable = false;
	if ((paramsModelPtr != NULL) && m_paramsSetGuiCompPtr.IsValid() && m_paramsSetObserverCompPtr.IsValid()){
		paramsModelPtr->AttachObserver(m_paramsSetObserverCompPtr.GetPtr());
		m_paramsSetGuiCompPtr->CreateGui(ParamsFrame);

		areParamsEditable = true;
	}

	ParamsGB->setVisible(
				(paramsPtr != NULL) &&
				(areParamsEditable || m_paramsLoaderCompPtr.IsValid()));
	LoadParamsButton->setVisible(m_paramsLoaderCompPtr.IsValid());
	SaveParamsButton->setVisible(m_paramsLoaderCompPtr.IsValid());
	ParamsFrame->setVisible(areParamsEditable);
}


void CCaliperBasedPositionSupplierGuiComp::OnGuiModelDetached()
{
	iipr::CCaliperBasedPositionSupplierComp* supplierPtr = GetObjectPtr();
	I_ASSERT(supplierPtr != NULL);	// model must be attached

	iprm::IParamsSet* paramsPtr = const_cast<iprm::IParamsSet*>(supplierPtr->GetParamsSet());
	imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(paramsPtr);

	if (		m_paramsSetObserverCompPtr.IsValid() &&
				(paramsModelPtr != NULL) &&
				paramsModelPtr->IsAttached(m_paramsSetObserverCompPtr.GetPtr())){
		paramsModelPtr->DetachObserver(m_paramsSetObserverCompPtr.GetPtr());
	}

	if (m_paramsSetGuiCompPtr.IsValid() && m_paramsSetGuiCompPtr->IsGuiCreated()){
		m_paramsSetGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiModelDetached();
}


} // namespace iqtipr


