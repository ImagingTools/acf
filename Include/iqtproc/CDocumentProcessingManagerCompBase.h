#ifndef iqtproc_CDocumentProcessingManagerCompBase_included
#define iqtproc_CDocumentProcessingManagerCompBase_included


// Qt includes
#include <QObject>


// ACF includes
#include "imod/TModelDispatcher.h"

#include "ibase/TLoggerCompWrap.h"
#include "ibase/ICommandsProvider.h"

#include "idoc/IDocumentManager.h"

#include "iproc/IProcessor.h"

#include "iqtgui/IGuiObject.h"
#include "iqtgui/CHierarchicalCommand.h"


namespace iqtproc
{


class CDocumentProcessingManagerCompBase:
			public QObject,
			public ibase::CLoggerComponentBase,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	
	I_BEGIN_BASE_COMPONENT(CDocumentProcessingManagerCompBase);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_documentManagerCompPtr, "DocumentManager", "Application's document manager", true, "DocumentManager")
		I_ASSIGN(m_processorCompPtr, "Processor", "Target processing component", true, "Processor");
		I_ASSIGN(m_progressManagerCompPtr, "ProgressManager", "Processing progress manager", false, "ProgressManager");
		I_ASSIGN(m_paramsSetCompPtr, "ProcessingParamsSet", "Processing parameters", false, "ProcessingParameters");
		I_ASSIGN(m_paramsGuiCompPtr, "ProcessingParamsGui", "UI for processing parameters", false, "ProcessingParamsGui");
		I_ASSIGN(m_commandNameAttrPtr, "ProcessingCommandName", "Name of the processing action in menu", true, "");
		I_ASSIGN(m_menuNameAttrPtr, "ProcessingMenuName", "Name of the processing menu", true, "Processing");
		I_ASSIGN(m_inputDocumentRequiredAttrPtr, "InputDocumentRequired", "If enabled, the input document is required for processing", false, true);
	I_END_COMPONENT;

	CDocumentProcessingManagerCompBase();

	void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

	/**
		Return \c true, if the input document is required for processing.
	*/
	bool IsInputDocumentRequired() const;

	/**
		Enable or disable processing command.
	*/
	void SetProcessingCommandEnabled(bool isProcessingCommandEnabled = true);

	// reimpemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

	// reimpemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// abstract methods

	/**
		Process the current document.
	*/
	virtual void DoDocumentProcessing(const istd::IChangeable* inputDocumentPtr, const std::string& documentTypeId) = 0;

private Q_SLOTS:
	void OnDoProcessing();

protected:
	I_REF(idoc::IDocumentManager, m_documentManagerCompPtr);
	I_REF(iproc::IProcessor, m_processorCompPtr);
	I_REF(iproc::IProgressManager, m_progressManagerCompPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(iqtgui::IGuiObject, m_paramsGuiCompPtr);
	I_ATTR(istd::CString, m_commandNameAttrPtr);
	I_ATTR(istd::CString, m_menuNameAttrPtr);
	I_ATTR(bool, m_inputDocumentRequiredAttrPtr);

	iqtgui::CHierarchicalCommand m_processingMenu;
	iqtgui::CHierarchicalCommand m_rootCommands;
	iqtgui::CHierarchicalCommand m_processingCommand;

private:
	imod::TModelDispatcher<CDocumentProcessingManagerCompBase> m_documentManagerObserver;
};


} // namespace iproc


#endif // !iqtproc_CDocumentProcessingManagerCompBase_included


