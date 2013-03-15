#ifndef iqt2d_CAnnulusSegmentParamsGuiComp_included
#define iqt2d_CAnnulusSegmentParamsGuiComp_included


// ACF includes
#include "i2d/CAnnulusSegment.h"

#include "iview/CAnnulusSegmentShape.h"

#include "iqt2d/TShapeParamsGuiCompBase.h"
#include "iqt2d/Generated/ui_CAnnulusSegmentParamsGuiComp.h"


namespace iqt2d
{


class CAnnulusSegmentParamsGuiComp:
			public iqt2d::TShapeParamsGuiCompBase<
						Ui::CAnnulusSegmentParamsGuiComp,
						iview::CAnnulusSegmentShape,
						i2d::CAnnulusSegment>
{
	Q_OBJECT

public:
	typedef iqt2d::TShapeParamsGuiCompBase<
				Ui::CAnnulusSegmentParamsGuiComp,
				iview::CAnnulusSegmentShape,
				i2d::CAnnulusSegment> BaseClass;

	I_BEGIN_COMPONENT(CAnnulusSegmentParamsGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemenented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiRetranslate();

protected Q_SLOTS:
	void OnParamsChanged(double value);
};


} // namespace iqt2d


#endif // !iqt2d_CAnnulusSegmentParamsGuiComp_included


