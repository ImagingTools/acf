#ifndef iloggui_CLogGuiComp_included
#define iloggui_CLogGuiComp_included


// Qt includes
#include <QtCore/QTimer>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QAction>
#else
#include <QtGui/QAction>
#endif

// ACF includes
#include "ifile/IFilePersistence.h"
#include "ilog/CMessageContainer.h"
#include "ilog/CMessage.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "GeneratedFiles/iloggui/ui_CLogGuiComp.h"


namespace iloggui
{


/**
	Message container displaying messages as log list.
*/
class CLogGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CLogGuiComp>,
			public ilog::CMessageContainer
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CLogGuiComp> BaseClass;
	typedef ilog::CMessageContainer BaseClass2;

	I_BEGIN_COMPONENT(CLogGuiComp);
		I_REGISTER_INTERFACE(ilog::IMessageConsumer);
		I_ASSIGN(m_fileLoaderCompPtr, "Exporter", "File loader used for log export", false, "Exporter");
		I_ASSIGN(m_slaveMessageConsumerCompPtr, "SlaveMessageConsumer", "Slave message consumer", false, "SlaveMessageConsumer");
		I_ASSIGN_TO(m_slaveMessageContainerCompPtr, m_slaveMessageConsumerCompPtr, false);
		I_ASSIGN(m_allowDiagnosticMessagesAttrPtr, "AllowDiagnosticMessages", "If enabled the collecting of diagnostic messages and display of corresponding GUI elements is allowed", true, false);
		I_ASSIGN(m_maxMessagesCountAttrPtr, "MaxMessageCount", "Maximal number of messages supported by the log", true, 1000);
		I_ASSIGN(m_defaultModeAttrPtr, "DefaultMode", "Default display mode,\n 0 - info,\n 1 - warning,\n 2 - error", true, 0);
		I_ASSIGN(m_showLogDescriptionAttrPtr, "ShowLogDescription", "Sets the log tables description visible", true, false);
		I_ASSIGN(m_showMessageTextFilterAttrPtr, "ShowMessageTextFilter", "If enabled, the text filter for the messages will be shown", true, true);
		I_ASSIGN(m_logTimeFormatAttrPtr, "TimeFormat", "Format of the date/time used for displaing message's time stamp", true, "");
	I_END_COMPONENT;

	CLogGuiComp();

protected:
	enum ColumnType
	{
		CT_ICON,
		CT_TIME,
		CT_SOURCE,
		CT_MESSAGE
	};

	enum MessageMode
	{
		MM_INFO = istd::IInformationProvider::IC_NONE,
		MM_WARNING = istd::IInformationProvider::IC_WARNING,
		MM_ERROR = istd::IInformationProvider::IC_ERROR,
		MM_ALL = istd::IInformationProvider::IC_CRITICAL
	};

	enum ActionType
	{
		AT_CLEAR,
		AT_EXPORT,
		AT_DIAGNOSTICS
	};

	enum DataRole
	{
		DR_CATEGORY = Qt::UserRole
	};

	/**
		Create GUI item corresponding to specified message.
	*/
	virtual QTreeWidgetItem* CreateGuiItem(const istd::IInformationProvider& message);

	/**
		Get icons corresponding to specified information category.
	*/
	virtual QIcon GetCategoryIcon(int category) const;

	/**
		Get icons corresponding to specified log action.
	*/
	virtual QIcon GetActionIcon(int functionType) const;

	/**
		Get text corresponding to specified information category.
	*/
	virtual QString GetCategoryText(int category) const;

	// reimplemented (ilog::IMessageConsumer)
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const;
	virtual void AddMessage(const MessagePtr& messagePtr);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	void UpdateVisualStatus();
	void UpdateItemVisibility(QTreeWidgetItem* itemPtr, const QString& filterText) const;
	void GenerateMessageList();
	void AddMessageToList(const MessagePtr& messagePtr);

protected Q_SLOTS:
	virtual void OnAddMessage(const MessagePtr& messagePtr);
	virtual void OnMessageModeChanged();

	virtual void OnClearAction();
	virtual void OnExportAction();

	void OnRemoveMessagesTimer();

	void on_FilterText_textChanged(const QString& filterText);

Q_SIGNALS:
	void EmitAddMessage(const MessagePtr& messagePtr);

protected:
	QAction* m_infoActionPtr;
	QAction* m_warningActionPtr;
	QAction* m_errorActionPtr;
	QAction* m_clearActionPtr;
	QAction* m_exportActionPtr;
	QAction* m_diagnosticModeActionPtr;

private:
	I_REF(ifile::IFilePersistence, m_fileLoaderCompPtr);
	I_REF(ilog::IMessageConsumer, m_slaveMessageConsumerCompPtr);
	I_REF(ilog::IHierarchicalMessageContainer, m_slaveMessageContainerCompPtr);
	I_ATTR(bool, m_allowDiagnosticMessagesAttrPtr);
	I_ATTR(int, m_defaultModeAttrPtr);
	I_ATTR(int, m_maxMessagesCountAttrPtr);
	I_ATTR(bool, m_showLogDescriptionAttrPtr);
	I_ATTR(bool, m_showMessageTextFilterAttrPtr);
	I_ATTR(QString, m_logTimeFormatAttrPtr);

	int m_currentMessageMode;

	int m_statusCategory;

	QTimer m_removeMessagesTimer;
};


} // namespace iloggui


#endif // !iloggui_CLogGuiComp_included

