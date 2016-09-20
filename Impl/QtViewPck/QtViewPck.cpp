#include "QtViewPck.h"


#include <icomp/export.h>


I_REGISTER_QT_RESOURCE(iqtgui);


namespace QtViewPck
{


I_EXPORT_PACKAGE(
			"Acf/Qt/Gui/View",
			"Viewport package",
			IM_PROJECT("ACF") IM_TAG("Qt Viewport") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ShapeStatusInfo,
			"Status information about a shape in the 2D-Console",
			"Display Console Shape Info" IM_TAG("Model 2D"));

I_EXPORT_COMPONENT(
			ColorSchema,
			"ColorSchema attributes set in comonent",
			"ColorSchema Color" IM_TAG("Color Schema"));

I_EXPORT_COMPONENT(
			NoneCalibrationShape,
			"Displays uncalibrated grid on view console",
			"Uncalibrated None Calibration 2D Grid Shape");

I_EXPORT_COMPONENT(
			AffiniteCalibrationShape,
			"Shows affine calibration grid on calibrated view",
			"Calibration Affine Display Observer 2D Grid Shape");

I_EXPORT_COMPONENT(
			PerspectiveCalibrationShape,
			"Shows perpective calibration grid on calibrated view",
			"Calibration Perspective Display Observer 2D Grid Shape");

I_EXPORT_COMPONENT(
			ShapeInfoGui,
			"GUI for the status information about a shape in the 2D-Console",
			"Display Console Shape Info" IM_TAG("GUI 2D"));

I_EXPORT_COMPONENT(
			SimpleShapeFactory,
			"Shape factory for common 2D-primitives",
			"View Display Console Shape Factory" IM_TAG("GUI 2D"));

I_EXPORT_COMPONENT(
			SceneExtenderDelegator,
			"Delegates a scene extender",
			"Scene Connector Provider Extender Display Console" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			MultiSceneExtenderDelegator,
			"Delegates several scene extenders",
			"Multi Scene Connector Provider Extender Display Console" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			SceneConnector,
			"Display scene provider, scene extender and connect it",
			"Scene Connector Provider Extender Display Console" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			ImageView,
			"Display bitmap using observer pattern",
			"Bitmap Display Viewer Observer" IM_TAG("GUI Image 2D"));

I_EXPORT_COMPONENT(
			Position2dParamsGui,
			"Position parameter GUI displaying its using scene",
			"Position Point Parameters Observer Display" IM_TAG("GUI Editor 2D"));

I_EXPORT_COMPONENT(
			Line2dParamsGui,
			"Line parameter GUI displaying line using scene",
			"Line Parameters Observer Display" IM_TAG("GUI Editor 2D"));

I_EXPORT_COMPONENT(
			RectangleParamsGui,
			"Rectangle parameter GUI displaying a rectangle using scene",
			"Rectangle Parameters Observer Display" IM_TAG("GUI Editor 2D"));

I_EXPORT_COMPONENT(
			CircleParamsGui,
			"Circle parameter GUI displaying a circle using scene",
			"Circle Parameters Observer Display" IM_TAG("GUI Editor 2D"));

I_EXPORT_COMPONENT(
			OrientedCircleEditor,
			"Oriented circle parameter GUI displaying a circle with orientation, using scene",
			"Circle Parameters Observer Display Oriented" IM_TAG("GUI Editor 2D"));

I_EXPORT_COMPONENT(
			AnnulusParamsGui,
			"Annulus parameter GUI displaying a annulus using scene",
			"Annulus Radius Radii Parameters Observer Display" IM_TAG("GUI Editor 2D"));

I_EXPORT_COMPONENT(
			AnnulusSegmentParamsGui,
			"Annulus segment parameter GUI displaying a annulus using scene",
			"Annulus Segment Angle Radius Radii Parameters Observer Display" IM_TAG("GUI Editor 2D"));

I_EXPORT_COMPONENT(
			QuadrangleParamsGui,
			"Quadrangle parameter GUI displaying a quadrangle object using scene",
			"Quadrangle Parameters Observer Display" IM_TAG("GUI Editor 2D"));

I_EXPORT_COMPONENT(
			PolygonParamsGui,
			"Polygon parameter GUI displaying a polygon using scene",
			"Polygon Parameters Observer Display" IM_TAG("GUI Editor 2D"));

I_EXPORT_COMPONENT(
			SceneBinder,
			"Simple binder for scene provider and scene extender",
			"Scene Extender Binder Console Connector Display" IM_TAG("2D"));

I_EXPORT_COMPONENT(
			SelectableSceneExtender,
			"Selectable scene extender",
			"Scene Extender Selection Console Connector Display" IM_TAG("2D"));

I_EXPORT_COMPONENT(
			ViewProvider,
			"View provider based on 2D-console",
			"View Console Display" IM_TAG("2D"));

I_EXPORT_COMPONENT(
			ExtendedConsoleGui,
			"2D-View decorated with commands toolbar and status widget",
			"View Console Display" IM_TAG("2D"));

I_EXPORT_COMPONENT(
			PolylineParamsGui,
			"Polyline parameter GUI displaying a polyline using 2D-view",
			"View Console Polyline Shape" IM_TAG("2D"));

I_EXPORT_COMPONENT(
			TubePolylineParamsGui,
			"Tube region parameter GUI displaying a polyline and its tube using 2D-view",
			"View Console Polyline Shape" IM_TAG("2D"));

I_EXPORT_COMPONENT(
			AffineTransformationParamsGui,
			"Affine transformations parameter GUI displaying",
			"Affine Transformations Parameters" IM_TAG("GUI Editor 2D"));

I_EXPORT_COMPONENT(
			MultiBitmapView,
			"Bitmap visualization for multi bitmap provider", 
			"Image Bitmap Multiple View");

I_EXPORT_COMPONENT(
			ArcParamsGui,
			"Arc parameter GUI displaying a arc using scene",
			"Arc Parameters Observer Display" IM_TAG("GUI Editor 2D"));

I_EXPORT_COMPONENT(
			PerspectiveCalibEditor,
			"Editor of perspective calibration parameters",
			"Perspective Calibration Transformation Matrix Translation" IM_TAG("GUI Editor 2D"));


} // namespace QtViewPck


