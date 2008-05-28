#ifndef BasePck_included
#define BasePck_included


#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"

#include "icomp/TModelCompWrap.h"

#include "isvn/TApplicationInfoWrap.h"

#include "idoc/CSingleDocumentTemplateComp.h"
#include "idoc/CCompositeDocumentTemplateComp.h"

#include "istdc/TFileSerializerComp.h"
#include "istdc/CApplicationInfoComp.h"
#include "iprm/CComposedParamsSetComp.h"
#include "istdc/CRegistryCodeSaverComp.h"

#include "icam/CFileBitmapAcquisitionParamsComp.h"


/**
	Base system-undependent general package.
*/
namespace BasePck
{


typedef istdc::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> XmlFileSerializer;
typedef isvn::TApplicationInfoWrap<istdc::CApplicationInfoComp> ApplicationInfo;
typedef icomp::TModelCompWrap<iprm::CComposedParamsSetComp> ComposedParamsSet;
typedef istdc::CRegistryCodeSaverComp RegistryCodeSaver;

typedef idoc::CSingleDocumentTemplateComp SingleDocumentTemplate;
typedef idoc::CCompositeDocumentTemplateComp CompositeDocumentTemplate;

typedef icomp::TModelCompWrap<icam::CFileBitmapAcquisitionParamsComp> FileBitmapAcquisitionParams;


} // namespace BasePck


#endif // !BasePck_included


