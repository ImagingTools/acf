#ifndef TutorialPck_included
#define TutorialPck_included


#include "icomp/TModelCompWrap.h"

#include "DocView/CTextEditorComp.h"
#include "DocView/CTextModelComp.h"


/**
	Base system-undependent general package.
*/
namespace TutorialPck
{


typedef icomp::TModelCompWrap<CTextModelComp> TextModel;
typedef CTextEditorComp TextEditor;


} // namespace TutorialPck


#endif // !TutorialPck_included


