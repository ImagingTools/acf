#ifndef BasePck_included
#define BasePck_included


#include "imod/IObserver.h"

#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"
#include "iser/CFileReadArchive.h"
#include "iser/CFileWriteArchive.h"

#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "idoc/CSingleDocumentTemplateComp.h"
#include "idoc/CCompositeDocumentTemplateComp.h"
#include "idoc/CDocumentManagerListenerComp.h"
#include "idoc/CSelectedDocModelBinderComp.h"
#include "idoc/CSerializedUndoManager.h"

#include "ibase/CComposedLoaderComp.h"
#include "ibase/CTextFileLoaderComp.h"
#include "ibase/CInstantiatorComp.h"
#include "ibase/CVersionInfoComp.h"
#include "ibase/CApplicationInfoComp.h"
#include "ibase/CTextDocumentComp.h"
#include "ibase/CModelProxyComp.h"
#include "ibase/CModelBinderComp.h"
#include "ibase/CAutoPersistenceComp.h"
#include "ibase/CFileTypeInfoComp.h"
#include "ibase/TFileSerializerComp.h"
#include "ibase/CConsoleLogComp.h"
#include "ibase/CTextFileLogComp.h"
#include "ibase/CFileCopyOverLoaderComp.h"
#include "ibase/CExternalFileConverterComp.h"
#include "ibase/CCopyAppComp.h"
#include "ibase/CUuidComp.h"
#include "ibase/CObjectQueueComp.h"
#include "ibase/CSelectableFileConverterComp.h"
#include "ibase/CFileConvertOverProcessorComp.h"
#include "ibase/CLogComp.h"
#include "ibase/CMultiObserverBinderComp.h"
#include "ibase/CSingletonApplicationComp.h"

#include "iprm/CSelectableParamsSetComp.h"
#include "iprm/CParamsManagerComp.h"
#include "iprm/CComposedParamsSetComp.h"
#include "iprm/CVariableParamComp.h"
#include "iprm/CFileNameParamComp.h"
#include "iprm/CEnableableParamComp.h"
#include "iprm/CSelectionParamComp.h"
#include "iprm/CManagerParamsSetShadowerComp.h"
#include "iprm/CSelectionConstraintsComp.h"
#include "iprm/CNameParamComp.h"
#include "iprm/CSelectionDelegatorComp.h"

#include "i2d/CPosition2dComp.h"
#include "i2d/CRectangleComp.h"
#include "i2d/CCircleComp.h"
#include "i2d/CAnnulusComp.h"
#include "i2d/CAnnulusSegmentComp.h"
#include "i2d/CLine2dComp.h"
#include "i2d/CQuadrangleComp.h"
#include "i2d/CPolygonComp.h"
#include "i2d/CAffineTransformation2dComp.h"
#include "i2d/CStaticCalibrationProviderComp.h"

#include "imath/CSampledFunction2d.h"

#include "iproc/CCascadedProcessorComp.h"
#include "iproc/CIterativeProcessorComp.h"
#include "iproc/CModelBasedProcessingTriggerComp.h"


