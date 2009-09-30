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
	I_END_COMPONENT();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor(int updateFlags = 0);
	virtual void UpdateModel() const;

private Q_SLOTS:
	void on_PlayButton_toggled(bool isToggled);
	void on_PositionSlider_valueChanged(int position);
};


} // namespace iqtmm


#endif // !iqtmm_CPlaybackControllerGuiComp_included

