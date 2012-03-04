#include "QtViewPck.h"


#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


namespace QtViewPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE(
			"Acf/Qt/Gui/View",
			"Viewport package",
			IM_PROJECT("ACF") IM_TAG("Qt Viewport") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			SceneExtenderDelegator,
			"Delegate a scene extender",
			"Scene Connector Provider Extender Display Console" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			SceneConnector,
			"Display scene provider, scene extender and connect it",
			"Scene Connector Provider Extender Display Console" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			SceneProvider,
			"Standard scene provider",
			"Scene Provider Extender Display Console" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			ImageView,
			"Display bitmap using observer pattern",
			"Bitmap Display Viewer Observer" IM_TAG("GUI Image 2D"));

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
			"Circle parameter GUI displaying a cicle using scene",
			"Circle Parameters Observer Display" IM_TAG("GUI Editor 2D"));

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
			SceneControllerGui,
			"Scene controller allowing to control scale and other scene properties",
			"Observer Display GUI Scene Controller Zoom Rotate" IM_TAG("GUI"));

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


} // namespace QtViewPck


