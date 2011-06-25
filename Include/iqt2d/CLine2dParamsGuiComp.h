#ifndef iqt2d_CLine2dParamsGuiComp_included
#define iqt2d_CLine2dParamsGuiComp_included


#include "i2d/CLine2d.h"


#include "iqt2d/CLine2dShape.h"
#include "iqt2d/TShapeParamsGuiCompBase.h"

#include "iqt2d/Generated/ui_CLine2dParamsGuiComp.h"


namespace iqt2d
{


class CLine2dParamsGuiComp:
			public iqt2d::TShapeParamsGuiCompBase<
						Ui::CLine2dParamsGuiComp,
						iqt2d::CLine2dShape,
						i2d::CLine2d>
{
	Q_OBJECT

public:
	typedef iqt2d::TShapeParamsGuiCompBase<
				Ui::CLine2dParamsGuiComp,
				iqt2d::CLine2dShape,
				i2d::CLine2d> BaseClass;

	I_BEGIN_COMPONENT(CLine2dParamsGuiComp)
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

protected Q_SLOTS:
	void OnParamsChanged(double value);
};


} // namespace iqt2d


#endif // !iqt2d_CLine2dParamsGuiComp_included


