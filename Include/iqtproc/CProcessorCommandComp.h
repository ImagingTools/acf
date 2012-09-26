#ifndef iqtproc_CProcessorCommandComp_included
#define iqtproc_CProcessorCommandComp_included



// ACF includes
#include "imod/TSingleModelObserverBase.h"

#include "icomp/CComponentBase.h"

#include "ibase/ICommandsProvider.h"

#include "iqtgui/IIconProvider.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "iproc/IProcessor.h"


namespace iqtproc
{


/**
	Component for binding of a menu command (action) with some processor.
*/
class CProcessorCommandComp:
			public QObject,
			public icomp::CComponentBase,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT
public:
	typedef icomp::CComponentBase BaseClass;
	
	I_BEGIN_COMPONENT(CProcessorCommandComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_processorCompPtr, "ActionProcessor", "Processor used for action execution", true, "ActionProcessor");
		I_ASSIGN(m_processorParamsCompPtr, "ProcessingParams", "Parameter set used for processor execution", false, "ProcessingParams");
		I_ASSIGN(m_processorInputCompPtr, "ProcessingInput", "Input object used for processor execution", false, "ProcessingInput");
		I_ASSIGN(m_processorOutputCompPtr, "ProcessingOutput", "Output object used for processor execution", false, "ProcessingOutput");
		I_ASSIGN(m_actionIconAttrPtr, "ActionIcon", "Icon used command menu", true, ":/Icons");
		I_ASSIGN(m_menuNameAttrPtr, "MenuName", "Name of the menu for the action group", true, "MenuName");
		I_ASSIGN(m_menuDescriptionAttrPtr, "MenuDescription", "Description for the action group", true, "MenuDescription");
		I_ASSIGN(m_rootMenuNameAttrPtr, "RootMenu", "Name of the root command", true, "RootMenu");
	I_END_COMPONENT;

	CProcessorCommandComp();

	// reimpemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

	// reimpemented (icomp::IComponent)
	virtual void OnComponentCreated();

private Q_SLOTS:
	void OnCommandActivated();

protected:
	I_REF(iproc::IProcessor, m_processorCompPtr);
	I_REF(iprm::IParamsSet, m_processorParamsCompPtr);
	I_REF(istd::IPolymorphic, m_processorInputCompPtr);
	I_REF(istd::IChangeable, m_processorOutputCompPtr);
	I_ATTR(QByteArray, m_actionIconAttrPtr);
	I_ATTR(QString, m_menuNameAttrPtr);
	I_ATTR(QString, m_menuDescriptionAttrPtr);
	I_ATTR(QString, m_rootMenuNameAttrPtr);

	iqtgui::CHierarchicalCommand m_rootMenuCommand;
	iqtgui::CHierarchicalCommand m_mainMenuCommand;
	iqtgui::CHierarchicalCommand m_processingCommand;
};


} // namespace iqtproc


#endif // !iqtproc_CProcessorCommandComp_included


