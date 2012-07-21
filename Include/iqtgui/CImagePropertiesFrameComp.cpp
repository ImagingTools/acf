#include "iqtgui/CImagePropertiesFrameComp.h"


// ACF includes
#include "i2d/ICalibrationProvider.h"
#include "i2d/CAffine2d.h"


namespace iqtgui
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CImagePropertiesFrameComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	iimg::IBitmap* bitmapPtr = GetObjectPtr();
	if (bitmapPtr != NULL && IsGuiCreated()){
		SizeLabel->setText(QString("%1x%2").arg(bitmapPtr->GetImageSize().GetX()).arg(bitmapPtr->GetImageSize().GetY()));
		DepthLabel->setText(QString("%1 bits").arg(bitmapPtr->GetComponentBitsCount() * bitmapPtr->GetComponentsCount()));

		const i2d::ITransformation2d* calibrationPtr = NULL;
		i2d::ICalibrationProvider* calibrationProviderPtr = dynamic_cast<i2d::ICalibrationProvider*>(bitmapPtr);
		if (calibrationProviderPtr != NULL){
			calibrationPtr = calibrationProviderPtr->GetCalibration();
		}

		if (calibrationPtr != NULL){
			i2d::CAffine2d transform;
			
			if (calibrationPtr->GetLocalInvTransform(i2d::CVector2d(0,0), transform)){
				double scale = transform.GetDeformMatrix().GetApproxScale();
				ResolutionLabel->setText(QString(tr("%1 px/mm")).arg(scale, 1, 'f', 1));
			}
		}
		else{
			ResolutionLabel->setVisible(false);
		}
	}
}


void CImagePropertiesFrameComp::OnGuiModelDetached()
{
	SizeLabel->clear();
	DepthLabel->clear();
	ResolutionLabel->clear();

	BaseClass::OnGuiModelDetached();
}

} // namespace iqtgui


