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
		PlayButton->setEnabled(isPlayable);
		TimeEdit->setEnabled(isPlayable);
		PositionSlider->setEnabled(isPlayable);
		FrameIndexSB->setEnabled(isPlayable);
		RepeatButton->setEnabled(isPlayable);

		bool isSeekEnabled = ((supportedFeatures & imm::IMediaController::SF_SEEK) != 0);

		if ((updateFlags & imm::IMediaController::CF_STATUS) != 0){
			iqt::CSignalBlocker block(this, true);

			bool isPlaying = objectPtr->IsPlaying();
			PlayButton->setChecked(isPlaying);

			int framesCount = objectPtr->GetFramesCount();
			if (framesCount > 1){
				PositionSlider->setMaximum(framesCount - 1);
				double mediumLength = objectPtr->GetMediumLength();
				TimeEdit->setMaximumTime(QTime().addMSecs(mediumLength * 1000));
				PositionSlider->setMaximum(framesCount - 1);
				FrameIndexSB->setMaximum(framesCount - 1);
			}
			else{
				isSeekEnabled = false;
				PositionSlider->setValue(0);
				TimeEdit->setTime(QTime());
				FrameIndexSB->setValue(0);
			}
		}
		PositionSlider->setEnabled(isSeekEnabled);
		TimeEdit->setEnabled(isSeekEnabled);
		FrameIndexSB->setEnabled(isSeekEnabled);

		if ((updateFlags & imm::IMediaController::CF_MEDIA_POSITION) != 0){
			iqt::CSignalBlocker block(PositionSlider, true);

			int currentFrame = objectPtr->GetCurrentFrame();
			if (currentFrame >= 0){
				PositionSlider->setValue(currentFrame);
				FrameIndexSB->setValue(currentFrame);
				TimeEdit->setTime(QTime().addMSecs(objectPtr->GetCurrentPosition() * 1000));
			}
		}
	}
}


void CPlaybackControllerGuiComp::UpdateModel() const
{
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CPlaybackControllerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	PlayButton->setVisible(*m_showPlayButtonAttrPtr);
	PlayButton->setEnabled(false);
	TimeEdit->setVisible(*m_showTimePositionAttrPtr);
	TimeEdit->setEnabled(false);
	PositionSlider->setVisible(*m_showPositionSliderAttrPtr);
	PositionSlider->setEnabled(false);
	FrameIndexSB->setVisible(*m_showFrameIndexAttrPtr);
	FrameIndexSB->setEnabled(false);
	RepeatButton->setVisible(*m_showRepeatButtonAttrPtr);
	RepeatButton->setEnabled(false);
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
		objectPtr->SetCurrentFrame(position);
	}
}


void CPlaybackControllerGuiComp::on_FrameIndexSB_editingFinished()
{
	imm::IVideoController* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		objectPtr->SetCurrentFrame(FrameIndexSB->value());
	}
}


void CPlaybackControllerGuiComp::on_TimeEdit_editingFinished()
{
	imm::IVideoController* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		QTime time = TimeEdit->time();
		double t = (time.hour() * 60.0 + time.minute()) * 60 + time.second() + time.msec() * 0.001;
		objectPtr->SetCurrentPosition(t);
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


