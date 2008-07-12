#include "icomp/export.h"

#include "IprPck.h"


namespace IprPck
{


I_EXPORT_PACKAGE("Ipr", "Image processing package", "Image Processing");

I_EXPORT_COMPONENT(MovingAverageProcessor, "Moving average image processor", "Image Processing");
I_EXPORT_COMPONENT(RectangularFilterParams, "Rectangular filter parameter", "Filter Image Processing Parameter");
I_EXPORT_COMPONENT(LocalDifferenceProcessor, "Line difference image processor", "Image Processing");
I_EXPORT_COMPONENT(LocalDifferenceFilterParams, "Line difference processor params", "Filter Image Processing Parameter");
I_EXPORT_COMPONENT(ProcessedCamera, "Processed Camera", "Image Bitmap Acquisition Snap Parameters Camera Processing");
I_EXPORT_COMPONENT(IterationProcessor, "Iterative Processor", "Image Bitmap Processing");
I_EXPORT_COMPONENT(IterativeProcessorParams, "Iterative Processor Params", "Iterative Filter Image Processing Parameter");
I_EXPORT_COMPONENT(MorphoMinOperator, "Morphological minimum operator with rectangular kernel", "Morphological Operator Image Processing");
I_EXPORT_COMPONENT(MorphoMaxOperator, "Morphological maximum operator with rectangular kernel", "Morphological Operator Image Processing");
I_EXPORT_COMPONENT(InverOperator, "Invertation operator", "Operator Image Processing");


} // namespace IprPck


