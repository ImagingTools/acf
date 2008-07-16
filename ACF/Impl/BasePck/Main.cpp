#include "icomp/export.h"

#include "BasePck.h"


namespace BasePck
{


I_EXPORT_PACKAGE("Base", "Base system-undependent general package", "Undependend Standard Base");

I_EXPORT_COMPONENT(XmlFileSerializer, "Simple file serializer using XML format", "File Serializer Archive XML Format");
I_EXPORT_COMPONENT(ApplicationInfo, "Simple application info based of SVN version", "Application Info SVN SubVersion Version");
I_EXPORT_COMPONENT(SelectableParamsSet, "Composed set of parameters", "Model Parameters Set Selection Options List");
I_EXPORT_COMPONENT(ParamsManager, "Manager of parameters set", "Model Parameters Set Manager");
I_EXPORT_COMPONENT(ComposedParamsSet, "Composed set of parameters", "Parameters Set List Composition Composed");
I_EXPORT_COMPONENT(SingleDocumentTemplate, "Single document template for only one type of documents", "Single Document Template MVC");
I_EXPORT_COMPONENT(CompositeDocumentTemplate, "Composite document template supporting more type of documents. It delegates document functionality to set of slave templates", "Composite Document Template MVC");
I_EXPORT_COMPONENT(SamplingParams, "Sampling params based on fixed boundary values", "Sample Sampling Acquisition Model Parameters");
I_EXPORT_COMPONENT(FileBitmapAcquisitionParams, "Parameters for bitmap acquisition from file", "File Directory Bitmap Acquisition Snap Parameters");
I_EXPORT_COMPONENT(ExposureParams, "Parameters for exposure parameter control", "Exposure Shutter Delay Time EEN Bitmap Acquisition Snap Parameters");
I_EXPORT_COMPONENT(Line, "Line 2D object", "Line 2D Object Parameter");


} // namespace BasePck


