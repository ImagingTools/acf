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
I_EXPORT_COMPONENT(InvertOperator, "Invertation operator", "Operator Image Processing");
I_EXPORT_COMPONENT(GaussianFilter, "Gaussian filter", "Filter Operator Gaussian Image Processing");
I_EXPORT_COMPONENT(GaussianFilterParams, "Gaussian filter params", "Filter Operator Gaussian Image Processing");
I_EXPORT_COMPONENT(CaliperParams, "Parameters for caliper tool", "Caliper Weight Direction Gradients Projection Parameters");
I_EXPORT_COMPONENT(ExtremumCaliperProcessor, "Caliper implementation extracting projection extremums", "Caliper Extremum Minimum Maximum Projection Processor");
I_EXPORT_COMPONENT(LineProjectionProcessor, "Line projection implementation for single line width", "Projection Line Processor");
I_EXPORT_COMPONENT(SearchResults, "Search results", "Model Search Feature Extraction");
I_EXPORT_COMPONENT(CaliperBasedPositionSupplier, "supplier implementation returning position found using caliper tool", "Model Caliper Position Processing Supplier");


} // namespace IprPck


