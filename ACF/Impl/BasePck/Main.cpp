#include "icomp/export.h"

#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"

#include "idoc/CSerializedDocumentTemplateComp.h"

#include "istdc/TFileSerializerComp.h"


I_EXPORT_PACKAGE("Base", "Base system-undependent general package", "Undependend Standard Base");
typedef istdc::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> CXmlFileSerializer;
I_EXPORT_COMPONENT(CXmlFileSerializer, XmlFileSerializer, "Simple file serializer using XML format", "File Serializer Archive XML Format");
I_EXPORT_COMPONENT(idoc::CSerializedDocumentTemplateComp, SerializedDocumentTemplate, "Simple file serializer using XML format", "File Serializer Archive XML Format");