/**
	Base system-undependent general package.
*/
namespace BasePck
{


typedef ibase::CFileTypeInfoComp FileTypeInfo;
typedef ibase::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> XmlFileSerializer;
typedef ibase::TFileSerializerComp<iser::CFileReadArchive, iser::CFileWriteArchive> BinaryFileSerializer;
typedef ibase::CComposedLoaderComp ComposedLoader;
typedef ibase::CTextFileLoaderComp TextFileLoader;
typedef ibase::CInstantiatorComp Instantiator;
typedef ibase::CVersionInfoComp VersionInfo;
typedef ibase::CApplicationInfoComp ApplicationInfo;
typedef ibase::CModelProxyComp ModelProxy;
typedef ibase::CModelBinderComp ModelBinder;
typedef ibase::CAutoPersistenceComp AutoPersistence;
typedef ibase::CConsoleLogComp ConsoleLog;
typedef ibase::CTextFileLogComp TextFileLog;
typedef ibase::CFileCopyOverLoaderComp FileCopyOverLoader;
typedef ibase::CExternalFileConverterComp ExternalFileConverter;
typedef ibase::CCopyAppComp CopyApp;
typedef icomp::TModelCompWrap<ibase::CUuidComp> Uuid;
typedef ibase::CObjectQueueComp ObjectQueue;
typedef icomp::TModelCompWrap<ibase::CSelectableFileConverterComp> SelectableFileConverter;
typedef ibase::CFileConvertOverProcessorComp FileConvertOverProcessor;
typedef icomp::TModelCompWrap<ibase::CLogComp> Log;
typedef ibase::CMultiObserverBinderComp MultiObserverBinder;
typedef ibase::CSingletonApplicationComp SingletonApplication;

typedef icomp::TModelCompWrap<iprm::CSelectableParamsSetComp> SelectableParamsSet;
typedef icomp::TModelCompWrap<iprm::CParamsManagerComp> ParamsManager;
typedef icomp::TModelCompWrap<iprm::CComposedParamsSetComp> ComposedParamsSet;
typedef icomp::TModelCompWrap<iprm::CVariableParamComp> VariableParam;
typedef icomp::TModelCompWrap<iprm::CFileNameParamComp> FileNameParam;
typedef icomp::TModelCompWrap<iprm::CEnableableParamComp> EnableableParam;
typedef icomp::TModelCompWrap<iprm::CSelectionParamComp> SelectionParam;
typedef iprm::CManagerParamsSetShadowerComp ManagerParamsSetShadower;
typedef iprm::CSelectionConstraintsComp SelectionConstraints;
typedef icomp::TModelCompWrap<iprm::CNameParamComp> NameParam;
typedef iprm::CSelectionDelegatorComp SelectionDelegator;

typedef idoc::CSingleDocumentTemplateComp SingleDocumentTemplate;
typedef idoc::CCompositeDocumentTemplateComp CompositeDocumentTemplate;
typedef icomp::TModelCompWrap<idoc::CDocumentManagerListenerComp> DocumentManagerListener;
typedef idoc::CSelectedDocModelBinderComp SelectedDocModelBinder;
typedef icomp::TModelCompWrap< icomp::TMakeComponentWrap<
			idoc::CSerializedUndoManager,
			idoc::IUndoManager,
			idoc::IDocumentStateComparator,
			imod::IObserver> > SerializedUndoManager;

typedef icomp::TModelCompWrap<i2d::CPosition2dComp> Position2d;
typedef icomp::TModelCompWrap<i2d::CRectangleComp> Rectangle;
typedef icomp::TModelCompWrap<i2d::CCircleComp> Circle;
typedef icomp::TModelCompWrap<i2d::CAnnulusComp> Annulus;
typedef icomp::TModelCompWrap<i2d::CAnnulusSegmentComp> AnnulusSegment;
typedef icomp::TModelCompWrap<i2d::CLine2dComp> Line;
typedef icomp::TModelCompWrap<i2d::CQuadrangleComp> Quadrangle;
typedef icomp::TModelCompWrap<i2d::CPolygonComp> Polygon;
typedef icomp::TModelCompWrap<i2d::CAffineTransformation2dComp> AffineTransformation2d;
typedef icomp::TModelCompWrap<i2d::CStaticCalibrationProviderComp> StaticCalibrationProvider;


typedef icomp::TModelCompWrap<ibase::CTextDocumentComp> TextDocument;

typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<imath::CSampledFunction2d>,
			imath::ISampledFunction2d,
			istd::IChangeable,
			imod::IModel> SampledFunction2d;

typedef iproc::CCascadedProcessorComp CascadedProcessor;
typedef iproc::CIterativeProcessorComp IterativeProcessor;
typedef iproc::CModelBasedProcessingTriggerComp ModelBasedProcessingTrigger;


} // namespace BasePck


#endif // !BasePck_included


