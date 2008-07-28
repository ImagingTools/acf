#include <QDir>

#include "icomp/export.h"

#include "QtIprPck.h"


namespace QtIprPck
{


I_EXPORT_PACKAGE("QtIpr", "Standard Qt image processing package", "Qt Image Processing");

I_EXPORT_COMPONENT(RectangularFilterParamsGui, "Rectangular filter params", "Filter Parameter Qt Image Processing");
I_EXPORT_COMPONENT(IterationParamsGui, "Iterative processor params", "Filter Parameter Qt Image Processing");
I_EXPORT_COMPONENT(SearchGui, "SearchGui", "Model Search Qt Image Processing");
I_EXPORT_COMPONENT(SearchParamsGui, "SearchParamsGui", "Model Search Parameter Qt Image Processing");


} // namespace QtIprPck
