#ifndef iqt2d_COrientedCircleEditorComp_included
#define iqt2d_COrientedCircleEditorComp_included


#include "i2d/COrientedCircle.h"

#include "iview/COrientedCircleShape.h"

#include "iqt2d/TShapeParamsGuiCompBase.h"
#include "iqt2d/Generated/ui_COrientedCircleEditorComp.h"


namespace iqt2d
{


class COrientedCircleEditorComp:
	public iqt2d::TShapeParamsGuiCompBase<
	Ui::COrientedCircleEditorComp,
	iview::COrientedCircleShape,
	i2d::COrientedCircle>
{
	Q_OBJECT

public:
	typedef iqt2d::TShapeParamsGuiCompBase<
	Ui::COrientedCircleEditorComp,
	iview::COrientedCircleShape,
	i2d::COrientedCircle> BaseClass;

	I_BEGIN_COMPONENT(COrientedCircleEditorComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiRetranslate();

protected Q_SLOTS:
	void OnParamsChanged(double value);
	void on_AxisOrientationCheckBox_stateChanged(int state);

};


} // namespace iqt2d


#endif // !iqt2d_COrientedCircleEditorComp_included
