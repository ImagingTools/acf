#include "BasePck.h"


#include <icomp/export.h>


namespace BasePck
{


I_EXPORT_PACKAGE(
			"Acf/Base",
			"Base system-undependent general package",
			IM_PROJECT("ACF") IM_TAG("Basic") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

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
			ModelBinder,
			"Model binder",
			"Observer Bind Composition Connection" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Composition"));

I_EXPORT_COMPONENT(
			ConsoleLog,
			"Simple log on windows console",
			"Windows Message Consumer" IM_CATEGORY(I_SERVICE) IM_TAG("Log"));

I_EXPORT_COMPONENT(
			TextFileLog,
			"Simple text file log",
			"Message Consumer" IM_CATEGORY(I_SERVICE) IM_TAG("Text File Log"));

I_EXPORT_COMPONENT(
			TextFileLogStreamer,
			"Simple text file log serializer",
			"Log Text Write Serialize" IM_CATEGORY(I_SERVICE) IM_TAG("Text File Log Write"));

I_EXPORT_COMPONENT(
			Uuid,
			"Unique identifier",
			"Unique Identifier UUID Generated" IM_CATEGORY(I_SERVICE));

I_EXPORT_COMPONENT(
			ObjectQueue,
			"Queue of any objects",
			"Objects Queue List FIFO Stack Elements Serial Delay Line" IM_CATEGORY(I_SERVICE));

I_EXPORT_COMPONENT(
			Log,
			"Data model of the logging component",
			IM_TAG("Log"));

I_EXPORT_COMPONENT(
			MultiObserverBinder,
			"Simple binder of few observers of the same model",
			"Multi Model Observer Binder" IM_TAG("Model") IM_AUTHOR("\"Krzysztof Gantzke\""));

I_EXPORT_COMPONENT(
			QtVersionInfo,
			"Version information about underlaying Qt framework",
			"Application Version Qt" IM_TAG("Application"));

I_EXPORT_COMPONENT(
			ObjectSynchronizer,
			"Data object synchronization",
			"Synchronization Data Object" IM_TAG("Model"));

I_EXPORT_COMPONENT(
			ConsoleApplication,
			"Console application",
			"Application Service" IM_CATEGORY(I_APPLICATION));

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
			EnableableParam,
			"Parameter with boolean flag",
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
			SelectedDocModelBinder,
			"Connect model with observer if document is active",
			"Multiple Model Observer Composite Template MVC" IM_CATEGORY(I_APPLICATION_LOGIC) IM_TAG("Document"));

I_EXPORT_COMPONENT(
			SerializedUndoManager,
			"Provide UNDO manager based on complete object serialization",
			"UNDO REDO Manager Persistence Serialization" IM_TAG("Model Document"));

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
			OrientedCircle,
			"Circle 2D object with orientation (inside or outside)",
			"Circle Position Radius 2D Object Oriented Inside Outside Direction Parameter" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

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
			Polygon,
			"Polygon 2D object",
			"Polygon2D Object Parameter" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

I_EXPORT_COMPONENT(
			Polyline,
			"Polyline 2D object",
			"Polyline2D Object Parameter" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

I_EXPORT_COMPONENT(
			AffineCalibration2d,
			"Affine calibration",
			"Affine Calibration Tranformation" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

I_EXPORT_COMPONENT(
			PerspectiveCalibration2d,
			"Perspectivic calibration",
			"Perspective Calibration Tranformation" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

I_EXPORT_COMPONENT(
			StaticCalibrationProvider,
			"Provider of a static calibration object",
			"2D Tranformation Calibration" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D"));

I_EXPORT_COMPONENT(
			TubePolyline,
			"Tube representation",
			"2D Tube Polyline Region" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D"));

I_EXPORT_COMPONENT(
			Calibration2dProxy,
			"Proxy of a 2D calibration using calibration provider",
			"2D Tranformation Calibration Proxy Provider" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D"));

I_EXPORT_COMPONENT(
			Arc,
			"Arc 2D object",
			"Arc Circle Position Radius 2D Object Parameter" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

I_EXPORT_COMPONENT(
			TextDocument,
			"Simple text document",
			IM_CATEGORY(I_DATA_MODEL) IM_TAG("Text Document"));

I_EXPORT_COMPONENT(
			MultiPageDocumentFilePersistence,
			"Simple text document",
			IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("Document Page"));

I_EXPORT_COMPONENT(
			StandardDocumentMetaInfo,
			"Standard document meta-information",
			IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("Document Meta Information Tag Description"));

I_EXPORT_COMPONENT(
			SampledFunction2d,
			"Sampled function with 2-dimensional input",
			"Function Sample" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Sampling Model"));

I_EXPORT_COMPONENT(
			MultiParamsManager,
			"Manager of multi parameters set",
			"Parameters Set Multi Manager" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Composition Parameter"));

I_EXPORT_COMPONENT(
			OptionsManager,
			"Simple manager for a list of options",
			"Manager Option Selection" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model"));

I_EXPORT_COMPONENT(
			FilteredOptionsList,
			"Filtered list of options",
			"Option Options List Filter" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model"));


} // namespace BasePck


