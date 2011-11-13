#include "icomp/export.h"

#include "TutorialPck.h"


namespace TutorialPck
{


I_EXPORT_PACKAGE(
			"Tutorial",
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


} // namespace TutorialPck


