#ifndef iqtgui_CLogGuiComp_included
#define iqtgui_CLogGuiComp_included


// Qt includes
#include <QtCore/QTimer>
#include <QtGui/QAction>


// ACF includes
#include "iser/IFileLoader.h"

#include "ibase/CMessageContainer.h"
#include "ibase/CMessage.h"

#include "iqtgui/IVisualStatusProvider.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtgui/Generated/ui_CLogGuiComp.h"


namespace iqtgui
{


/**
	Message container displaying messages as log list.
*/
class CLogGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CLogGuiComp>,
			public ibase::CMessageContainer
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CLogGuiComp> BaseClass;
	typedef ibase::CMessageContainer BaseClass2;

	I_BEGIN_COMPONENT(CLogGuiComp);
		I_REGISTER_INTERFACE(ibase::IMessageConsumer);
		I_ASSIGN(m_fileLoaderCompPtr, "Exporter", "File loader used for log export", false, "Exporter");
		I_ASSIGN(m_maxMessagesCountAttrPtr, "MaxMessageCount", "Maximal number of messages supported by the log", true, 1000);
		I_ASSIGN(m_defaultModeAttrPtr, "DefaultMode", "Default display mode,\n 0 - info,\n 1 - warning,\n 2 - error", true, 0);
		I_ASSIGN(m_showLogDescriptionAttrPtr, "ShowLogDescription", "Sets the log tables description visible", false, false);
		I_ASSIGN(m_showMessageTextFilterAttrPtr, "ShowMessageTextFilter", "If enabled, the text filter for the messages will be shown", true, true);
		I_ASSIGN(m_logTimeFormatAttrPtr, "TimeFormat", "Format of the date/time used for displaing message's time stamp", true, "");
		I_ASSIGN(m_slaveMessageConsumerCompPtr, "SlaveMessageConsumer", "Slave message consumer", false, "SlaveMessageConsumer");
		I_ASSIGN_TO(m_slaveMessageContainerCompPtr, m_slaveMessageConsumerCompPtr, false);
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
		MM_INFO = istd::IInformationProvider::IC_INFO,
		MM_WARNING = istd::IInformationProvider::IC_WARNING,
		MM_ERROR = istd::IInformationProvider::IC_ERROR,
		MM_ALL = istd::IInformationProvider::IC_CRITICAL
	};

	enum DataRole
	{
		DR_MESSAGE_ID = Qt::UserRole,
		DR_CATEGORY
	};

	/**
		Create GUI item corresponding to specified message.
	*/
	virtual QTreeWidgetItem* CreateGuiItem(const istd::IInformationProvider& message);

	/**
		Get icons corresponding to specified information category.
	*/
	QIcon GetCategoryIcon(int category) const;
	/**
		Get text corresponding to specified information category.
	*/
	QString GetCategoryText(int category) const;

	// reimplemented (ibase::IMessageConsumer)
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
	void on_FilterClearButton_clicked();

Q_SIGNALS:
	void EmitAddMessage(const MessagePtr& messagePtr);

protected:
	QAction* m_infoAction;
	QAction* m_warningAction;
	QAction* m_errorAction;
	QAction* m_clearAction;
	QAction* m_exportAction;

private:
	I_REF(iser::IFileLoader, m_fileLoaderCompPtr);
	I_REF(ibase::IMessageConsumer, m_slaveMessageConsumerCompPtr);
	I_REF(ibase::IHierarchicalMessageContainer, m_slaveMessageContainerCompPtr);
	I_ATTR(int, m_defaultModeAttrPtr);
	I_ATTR(int, m_maxMessagesCountAttrPtr);
	I_ATTR(bool, m_showLogDescriptionAttrPtr);
	I_ATTR(bool, m_showMessageTextFilterAttrPtr);
	I_ATTR(QString, m_logTimeFormatAttrPtr);

	int m_currentMessageMode;

	int m_statusCategory;

	QTimer m_removeMessagesTimer;
};


} // namespace iqtgui


#endif // !iqtgui_CLogGuiComp_included

