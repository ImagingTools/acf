#ifndef iqt2d_CAnnulusParamsGuiComp_included
#define iqt2d_CAnnulusParamsGuiComp_included


// ACF includes
#include <i2d/CAnnulus.h>

#include <iview/CAnnulusShape.h>

#include <iqt2d/TShapeParamsGuiCompBase.h>
#include <GeneratedFiles/iqt2d/ui_CAnnulusParamsGuiComp.h>


namespace iqt2d
{


class CAnnulusParamsGuiComp:
			public iqt2d::TShapeParamsGuiCompBase<
						Ui::CAnnulusParamsGuiComp,
						iview::CAnnulusShape,
						i2d::CAnnulus>
{
	Q_OBJECT

public:
	typedef iqt2d::TShapeParamsGuiCompBase<
				Ui::CAnnulusParamsGuiComp,
				iview::CAnnulusShape,
				i2d::CAnnulus> BaseClass;

	I_BEGIN_COMPONENT(CAnnulusParamsGuiComp);
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


#endif // !iqt2d_CAnnulusParamsGuiComp_included


