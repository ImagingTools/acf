#include "iqtcam/CSnapBitmapSupplierGuiComp.h"


// Qt includes
#include <QMessageBox>

#include "iqt2d/CImageShape.h"


namespace iqtcam
{


// reimplemented (imod::IModelEditor)

void CSnapBitmapSupplierGuiComp::UpdateModel() const
{
}


void CSnapBitmapSupplierGuiComp::UpdateEditor()
{
	icam::CSnapBitmapSupplierComp* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		const iimg::IBitmap* bitmapPtr = GetCurrentBitmap();
		imod::IModel* bitmapModelPtr = dynamic_cast<imod::IModel*>(const_cast<iimg::IBitmap*>(bitmapPtr));

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

		if (IsGuiCreated()){
			istd::CIndex2d bitmapSize(0, 0);
			if (bitmapPtr != NULL){
				bitmapSize = bitmapPtr->GetImageSize();
			}

			SizeLabel->setText(tr("(%1 x %2)").arg(bitmapSize.GetX()).arg(bitmapSize.GetY()));
		}
	}

	if (IsGuiCreated()){
		const iimg::IBitmap* bitmapPtr = GetCurrentBitmap();
		bool hasBitmap = (bitmapPtr != NULL);

		SaveImageButton->setEnabled(hasBitmap);
	}
}


// protected slots

void CSnapBitmapSupplierGuiComp::on_SnapImageButton_clicked()
{
	iproc::ISupplier* supplierPtr = GetObjectPtr();
	iproc::IIdManager* idManagerPtr = GetIdManager();
	I_DWORD objectId;
	if (		(supplierPtr != NULL) &&
				(idManagerPtr != NULL) &&
				idManagerPtr->SkipToNextId(objectId)){
		supplierPtr->BeginNextObject(objectId);
		supplierPtr->EnsureWorkFinished(objectId);

		if (supplierPtr->GetWorkStatus(objectId) >= iproc::ISupplier::WS_ERROR){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Snap Error"));
		}
	}
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
	LoadParams();
}


void CSnapBitmapSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


// protected methods

const iimg::IBitmap* CSnapBitmapSupplierGuiComp::GetCurrentBitmap() const
{
	icam::CSnapBitmapSupplierComp* supplierPtr = GetObjectPtr();
	iproc::IIdManager* idManagerPtr = GetIdManager();
	I_DWORD objectId;
	if (		(supplierPtr != NULL) &&
				(idManagerPtr != NULL) &&
				idManagerPtr->GetCurrentId(objectId)){
		return supplierPtr->GetBitmap(objectId);
	}

	return NULL;
}


// reimplemented (iqt::CGuiComponentBase)

void CSnapBitmapSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	SaveImageButton->setVisible(m_bitmapLoaderCompPtr.IsValid());
}


// reimplemented (iqtproc::TSupplierGuiCompBase)

QWidget* CSnapBitmapSupplierGuiComp::GetParamsWidget() const
{
	I_ASSERT(IsGuiCreated());

	return ParamsFrame;
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

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());

	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


} // namespace iqtcam


