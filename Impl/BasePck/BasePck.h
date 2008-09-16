#ifndef BasePck_included
#define BasePck_included


#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"

#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "isvn/TApplicationInfoWrap.h"

#include "idoc/CSingleDocumentTemplateComp.h"
#include "idoc/CCompositeDocumentTemplateComp.h"

#include "ibase/TFileSerializerComp.h"
#include "ibase/CApplicationInfoComp.h"
#include "ibase/CRegistryCodeSaverComp.h"
#include "ibase/CRectangleComp.h"

#include "iprm/CSelectableParamsSetComp.h"
#include "iprm/CParamsManagerComp.h"
#include "iprm/CComposedParamsSetComp.h"
#include "iprm/CFileNameParamComp.h"

#include "i2d/CPosition2d.h"
#include "i2d/CCircle.h"
#include "i2d/CLine2d.h"
#include "i2d/CRectangle.h"
#include "i2d/CAnnulus.h"
#include "i2d/CAnnulusSegment.h"


/**
	Base system-undependent general package.
*/
namespace BasePck
{


typedef ibase::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> XmlFileSerializer;
typedef isvn::TApplicationInfoWrap<ibase::CApplicationInfoComp> ApplicationInfo;
typedef ibase::CRegistryCodeSaverComp RegistryCodeSaver;

typedef icomp::TModelCompWrap<iprm::CSelectableParamsSetComp> SelectableParamsSet;
typedef icomp::TModelCompWrap<iprm::CParamsManagerComp> ParamsManager;
typedef icomp::TModelCompWrap<iprm::CComposedParamsSetComp> ComposedParamsSet;
typedef icomp::TModelCompWrap<iprm::CFileNameParamComp> FileNameParam;

typedef idoc::CSingleDocumentTemplateComp SingleDocumentTemplate;
typedef idoc::CCompositeDocumentTemplateComp CompositeDocumentTemplate;
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

typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<i2d::CAnnulusSegment>,
			i2d::CAnnulus,
			i2d::IObject2d,
			iser::ISerializable,
			imod::IModel> AnnulusSegment;

typedef icomp::TModelCompWrap<ibase::CRectangleComp> AttributedRectangle;

} // namespace BasePck


#endif // !BasePck_included


