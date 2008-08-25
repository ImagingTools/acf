#ifndef BasePck_included
#define BasePck_included


#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"

#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "isvn/TApplicationInfoWrap.h"

#include "idoc/CSingleDocumentTemplateComp.h"
#include "idoc/CCompositeDocumentTemplateComp.h"

#include "istdc/TFileSerializerComp.h"
#include "istdc/CApplicationInfoComp.h"
#include "istdc/CRegistryCodeSaverComp.h"

#include "i2d/CPosition2d.h"
#include "i2d/CCircle.h"
#include "i2d/CLine2d.h"
#include "i2d/CRectangle.h"
#include "i2d/CAnnulus.h"

#include "iprm/CSelectableParamsSetComp.h"
#include "iprm/CParamsManagerComp.h"
#include "iprm/CComposedParamsSetComp.h"

#include "isig/CSamplingParamsComp.h"

#include "iproc/CTestIdManagerComp.h"
#include "iproc/CCascadedProcessorComp.h"

#include "icam/CFileBitmapAcquisitionParamsComp.h"
#include "icam/CExposureParamsComp.h"
#include "icam/CSnapBitmapSupplierComp.h"

#include "ibase/CRectangleComp.h"


/**
	Base system-undependent general package.
*/
namespace BasePck
{


typedef istdc::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> XmlFileSerializer;
typedef isvn::TApplicationInfoWrap<istdc::CApplicationInfoComp> ApplicationInfo;
typedef istdc::CRegistryCodeSaverComp RegistryCodeSaver;

typedef icomp::TModelCompWrap<iprm::CSelectableParamsSetComp> SelectableParamsSet;
typedef icomp::TModelCompWrap<iprm::CParamsManagerComp> ParamsManager;
typedef icomp::TModelCompWrap<iprm::CComposedParamsSetComp> ComposedParamsSet;

typedef idoc::CSingleDocumentTemplateComp SingleDocumentTemplate;
typedef idoc::CCompositeDocumentTemplateComp CompositeDocumentTemplate;

typedef icomp::TModelCompWrap<isig::CSamplingParamsComp> SamplingParams;

typedef icomp::TModelCompWrap<icam::CFileBitmapAcquisitionParamsComp> FileBitmapAcquisitionParams;
typedef icomp::TModelCompWrap<icam::CExposureParamsComp> ExposureParams;
typedef icomp::TModelCompWrap<icam::CSnapBitmapSupplierComp> SnapBitmapSupplier;

typedef iproc::CTestIdManagerComp TestIdManager;
typedef iproc::CCascadedProcessorComp CascadedProcessor;

typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<i2d::CPosition2d>,
			i2d::CPosition2d,
			i2d::IObject2d,
			iser::ISerializable,
			imod::IModel> Position2d;

typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<i2d::CCircle>,
			i2d::CCircle,
			i2d::CPosition2d,
			i2d::IObject2d,
			iser::ISerializable,
			imod::IModel> Circle;

typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<i2d::CLine2d>,
			i2d::CLine2d,
			i2d::IObject2d,
			iser::ISerializable,
			imod::IModel> Line;

typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<i2d::CRectangle>,
			i2d::CRectangle,
			i2d::IObject2d,
			iser::ISerializable,
			imod::IModel> Rectangle;

typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<i2d::CAnnulus>,
			i2d::CAnnulus,
			i2d::IObject2d,
			iser::ISerializable,
			imod::IModel> Annulus;

typedef icomp::TModelCompWrap<ibase::CRectangleComp> AttributedRectangle;

} // namespace BasePck


#endif // !BasePck_included


