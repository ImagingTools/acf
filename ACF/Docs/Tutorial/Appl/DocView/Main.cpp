#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"

#include "imod/TModelWrap.h"

#include "icomp/TSimComponentWrap.h"
#include "icomp/TSimComponentsFactory.h"

#include "idoc/CSerializedDocumentTemplateComp.h"

#include "istdc/TSingleFactoryComp.h"
#include "istdc/TFileSerializerComp.h"

#include "iqt/CApplicationComp.h"

#include "iqmain/CMainWindowGuiComp.h"
#include "iqmain/CMultiDocumentWorkspaceGuiComp.h"

#include "CTextEditorComp.h"
#include "CTextModelComp.h"


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqmain);

	// model factory:
	icomp::TSimComponentsFactory<imod::TModelWrap<CTextModelComp> > modelFactoryComp;
	modelFactoryComp.SetStringAttr("DefaultText", "Ahoj przygodo!");

	icomp::TSimComponentsFactory<CTextEditorComp> viewFactoryComp;

	typedef istdc::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> SerializerComp;
	icomp::TSimComponentWrap<SerializerComp> serializerComp;
	serializerComp.InitComponent();

	icomp::TSimComponentWrap<idoc::CSerializedDocumentTemplateComp> documentTemplateComp;
	documentTemplateComp.SetFactory("DocumentFactory", &modelFactoryComp);
	documentTemplateComp.SetFactory("ViewFactory", &viewFactoryComp);
	documentTemplateComp.SetRef("DocumentSerializer", &serializerComp);
	documentTemplateComp.InitComponent();

	icomp::TSimComponentWrap<imod::TModelWrap<iqmain::CMultiDocumentWorkspaceGuiComp> > workspaceComp;
	workspaceComp.SetRef("DocumentTemplate", &documentTemplateComp);
	workspaceComp.InitComponent();

	icomp::TSimComponentWrap<iqmain::CMainWindowGuiComp> mainWindowComp;
	mainWindowComp.SetRef("Workspace", &workspaceComp);
	mainWindowComp.SetRef("DocumentManager", &workspaceComp);
	mainWindowComp.InitComponent();

	icomp::TSimComponentWrap<iqt::CApplicationComp> application;
	application.SetRef("MainGui", &mainWindowComp);
	application.InitComponent();

	return application.Execute(argc, argv);
}

