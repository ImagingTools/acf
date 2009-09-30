#include "iqtmm/CPlaybackControllerGuiComp.h"


// Qt includes
#include <QCoreApplication>
#include <QEventLoop>
#include <QUrl>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"




namespace iqtmm
{


// public methods
	
// reimplemented (imod::IModelEditor)
	
void CPlaybackControllerGuiComp::UpdateEditor(int updateFlags)
{
	imm::IVideoController* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		if ((updateFlags & imm::IMediaController::CF_STATUS) != 0){
			iqt::CSignalBlocker block(this, true);

			bool isPlaying = objectPtr->IsPlaying();
			PlayButton->setChecked(isPlaying);

			int framesCount = objectPtr->GetFramesCount();
			if (framesCount > 1){
				PositionSlider->setMaximum(framesCount);
				PositionSlider->setEnabled(true);
			}
			else{
				PositionSlider->setValue(0);
				PositionSlider->setEnabled(false);
			}
		}

		if ((updateFlags & imm::IMediaController::CF_POSITION) != 0){
			iqt::CSignalBlocker block(this, true);

			PositionSlider->setValue(objectPtr->GetCurrentFrame());
		}
	}
}


void CPlaybackControllerGuiComp::UpdateModel() const
{
}


// private slots

void CPlaybackControllerGuiComp::on_PlayButton_toggled(bool isToggled)
{
	imm::IMediaController* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		UpdateBlocker block(this);

		objectPtr->SetPlaying(isToggled);
	}
}


void CPlaybackControllerGuiComp::on_PositionSlider_valueChanged(int position)
{
	imm::IVideoController* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		UpdateBlocker block(this);

		objectPtr->SetCurrentFrame(position);
	}
}



} // namespace iqtmm


