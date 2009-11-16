#include "iqtmm/CPlaybackControllerGuiComp.h"


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
		int supportedFeatures = objectPtr->GetSupportedFeatures();
		bool isPlayable = ((supportedFeatures & imm::IMediaController::SF_PLAY) != 0);
		PlayButton->setVisible(isPlayable);
		RepeatButton->setVisible(isPlayable);

		PositionSlider->setEnabled((supportedFeatures & imm::IMediaController::SF_SEEK) != 0);

		if ((updateFlags & imm::IMediaController::CF_STATUS) != 0){
			iqt::CSignalBlocker block(this, true);

			bool isPlaying = objectPtr->IsPlaying();
			PlayButton->setChecked(isPlaying);

			int framesCount = objectPtr->GetFramesCount();
			if (framesCount > 1){
				PositionSlider->setMaximum(framesCount - 1);
				PositionSlider->setEnabled(true);
			}
			else{
				PositionSlider->setValue(0);
				PositionSlider->setEnabled(false);
			}
		}

		if ((updateFlags & imm::IMediaController::CF_MEDIA_POSITION) != 0){
			iqt::CSignalBlocker block(PositionSlider, true);

			int currentFrame = objectPtr->GetCurrentFrame();
			if (currentFrame >= 0){
				PositionSlider->setValue(currentFrame);
			}
		}
	}
}


void CPlaybackControllerGuiComp::UpdateModel() const
{
}


// reimplemented (icomp::IComponent)

void CPlaybackControllerGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	connect(&m_playTimer, SIGNAL(timeout()), this, SLOT(OnTimerTick()));
}


void CPlaybackControllerGuiComp::OnComponentDestroyed()
{
	disconnect(&m_playTimer, SIGNAL(timeout()), this, SLOT(OnTimerTick()));

	BaseClass::OnComponentDestroyed();
}


// private slots

void CPlaybackControllerGuiComp::on_PlayButton_toggled(bool isToggled)
{
	imm::IVideoController* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		UpdateBlocker block(this);

		// if the controller doesn't support the auto playing of the content,
		// we will create a timer and simulate playing manually:
		if ((objectPtr->GetSupportedFeatures() & imm::IVideoController::SF_AUTO_PLAY) == 0){
			int timerIntervall = objectPtr->GetFrameIntervall() * 1000;

			if (isToggled){
				m_playTimer.start(timerIntervall);
			}
			else{
				m_playTimer.stop();
			}
		}

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


// private slots

void CPlaybackControllerGuiComp::OnTimerTick()
{
	imm::IVideoController* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int framesCount = objectPtr->GetFramesCount();
		if (framesCount > 0){
			int currentFrameIndex = objectPtr->GetCurrentFrame();
			I_ASSERT(currentFrameIndex < framesCount);
			int nextCurrentFrameIndex = RepeatButton->isChecked() ? (currentFrameIndex + 1) % framesCount : currentFrameIndex + 1;
			if (nextCurrentFrameIndex < framesCount){
				objectPtr->SetCurrentFrame(nextCurrentFrameIndex);
			}
			else{
				objectPtr->SetPlaying(false);
				objectPtr->SetCurrentFrame(0);
			}
		}
	}
}


} // namespace iqtmm


