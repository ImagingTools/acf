#include "imod/TModelWrap.h"

#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"

#include "icomp/TSimComponentWrap.h"

#include "idoc/CSerializedDocumentTemplateComp.h"

#include "iqt/CApplicationComp.h"

#include "iqmain/CMainWindowGuiComp.h"
#include "iqmain/CMultiDocumentWorkspaceGuiComp.h"

#include "istdc/TSingleFactoryComp.h"
#include "istdc/TFileSerializerComp.h"

#include "CTextEditor.h"
#include "CTextModel.h"


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqmain);

	// model factory:
	typedef istdc::TSingleFactoryComp<imod::IModel, imod::TModelWrap<CTextModel> > ModelFactoryComp;
	icomp::TSimComponentWrap<ModelFactoryComp> modelFactoryComp;
	modelFactoryComp.InitComponent();

	// view factory:
	typedef istdc::TSingleFactoryComp<imod::IObserver, CTextEditor> ViewFactoryComp;
	icomp::TSimComponentWrap<ViewFactoryComp> viewFactoryComp;
	viewFactoryComp.InitComponent();

	typedef istdc::TFileSerializerComp<iser::CXmlFileReadArchive, iser::CXmlFileWriteArchive> SerializerComp;
	icomp::TSimComponentWrap<SerializerComp> serializerComp;
	serializerComp.InitComponent();

	icomp::TSimComponentWrap<idoc::CSerializedDocumentTemplateComp> documentTemplateComp;
	documentTemplateComp.SetRef("DocumentFactory", &modelFactoryComp);
	documentTemplateComp.SetRef("ViewFactory", &viewFactoryComp);
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

