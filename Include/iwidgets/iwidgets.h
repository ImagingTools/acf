#ifndef iwidgets_included
#define iwidgets_included


// ACF includes
#include <iqt/iqt.h>


class QLayout;


/**
	Namespace contains implementation of Qt widget extensions
*/
namespace iwidgets
{


/**
	Remove all items (widgets and sub-layouts) from a given layout.
*/
void ClearLayout(QLayout* layout);


} // namespace iwidgets


#endif // !iwidgets_included


