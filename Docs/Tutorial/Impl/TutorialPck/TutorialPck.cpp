#include <icomp/export.h>

#include "TutorialPck.h"


namespace TutorialPck
{


I_EXPORT_PACKAGE(
			"Acf/Tutorial",
			"ACF Tutorial package",
			IM_TAG("Qt Tutorial") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			TextEditor,
			"Text editor being observer of text model",
			"Text Tutorial Document Observer Qt GUI");

I_EXPORT_COMPONENT(
			AB,
			"Data model of AB",
			"Data Model AB");

I_EXPORT_COMPONENT(
			AbGui,
			"Editor for AB data model",
			"Data Model AB Editor GUI");

I_EXPORT_COMPONENT(
			InterpolationTest,
			"Test of polyline interpolation",
			"Data Model Polyline Polygon Interpolation Nodes Function");


} // namespace TutorialPck


