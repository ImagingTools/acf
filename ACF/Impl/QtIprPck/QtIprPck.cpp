#include <QDir>

#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QtIprPck.h"


namespace QtIprPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("QtIpr", "Standard Qt image processing package", "Qt Image Processing");

I_EXPORT_COMPONENT(RectangularFilterParamsGui, "Rectangular filter params", "Filter Parameter Qt Image Processing");
I_EXPORT_COMPONENT(IterationParamsGui, "Iterative processor params", "Filter Parameter Qt Image Processing");
I_EXPORT_COMPONENT(SearchGui, "SearchGui", "Model Search Qt Image Processing");
I_EXPORT_COMPONENT(SearchParamsGui, "SearchParamsGui", "GUI Model Search Parameter Qt Image Processing");
I_EXPORT_COMPONENT(ValueSupplierGui, "GUI for supplier implementation returning position found using caliper tool", "GUI Observer Caliper Position Qt Processing Supplier");
I_EXPORT_COMPONENT(CaliperParamsGui, "GUI to parametrize caliper", "GUI Observer Caliper Parameters Direction Edge Mode Threshold Qt");


} // namespace QtIprPck


