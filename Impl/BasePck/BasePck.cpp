#include "BasePck.h"


#include "icomp/export.h"


namespace BasePck
{


I_EXPORT_DEFAULT_SERVICES;

I_EXPORT_PACKAGE(
			"Base",
			"Base system-undependent general package",
			IM_PROJECT("ACF") IM_TAG("Basic") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			SerializedUndoManager,
			"Provide UNDO manager based on complete object serialization",
			"UNDO REDO Manager Persistence Serialization" IM_TAG("Model Document"));

I_EXPORT_COMPONENT(
			FileTypeInfo,
			"General file type information provider",
			"Type Information Format" IM_TAG("File"));

I_EXPORT_COMPONENT(
			XmlFileSerializer,
			"File loader serializing objects using simplified XML format",
			"Save Serializer Archive Format" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("XML Loader File"));

I_EXPORT_COMPONENT(
			BinaryFileSerializer,
			"Simple file loader serializing objects using binary files",
			"Binary Load Save Serializer Archive Format" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("Loader File"));

I_EXPORT_COMPONENT(
			ComposedLoader,
			"Composed loader delegating loading and saving to slave loaders",
			"Composed File Load Save Slave Delegating" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("Composition Loader"));

I_EXPORT_COMPONENT(
			TextFileLoader,
			"Simple file serializer for text files",
			"Text Loader File Serializer Format" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("Text Loader"));

I_EXPORT_COMPONENT(
			Instantiator,
			"Access specified components forcing to create its instances",
			"Instance Create Free Components" IM_CATEGORY(I_SERVICE));

I_EXPORT_COMPONENT(
			VersionInfo,
			"Genaral information about versions",
			"Version Information" IM_CATEGORY(I_SERVICE) IM_TAG("Version"));

I_EXPORT_COMPONENT(
			ApplicationInfo,
			"General application information provider",
			"Application Meta Information" IM_CATEGORY(I_SERVICE) IM_TAG("Version"));

I_EXPORT_COMPONENT(
			RegistryCodeSaver,
			"Stores registry as C++ code",
			"Code C++ File Store Save Registry Compile" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("Loader"));

I_EXPORT_COMPONENT(
			ModelProxy,
			"Model proxy",
			"Proxy" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model"));

I_EXPORT_COMPONENT(
			ModelBinder,
			"Model binder",
			"Observer Bind Composition Connection" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Composition"));

I_EXPORT_COMPONENT(
			AutoPersistence,
			"Allows object persistence using files",
			"Free Create Store Restore Load Save" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("Persistence File"));

I_EXPORT_COMPONENT(
			ConsoleLog,
			"Simple log on windows console",
			"Windows Message Consumer" IM_CATEGORY(I_SERVICE) IM_TAG("Log"));

I_EXPORT_COMPONENT(
			TextFileLog,
			"Simple text file log",
			"Message Consumer" IM_CATEGORY(I_SERVICE) IM_TAG("Text File Log"));

I_EXPORT_COMPONENT(
			FileCopyOverLoader,
			"General file copy or transform using loaders and any object",
			"Copy Compile Transform Saver" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("File Loader"));

I_EXPORT_COMPONENT(
			ExternalFileConverter,
			"General file copy or transform using loaders and any object",
			"File Converter External Application" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("File Converter"));

I_EXPORT_COMPONENT(
			CopyApp,
			"Copy application transforming files",
			"Copy Compile Transform Application" IM_CATEGORY(I_DATA_PROCESSING I_APPLICATION) IM_TAG("File"));

I_EXPORT_COMPONENT(
			Uuid,
			"Unique identifier",
			"Unique Identifier UUID Generated" IM_CATEGORY(I_SERVICE));

I_EXPORT_COMPONENT(
			ObjectQueue,
			"Queue of any objects",
			"Objects Queue List FIFO Stack Elements Serial Delay Line" IM_CATEGORY(I_SERVICE));

I_EXPORT_COMPONENT(
			SelectableFileConverter,
			"Selectable file converter",
			"File Converter" IM_CATEGORY(I_DATA_PROCESSING I_SERVICE));

I_EXPORT_COMPONENT(
			FileConvertOverProcessor,
			"File conversion over processor",
			"File Converter" IM_CATEGORY(I_DATA_PROCESSING I_SERVICE));

I_EXPORT_COMPONENT(
			SelectableParamsSet,
			"Composed set of parameters",
			"Parameters Set Selection Options List" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Parameter"));

I_EXPORT_COMPONENT(
			ParamsManager,
			"Manager of parameters set",
			"Parameters Set Manager" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Composition Parameter"));

I_EXPORT_COMPONENT(
			ComposedParamsSet,
			"Composed set of parameters",
			"Parameters Set List Composed" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Composition Parameter"));

I_EXPORT_COMPONENT(
			VariableParam,
			"Parameter which can change its type",
			"Parameter Choice Change Selection Mutable Variable" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Parameter"));

I_EXPORT_COMPONENT(
			FileNameParam,
			"Parameters for file/directory selection",
			"Name Directory Parameters URL" IM_CATEGORY(I_DATA_MODEL) IM_TAG("File Model Parameter"));

I_EXPORT_COMPONENT(
			EnableableParam,
			"Parameter with boolean flag",
			"Parameter Enabled Bool" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Parameter"));

I_EXPORT_COMPONENT(
			LinearAdjustParams,
			"Parameter with scale and offset for simple linear transformations",
			"Parameter Enabled Bool" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Parameter"));

I_EXPORT_COMPONENT(
			SelectionParam,
			"Selection Parameter",
			"Selection Parameter Integer Index Choice" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Parameter"));

I_EXPORT_COMPONENT(
			ManagerParamsSetShadower,
			"Synchronize selected parameter set with shadow object",
			"Synchronizer Synchronisation" IM_CATEGORY(I_DATA_MODEL));

I_EXPORT_COMPONENT(
			SelectionConstraints,
			"Define selection constraints",
			"Selection Constraints" IM_CATEGORY(I_DATA_MODEL));

I_EXPORT_COMPONENT(
			NameParam,
			"Name parameter",
			"Name Parameter" IM_CATEGORY(I_DATA_MODEL));

I_EXPORT_COMPONENT(
			SingleDocumentTemplate,
			"Single document template for only one type of documents",
			"Single Template MVC" IM_CATEGORY(I_APPLICATION_LOGIC) IM_TAG("Document"));

I_EXPORT_COMPONENT(
			CompositeDocumentTemplate,
			"Composite document template supporting more type of documents.It delegates document functionality to set of slave templates",
			"Composite Template MVC" IM_CATEGORY(I_APPLICATION_LOGIC) IM_TAG("Document Composition"));

I_EXPORT_COMPONENT(
			DocumentManagerListener,
			"Document workspace controller",
			"Model Proxy Composite Template MVC" IM_CATEGORY(I_APPLICATION_LOGIC) IM_TAG("Document"));

I_EXPORT_COMPONENT(
			SelectedDocModelBinder,
			"Connect model with observer if document is active",
			"Multiple Model Observer Composite Template MVC" IM_CATEGORY(I_APPLICATION_LOGIC) IM_TAG("Document"));

I_EXPORT_COMPONENT(
			ComposedAuthorizationVerifier,
			"Allows to combine many authorization versifiers.",
			"Verifier Right Security" IM_CATEGORY(I_APPLICATION_LOGIC) IM_TAG("Authorization Composition"));

I_EXPORT_COMPONENT(
			FixedAuthorisationVerifier,
			"Authorization verifier with fixed rights",
			"Simple Verifier Fixed Right Security" IM_CATEGORY(I_APPLICATION_LOGIC) IM_TAG("Authorization"));

I_EXPORT_COMPONENT(
			StaticUserLogin,
			"Authorization verifier delegating authorisation request depent on logged user",
			"User Login Verifier Right Security" IM_CATEGORY(I_APPLICATION_LOGIC) IM_TAG("Authorization"));

I_EXPORT_COMPONENT(
			Line,
			"Line 2D object",
			"Line Object Parameter" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

I_EXPORT_COMPONENT(
			Position2d,
			"Position 2D object",
			"Position Object Parameter" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

I_EXPORT_COMPONENT(
			Circle,
			"Circle 2D object",
			"Circle Position Radius 2D Object Parameter" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

I_EXPORT_COMPONENT(
			Rectangle,
			"Rectangle (orthogonal to axis) object",
			"Rectangle Orthogonal Object Parameter" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

I_EXPORT_COMPONENT(
			Annulus,
			"Annulus object",
			"Annulus 2D Object Parameter" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

I_EXPORT_COMPONENT(
			AnnulusSegment,
			"Segment of annulus object",
			"Annulus Segment 2D Object Parameter" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

I_EXPORT_COMPONENT(
			Quadrangle,
			"Quadrangle object",
			"Quadrangle 2D Object Parameter" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

I_EXPORT_COMPONENT(
			TextDocument,
			"Simple text document",
			IM_CATEGORY(I_DATA_MODEL) IM_TAG("Text Document"));

I_EXPORT_COMPONENT(
			SampledFunction2d,
			"Sampled function with 2-dimensional input",
			"Function Sample" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Sampling Model"));

I_EXPORT_COMPONENT(
			CascadedProcessor,
			"Sequence processing using slave processors",
			"Processor Sequence Buffer Cascade" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Processing Composition"));

I_EXPORT_COMPONENT(
			IterativeProcessor,
			"Iterative Processor",
			"Iterative Loop Sequence Processor" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Processing"));

I_EXPORT_COMPONENT(
			Log,
			"Data model of the logging component",
			IM_TAG("Log"));

I_EXPORT_COMPONENT(
			ModelBasedProcessingTrigger,
			"Model-based processing trigger",
			"Processor Model Trigger" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Processing"));


} // namespace BasePck


