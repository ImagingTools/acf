#ifndef iqt2d_CAnnulusParamsGuiComp_included
#define iqt2d_CAnnulusParamsGuiComp_included


// ACF includes
#include "i2d/CAnnulus.h"

#include "iview/CAnnulusShape.h"

#include "iqt2d/TShapeParamsGuiCompBase.h"
#include "iqt2d/Generated/ui_CAnnulusParamsGuiComp.h"


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
};


} // namespace iqt2d


#endif // !iqt2d_CAnnulusParamsGuiComp_included


