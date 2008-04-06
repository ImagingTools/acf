#include "imod/TModelWrap.h"

#include "iser/ISerializable.h"

#include "icomp/TSimComponentWrap.h"

#include "iqt/CApplicationComp.h"

#include "iqmain/CMainWindowGuiComp.h"
#include "iqmain/CMultiDocumentWorkspaceGuiComp.h"

#include "istdc/TSingleFactoryComp.h"

#include "idoc/TDocumentTemplateCompWrap.h"
#include "idoc/TDocumentFactoryComp.h"
#include "idoc/CSerializableDocument.h"
#include "idoc/CMultiDocumentTemplate.h"

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

	// document factory:
	typedef idoc::TDocumentFactoryComp<idoc::CSerializableDocument> DocumentFactoryComp;
	icomp::TSimComponentWrap<DocumentFactoryComp> documentFactoryComp;
	documentFactoryComp.SetRef("ModelFactory", &modelFactoryComp);
	documentFactoryComp.InitComponent();

	typedef idoc::TDocumentTemplateCompWrap<idoc::CMultiDocumentTemplate> DocumentTemplateComp;
	icomp::TSimComponentWrap<DocumentTemplateComp> documentTemplateComp;
	documentTemplateComp.SetRef("DocumentFactory", &documentFactoryComp);
	documentTemplateComp.SetRef("ViewFactory", &viewFactoryComp);
	documentTemplateComp.InitComponent();

	icomp::TSimComponentWrap<imod::TModelWrap<iqmain::CMultiDocumentWorkspaceGuiComp> > workspaceComp;
	workspaceComp.InsertMultiRef("DocumentTemplates", &documentTemplateComp);
	workspaceComp.InsertMultiAttr<istd::CString>("DocumentIds", "Registry");

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

