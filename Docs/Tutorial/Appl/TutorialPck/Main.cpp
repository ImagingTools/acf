#include "icomp/export.h"

#include "TutorialPck.h"


namespace TutorialPck
{


I_EXPORT_PACKAGE("Tutorial", "Tutorial package", "Qt Tutorial");
I_EXPORT_COMPONENT(TextModel, "Simple data model of text", "Text Tutorial Document Model Observer");
I_EXPORT_COMPONENT(TextEditor, "Text editor being observer of text model", "Text Tutorial Document Observer Qt GUI");


} // namespace TutorialPck


