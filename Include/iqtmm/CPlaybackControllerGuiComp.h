#ifndef iqtmm_CPlaybackControllerGuiComp_included
#define iqtmm_CPlaybackControllerGuiComp_included


// Qt includes
#include <QTimer>


// ACF inlcudes
#include "imm/IVideoController.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "generated/ui_CPlaybackControllerGuiComp.h"


namespace iqtmm
{


/**
	Component to play some image (frame) sequence from a directory.
*/
class CPlaybackControllerGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<Ui::CPlaybackControllerGuiComp, imm::IVideoController>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CPlaybackControllerGuiComp, imm::IVideoController> BaseClass;

	I_BEGIN_COMPONENT(CPlaybackControllerGuiComp);
		I_ASSIGN(m_showPlayButtonAttrPtr, "ShowPlayButton", "Allow to display play button", true, true);
		I_ASSIGN(m_showPositionSliderAttrPtr, "ShowPositionSlider", "Shows and allows edit position slider", true, true);
		I_ASSIGN(m_showTimePositionAttrPtr, "ShowTimePosition", "Shows and allows edit current time position", true, false);
		I_ASSIGN(m_showFrameIndexAttrPtr, "ShowFrameIndex", "Shows and allows edit current frame index", true, false);
		I_ASSIGN(m_showRepeatButtonAttrPtr, "ShowRepeatButton", "Allow to display play button", true, true);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor(int updateFlags = 0);
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private slots:
	void OnTimerTick();

private Q_SLOTS:
	void on_PlayButton_toggled(bool isToggled);
	void on_PositionSlider_valueChanged(int position);
	void on_FrameIndexSB_valueChanged(int position);
	void on_TimeEdit_editingFinished();

private:
	QTimer m_playTimer;
	int m_currentFrameIndex;

	I_ATTR(bool, m_showPlayButtonAttrPtr);
	I_ATTR(bool, m_showPositionSliderAttrPtr);
	I_ATTR(bool, m_showTimePositionAttrPtr);
	I_ATTR(bool, m_showFrameIndexAttrPtr);
	I_ATTR(bool, m_showRepeatButtonAttrPtr);
};


} // namespace iqtmm


#endif // !iqtmm_CPlaybackControllerGuiComp_included

