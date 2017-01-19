#ifndef iqt2d_CAnnulusSegmentParamsGuiComp_included
#define iqt2d_CAnnulusSegmentParamsGuiComp_included


// ACF includes
#include <i2d/CAnnulusSegment.h>
#include <iview/CAnnulusSegmentShape.h>
#include <iqt2d/TShapeParamsGuiCompBase.h>

#include <GeneratedFiles/iqt2d/ui_CAnnulusSegmentParamsGuiComp.h>


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

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiRetranslate();

protected Q_SLOTS:
	void OnParamsChanged(double value);
	void OnActionTriggered(QAction* actionPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CAnnulusSegmentParamsGuiComp_included


