#include "icomp/export.h"

#include "imod/TModelWrap.h"

#include "DocView/CTextEditorComp.h"
#include "DocView/CTextModelComp.h"


I_EXPORT_PACKAGE("Tutorial", "Tutorial package", "Qt Tutorial");
I_EXPORT_COMPONENT(imod::TModelWrap<CTextModelComp>, TextModel, "Simple data model of text", "Text Tutorial Document Model Observer");
I_EXPORT_COMPONENT(CTextEditorComp, TextEditor, "Text editor being observer of text model", "Text Tutorial Document Observer Qt GUI");


