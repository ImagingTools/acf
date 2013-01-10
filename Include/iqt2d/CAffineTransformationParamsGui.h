#ifndef CAffineTransformationParamsGui_included
#define CAffineTransformationParamsGui_included

// ACF includes
#include <iview/CAffineTransformation2dShape.h>
#include <i2d/CAffineTransformation2d.h>

#include <iqt2d/TShapeParamsGuiCompBase.h>

#include "iqt2d/Generated/ui_CAffineTransformationParamsGui.h"


namespace iqt2d
{


/**
	Gui Editor for shape affine transformation parameters
 */
class CAffineTransformationParamsGui:
			public iqt2d::TShapeParamsGuiCompBase<
						Ui::CAffineTransformationParamsGui,
						iview::CAffineTransformation2dShape,
						i2d::CAffineTransformation2d>
{
	Q_OBJECT

public:
	typedef iqt2d::TShapeParamsGuiCompBase<
					Ui::CAffineTransformationParamsGui,
					iview::CAffineTransformation2dShape,
					i2d::CAffineTransformation2d> BaseClass;
	

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


#endif // CAffineTransformationParamsGui_included

