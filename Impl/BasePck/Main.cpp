#include "icomp/export.h"

#include "BasePck.h"


namespace BasePck
{


I_EXPORT_DEFAULT_SERVICES;

I_EXPORT_PACKAGE("Base", "Base system-undependent general package", "Undependend Standard Base");

I_EXPORT_COMPONENT(XmlFileSerializer, "Simple file loader serializing objects using XML format", "File Loader Load Save Serializer Archive XML Format");
I_EXPORT_COMPONENT(BinaryFileSerializer, "Simple file loader serializing objects using binary files", "Binary File Loader Load Save Serializer Archive Format");
I_EXPORT_COMPONENT(ComposedLoader, "Composed loader delegating loading and saving to slave loaders", "Composed File Loader Load Save Slave Delegating");
I_EXPORT_COMPONENT(TextFileSerializer, "Simple file serializer for text files", "Text File Serializer Format");
I_EXPORT_COMPONENT(Instantiator, "Access specified components forcing to create its instances", "Instance Init Create Free Components");
I_EXPORT_COMPONENT(ApplicationInfo, "Simple application info", "Application Info Version");
I_EXPORT_COMPONENT(RegistryCodeSaver, "Stores registry as C++ code", "Code C++ File Loader Store Save Registry Compile");
I_EXPORT_COMPONENT(ModelProxy, "Model proxy", "Model Proxy");
I_EXPORT_COMPONENT(ModelBinder, "Model binder", "Model Observer Bind Binder");
I_EXPORT_COMPONENT(FilePersistence, "Allows object persistence using files", "Persistence File Free Create Store Restore Load Save");
I_EXPORT_COMPONENT(ConsoleLog, "Simple log on windows console", "Log Windows Message Consumer");
I_EXPORT_COMPONENT(TextFileLog, "Simple text file log", "Log Message Consumer");
I_EXPORT_COMPONENT(FileCopyOverLoader, "General file copy or transform using loaders and any object", "File Copy Compile Transform Loader Saver");
I_EXPORT_COMPONENT(CopyApp, "Copy application transforming files", "File Copy Compile Transform Application");

I_EXPORT_COMPONENT(SelectableParamsSet, "Composed set of parameters", "Model Parameters Set Selection Options List");
I_EXPORT_COMPONENT(ParamsManager, "Manager of parameters set", "Model Parameters Set Manager");
I_EXPORT_COMPONENT(ComposedParamsSet, "Composed set of parameters", "Parameters Set List Composition Composed");
I_EXPORT_COMPONENT(FileNameParam, "Parameters for file/directory selection", "File Directory Parameters Model");
I_EXPORT_COMPONENT(EnableableParam, "Parameter with boolean flag", "Enabled Bool Parameter Model");

I_EXPORT_COMPONENT(SingleDocumentTemplate, "Single document template for only one type of documents", "Single Document Template MVC");
I_EXPORT_COMPONENT(CompositeDocumentTemplate, "Composite document template supporting more type of documents. It delegates document functionality to set of slave templates", "Composite Document Template MVC");
I_EXPORT_COMPONENT(DocumentManagerListener, "Document workspace controller", "Model Proxy Composite Document Template MVC");

I_EXPORT_COMPONENT(ComposedAuthorizationVerifier, "Allows to combine many authorization versifiers.", "Authorization Verifier Composition Right Security");
I_EXPORT_COMPONENT(FixedAuthorisationVerifier, "Authorization verifier with fixed rights", "Simple Authorization Verifier Fixed Right Security");
I_EXPORT_COMPONENT(StaticUserLogin, "Authorization verifier delegating authorisation request depent on logged user", "User Login Authorization Verifier Right Security");

I_EXPORT_COMPONENT(Line, "Line 2D object", "Line 2D Object Parameter");
I_EXPORT_COMPONENT(Position2d, "Position 2D object", "Position 2D Object Parameter");
I_EXPORT_COMPONENT(Circle, "Circle 2D object", "Circle Position Radius 2D Object Parameter");
I_EXPORT_COMPONENT(Rectangle, "Rectangle (orthogonal to axis) object", "Rectangle Orthogonal 2D Object Parameter");
I_EXPORT_COMPONENT(Annulus, "Annulus object", "Annulus 2D Object Parameter");
I_EXPORT_COMPONENT(AnnulusSegment, "Segment of annulus object", "Annulus Segment 2D Object Parameter");
I_EXPORT_COMPONENT(Quadrangle, "Quadrangle object", "Quadrangle 2D Object Parameter");
I_EXPORT_COMPONENT(AttributedRectangle, "Rectangle component with attributes", "Rectangle 2D Object Parameter");
I_EXPORT_COMPONENT(TextDocument, "Simple text document", "Text Document");

I_EXPORT_COMPONENT(SampledFunction2d, "Sampled function with 2-dimensional input", "Function Sample");

I_EXPORT_COMPONENT(Histogram, "Histogram ", "Image Histogram");

I_EXPORT_COMPONENT(TestIdManager, "Implementation of ID manager generating ID's for test", "ID Manager Test Supplier");
I_EXPORT_COMPONENT(CascadedProcessor, "Sequence processing using slave processors", "Processor Sequence Buffer Cascade");


} // namespace BasePck


