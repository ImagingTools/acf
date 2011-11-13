#ifndef TutorialPck_included
#define TutorialPck_included


#include "icomp/TModelCompWrap.h"

#include "DocView/CTextEditorComp.h"

#include "ModelObserver/CAbComp.h"


/**
	Base system-undependent general package.
*/
namespace TutorialPck
{


typedef CTextEditorComp TextEditor;

typedef icomp::TModelCompWrap<CAbComp> AB;


} // namespace TutorialPck


#endif // !TutorialPck_included


