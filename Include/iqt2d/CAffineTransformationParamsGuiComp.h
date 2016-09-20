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
public:
	typedef iqt2d::TShapeParamsGuiCompBase<
	Ui::CAffineTransformationParamsGui,
	iview::CAffineTransformation2dShape,
	i2d::CAffineTransformation2d> BaseClass;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

	virtual void CreateShapes(int sceneId, Shapes& result);

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);


	Q_OBJECT
	protected Q_SLOTS:
	void OnParamsChanged(double value);
	void on_ResetButton_clicked(bool = false);
};


} // namespace iqt2d


#endif // CAffineTransformationParamsGui_included

