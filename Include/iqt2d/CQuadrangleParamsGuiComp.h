#ifndef iqt2d_CQuadrangleParamsGuiComp_included
#define iqt2d_CQuadrangleParamsGuiComp_included


#include "i2d/CQuadrangle.h"

#include "iview/CInteractiveParallelogramShape.h"

#include "iqt2d/TShapeParamsGuiCompBase.h"
#include "iqt2d/Generated/ui_CQuadrangleParamsGuiComp.h"


namespace iqt2d
{


class CQuadrangleParamsGuiComp:
			public iqt2d::TShapeParamsGuiCompBase<
						Ui::CQuadrangleParamsGuiComp,
						iview::CInteractiveParallelogramShape,
						i2d::CQuadrangle>
{
	Q_OBJECT

public:
	typedef iqt2d::TShapeParamsGuiCompBase<
				Ui::CQuadrangleParamsGuiComp,
				iview::CInteractiveParallelogramShape,
				i2d::CQuadrangle> BaseClass;

	I_BEGIN_COMPONENT(CQuadrangleParamsGuiComp);
	I_END_COMPONENT;

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


#endif // !iqt2d_CQuadrangleParamsGuiComp_included


