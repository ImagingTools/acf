#ifndef iqtproc_CDocumentProcessingManagerComp_included
#define iqtproc_CDocumentProcessingManagerComp_included


// Qt includes
#include <QObject>


// ACF includes
#include "ibase/TLoggerCompWrap.h"
#include "ibase/ICommandsProvider.h"

#include "idoc/IDocumentManager.h"
#include "idoc/IDocumentTemplate.h"

#include "iproc/IProcessor.h"

#include "iqtgui/IGuiObject.h"
#include "iqtgui/CHierarchicalCommand.h"


namespace iqtproc
{


class CDocumentProcessingManagerComp:
			public QObject,
			public ibase::CLoggerComponentBase,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	
	I_BEGIN_COMPONENT(CDocumentProcessingManagerComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_documentManagerCompPtr, "DocumentManager", "Application's document manager", true, "DocumentManager")
		I_ASSIGN(m_processorCompPtr, "Processor", "Target processing component", true, "Processor");
		I_ASSIGN(m_progressManagerCompPtr, "ProgressManager", "Processing progress manager", false, "ProgressManager");
		I_ASSIGN(m_paramsSetCompPtr, "ProcessingParamsSet", "Processing parameters", false, "ProcessingParameters");
		I_ASSIGN(m_paramsGuiCompPtr, "ProcessingParamsGui", "UI for processing parameters", false, "ProcessingParamsGui");
		I_ASSIGN(m_commandNameAttrPtr, "ProcessingCommandName", "Name of the processing action in menu", true, "");
		I_ASSIGN(m_menuNameAttrPtr, "ProcessingMenuName", "Name of the processing menu", true, "Processing");
		I_ASSIGN(m_inPlaceProcessingAttrPtr, "InPlaceProcessing", "If enabled, the input document will be the result of processing", false, false);
	I_END_COMPONENT;

	CDocumentProcessingManagerComp();

	// reimpemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

	// reimpemented (icomp::IComponent)
	virtual void OnComponentCreated();

private Q_SLOTS:
	void OnDoProcessing();

private:
	void DoDocumentProcessing(const istd::IChangeable& inputDocument, const std::string& documentTypeId);
	void DoInPlaceDocumentProcessing(istd::IChangeable& inputDocument);

private:
	I_REF(idoc::IDocumentManager, m_documentManagerCompPtr);
	I_REF(iproc::IProcessor, m_processorCompPtr);
	I_REF(iproc::IProgressManager, m_progressManagerCompPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(iqtgui::IGuiObject, m_paramsGuiCompPtr);
	I_ATTR(istd::CString, m_commandNameAttrPtr);
	I_ATTR(istd::CString, m_menuNameAttrPtr);
	I_ATTR(bool, m_inPlaceProcessingAttrPtr);

	iqtgui::CHierarchicalCommand m_processingMenu;
	iqtgui::CHierarchicalCommand m_rootCommands;
	iqtgui::CHierarchicalCommand m_processingCommand;
};


} // namespace iproc


#endif // !iqtproc_CDocumentProcessingManagerComp_included
