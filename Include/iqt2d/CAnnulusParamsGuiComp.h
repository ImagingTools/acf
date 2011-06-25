#ifndef iqt2d_CAnnulusParamsGuiComp_included
#define iqt2d_CAnnulusParamsGuiComp_included


#include "i2d/CAnnulus.h"

#include "iqt2d/TShapeParamsGuiCompBase.h"
#include "iqt2d/CAnnulusShape.h"

#include "iqt2d/Generated/ui_CAnnulusParamsGuiComp.h"


namespace iqt2d
{


class CAnnulusParamsGuiComp:
			public iqt2d::TShapeParamsGuiCompBase<
						Ui::CAnnulusParamsGuiComp,
						iqt2d::CAnnulusShape,
						i2d::CAnnulus>
{
	Q_OBJECT

public:
	typedef iqt2d::TShapeParamsGuiCompBase<
				Ui::CAnnulusParamsGuiComp,
				iqt2d::CAnnulusShape,
				i2d::CAnnulus> BaseClass;

	I_BEGIN_COMPONENT(CAnnulusParamsGuiComp);
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemenented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

protected Q_SLOTS:
	void OnParamsChanged(double value);
};


} // namespace iqt2d


#endif // !iqt2d_CAnnulusParamsGuiComp_included


