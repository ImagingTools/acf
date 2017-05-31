#ifndef BasePck_included
#define BasePck_included


#include <imod/IObserver.h>

#include <ilog/CConsoleLogComp.h>
#include <ilog/CLogComp.h>

#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

#include <idoc/CSingleDocumentTemplateComp.h>
#include <idoc/CCompositeDocumentTemplateComp.h>
#include <idoc/CSelectedDocModelBinderComp.h>
#include <idoc/CSerializedUndoManagerComp.h>
#include <idoc/CTextFileLoaderComp.h>
#include <idoc/CTextDocumentComp.h>
#include <idoc/CMultiPageDocumentFilePersistenceComp.h>
#include <idoc/CStandardDocumentMetaInfo.h>

#include <ibase/TModelObserverCompWrap.h>
#include <ibase/CInstantiatorComp.h>
#include <ibase/CVersionInfoComp.h>
#include <ibase/CApplicationInfoComp.h>
#include <ibase/CModelBinderComp.h>
#include <ibase/CUuidComp.h>
#include <ibase/CObjectQueueComp.h>
#include <ibase/CMultiObserverBinderComp.h>
#include <ibase/CQtVersionInfoComp.h>
#include <ibase/CObjectSynchronizerComp.h>
#include <ibase/CConsoleApplicationComp.h>

#include <iprm/CSelectableParamsSetComp.h>
#include <iprm/CParamsManagerComp.h>
#include <iprm/CComposedParamsSetComp.h>
#include <iprm/CVariableParamComp.h>
#include <iprm/CEnableableParamComp.h>
#include <iprm/CSelectionParamComp.h>
#include <iprm/CManagerParamsSetShadowerComp.h>
#include <iprm/CSelectionConstraintsComp.h>
#include <iprm/CNameParamComp.h>
#include <iprm/CMultiParamsManagerComp.h>
#include <iprm/COptionsManagerComp.h>
#include <iprm/CFilteredOptionsListComp.h>

#include <i2d/CPosition2dComp.h>
#include <i2d/CRectangleComp.h>
#include <i2d/CCircleComp.h>
#include <i2d/COrientedCircleComp.h>
#include <i2d/CAnnulusComp.h>
#include <i2d/CAnnulusSegmentComp.h>
#include <i2d/CLine2dComp.h>
#include <i2d/CQuadrangleComp.h>
#include <i2d/CPolygonComp.h>
#include <i2d/CPolylineComp.h>
#include <i2d/CTubePolylineComp.h>
#include <i2d/CArcComp.h>

#include <imath/CSampledFunction2d.h>


/**
	Base system-independent general package.
*/
namespace BasePck
{


typedef ilog::CConsoleLogComp ConsoleLog;
typedef icomp::TModelCompWrap<ilog::CLogComp> Log;

typedef ibase::CInstantiatorComp Instantiator;
typedef ibase::CVersionInfoComp VersionInfo;
typedef ibase::CApplicationInfoComp ApplicationInfo;
typedef ibase::CModelBinderComp ModelBinder;
typedef icomp::TModelCompWrap<ibase::CUuidComp> Uuid;
typedef ibase::CObjectQueueComp ObjectQueue;
typedef ibase::CMultiObserverBinderComp MultiObserverBinder;
typedef ibase::CQtVersionInfoComp QtVersionInfo;
typedef ibase::CObjectSynchronizerComp ObjectSynchronizer;
typedef ibase::CConsoleApplicationComp ConsoleApplication;

typedef icomp::TModelCompWrap<iprm::CSelectableParamsSetComp> SelectableParamsSet;
typedef icomp::TModelCompWrap<iprm::CParamsManagerComp> ParamsManager;
typedef icomp::TModelCompWrap<iprm::CComposedParamsSetComp> ComposedParamsSet;
typedef icomp::TModelCompWrap<iprm::CVariableParamComp> VariableParam;
typedef icomp::TModelCompWrap<iprm::CEnableableParamComp> EnableableParam;
typedef icomp::TModelCompWrap<iprm::CSelectionParamComp> SelectionParam;
typedef iprm::CManagerParamsSetShadowerComp ManagerParamsSetShadower;
typedef icomp::TModelCompWrap<iprm::CSelectionConstraintsComp> SelectionConstraints;
typedef icomp::TModelCompWrap<iprm::CNameParamComp> NameParam;

typedef idoc::CSingleDocumentTemplateComp SingleDocumentTemplate;
typedef idoc::CCompositeDocumentTemplateComp CompositeDocumentTemplate;
typedef idoc::CSelectedDocModelBinderComp SelectedDocModelBinder;
typedef ibase::TModelObserverCompWrap<
			icomp::TModelCompWrap<idoc::CSerializedUndoManagerComp> > SerializedUndoManager;
typedef idoc::CTextFileLoaderComp TextFileLoader;
typedef icomp::TModelCompWrap<idoc::CTextDocumentComp> TextDocument;
typedef idoc::CMultiPageDocumentFilePersistenceComp MultiPageDocumentFilePersistence;
typedef icomp::TModelCompWrap<icomp::TMakeComponentWrap<
			idoc::CStandardDocumentMetaInfo, idoc::IDocumentMetaInfo, iser::ISerializable> > StandardDocumentMetaInfo;

typedef icomp::TModelCompWrap<i2d::CPosition2dComp> Position2d;
typedef icomp::TModelCompWrap<i2d::CRectangleComp> Rectangle;
typedef icomp::TModelCompWrap<i2d::CCircleComp> Circle;
typedef icomp::TModelCompWrap<i2d::COrientedCircleComp> OrientedCircle;
typedef icomp::TModelCompWrap<i2d::CAnnulusComp> Annulus;
typedef icomp::TModelCompWrap<i2d::CAnnulusSegmentComp> AnnulusSegment;
typedef icomp::TModelCompWrap<i2d::CLine2dComp> Line;
typedef icomp::TModelCompWrap<i2d::CQuadrangleComp> Quadrangle;
typedef icomp::TModelCompWrap<i2d::CPolygonComp> Polygon;
typedef icomp::TModelCompWrap<i2d::CPolylineComp> Polyline;
typedef icomp::TModelCompWrap<i2d::CTubePolylineComp> TubePolyline;
typedef icomp::TModelCompWrap<i2d::CArcComp> Arc;

typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<imath::CSampledFunction2d>,
			imath::ISampledFunction2d,
			istd::IChangeable,
			imod::IModel> SampledFunction2d;

typedef icomp::TModelCompWrap<iprm::CMultiParamsManagerComp> MultiParamsManager;
typedef icomp::TModelCompWrap<iprm::COptionsManagerComp> OptionsManager;
typedef icomp::TModelCompWrap<iprm::CFilteredOptionsListComp> FilteredOptionsList;


} // namespace BasePck


#endif // !BasePck_included


