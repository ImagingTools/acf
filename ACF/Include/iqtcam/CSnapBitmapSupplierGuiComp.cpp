#include "iqtcam/CSnapBitmapSupplierGuiComp.h"


// Qt includes
#include <QMessageBox>

#include "istd/TDelPtr.h"

#include "iqt2d/CImageShape.h"


namespace iqtcam
{


// reimplemented (iqt2d::ISceneExtender)

void CSnapBitmapSupplierGuiComp::AddItemsToScene(iqt2d::ISceneProvider* providerPtr, int flags)
{
	BaseClass::AddItemsToScene(providerPtr, flags);

	if (m_paramsSetExtenderCompPtr.IsValid()){
		m_paramsSetExtenderCompPtr->AddItemsToScene(providerPtr, flags);
	}
}


void CSnapBitmapSupplierGuiComp::RemoveItemsFromScene(iqt2d::ISceneProvider* providerPtr)
{
	if (m_paramsSetExtenderCompPtr.IsValid()){
		m_paramsSetExtenderCompPtr->RemoveItemsFromScene(providerPtr);
	}

	BaseClass::RemoveItemsFromScene(providerPtr);
}


// reimplemented (imod::IModelEditor)

void CSnapBitmapSupplierGuiComp::UpdateModel() const
{
	icam::CSnapBitmapSupplierComp* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		imod::IModel* bitmapModelPtr = dynamic_cast<imod::IModel*>(const_cast<iimg::IBitmap*>(GetCurrentBitmap()));

		const ShapesMap& shapesMap = GetShapesMap();
		for (		ShapesMap::const_iterator iter = shapesMap.begin();
					iter != shapesMap.end();
					++iter){
			const Shapes& shapes = iter->second;
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				iqt2d::CImageShape* shapePtr = dynamic_cast<iqt2d::CImageShape*>(shapes.GetAt(shapeIndex));
				if (shapePtr != NULL){
					imod::IModel* prevAttachedModelPtr = shapePtr->GetModelPtr();
					if (prevAttachedModelPtr != bitmapModelPtr){
						if (prevAttachedModelPtr != NULL){
							prevAttachedModelPtr->DetachObserver(shapePtr);
						}

						if (bitmapModelPtr != NULL){
							bitmapModelPtr->AttachObserver(shapePtr);
						}
					}
				}
			}
		}
	}

	if (IsGuiCreated()){
		const iimg::IBitmap* bitmapPtr = GetCurrentBitmap();
		bool hasBitmap = (bitmapPtr != NULL);

		SaveImageButton->setVisible(hasBitmap && m_bitmapLoaderCompPtr.IsValid());
	}
}


void CSnapBitmapSupplierGuiComp::UpdateEditor()
{
}


// protected slots

void CSnapBitmapSupplierGuiComp::on_SnapImageButton_clicked()
{
	SnapImage();
}


void CSnapBitmapSupplierGuiComp::on_SaveImageButton_clicked()
{
	icam::CSnapBitmapSupplierComp* supplierPtr = GetObjectPtr();
	if ((supplierPtr != NULL) && m_bitmapLoaderCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = GetCurrentBitmap();
		if ((bitmapPtr != NULL) && (m_bitmapLoaderCompPtr->SaveToFile(*bitmapPtr, "") == iser::IFileLoader::StateFailed)){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Cannot save image"));
		}
	}
}


void CSnapBitmapSupplierGuiComp::on_LoadParamsButton_clicked()
{
	icam::CSnapBitmapSupplierComp* supplierPtr = GetObjectPtr();
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


void CSnapBitmapSupplierGuiComp::on_SaveParamsButton_clicked()
{
	icam::CSnapBitmapSupplierComp* supplierPtr = GetObjectPtr();
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

const iimg::IBitmap* CSnapBitmapSupplierGuiComp::GetCurrentBitmap() const
{
	icam::CSnapBitmapSupplierComp* supplierPtr = GetObjectPtr();
	I_DWORD objectId;
	if (		(supplierPtr != NULL) &&
				m_idManagerCompPtr.IsValid() &&
				m_idManagerCompPtr->GetCurrentId(objectId)){
		return supplierPtr->GetBitmap(objectId);
	}

	return NULL;
}


bool CSnapBitmapSupplierGuiComp::SnapImage()
{
	icam::CSnapBitmapSupplierComp* supplierPtr = GetObjectPtr();
	I_DWORD objectId;
	if (		(supplierPtr != NULL) &&
				m_bitmapLoaderCompPtr.IsValid() &&
				m_idManagerCompPtr.IsValid() &&
				m_idManagerCompPtr->SkipToNextId(objectId)){
		const iimg::IBitmap* bitmapPtr = supplierPtr->GetBitmap(objectId);
		if (bitmapPtr != NULL){
			supplierPtr->EnsureWorkFinished(objectId);

			return supplierPtr->GetWorkStatus(objectId) < iproc::ISupplier::WS_ERROR;
		}
	}

	return false;
}


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CSnapBitmapSupplierGuiComp::CreateShapes(int /*sceneId*/, bool /*inactiveOnly*/, Shapes& result)
{
	iqt2d::CImageShape* shapePtr = new iqt2d::CImageShape;
	if (shapePtr != NULL){
		result.PushBack(shapePtr);

		icam::CSnapBitmapSupplierComp* supplierPtr = GetObjectPtr();
		if (supplierPtr != NULL){
			imod::IModel* bitmapModelPtr = dynamic_cast<imod::IModel*>(const_cast<iimg::IBitmap*>(GetCurrentBitmap()));

			if (bitmapModelPtr != NULL){
				bitmapModelPtr->AttachObserver(shapePtr);
			}
		}
	}
}


// reimplemented (iqt::TGuiObserverWrap)

void CSnapBitmapSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	icam::CSnapBitmapSupplierComp* supplierPtr = GetObjectPtr();
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


void CSnapBitmapSupplierGuiComp::OnGuiModelDetached()
{
	icam::CSnapBitmapSupplierComp* supplierPtr = GetObjectPtr();
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


} // namespace iqtcam


