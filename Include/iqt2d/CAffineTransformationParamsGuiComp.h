#ifndef CAffineTransformationParamsGui_included
#define CAffineTransformationParamsGui_included

// ACF includes
#include <iview/CAffineTransformation2dShape.h>
#include <i2d/CAffineTransformation2d.h>
#include <iqt2d/TShapeParamsGuiCompBase.h>

#include <GeneratedFiles/iqt2d/ui_CAffineTransformationParamsGui.h>


namespace iqt2d
{


/**
	Gui Editor for shape affine transformation parameters
 */
class CAffineTransformationParamsGuiComp:
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

	I_BEGIN_COMPONENT(CAffineTransformationParamsGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void OnParamsChanged(double value);
	void on_ResetButton_clicked(bool = false);
};


} // namespace iqt2d


#endif // CAffineTransformationParamsGui_included

