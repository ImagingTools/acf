#ifndef iqtgui_CLogGuiComp_included
#define iqtgui_CLogGuiComp_included


// Qt includes
#include <QtCore/QTimer>
#include <QtGui/QAction>


// ACF includes
#include "iser/IFileLoader.h"

#include "ibase/IMessageContainer.h"
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
			public iqtgui::TDesignerGuiObserverCompBase<Ui::CLogGuiComp, ibase::IMessageContainer>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CLogGuiComp, ibase::IMessageContainer> BaseClass;

	I_BEGIN_COMPONENT(CLogGuiComp);
		I_ASSIGN(m_fileLoaderCompPtr, "Exporter", "File loader used for log export", false, "Exporter");
		I_ASSIGN(m_defaultModeAttrPtr, "DefaultMode", "Default display mode,\n 0 - info,\n 1 - warning,\n 2 - error", true, 0);
		I_ASSIGN(m_showLogDescriptionAttrPtr, "ShowLogDescription", "Sets the log tables description visible", false, false);
		I_ASSIGN(m_showMessageTextFilterAttrPtr, "ShowMessageTextFilter", "If enabled, the text filter for the messages will be shown", true, true);
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
		DR_CATEGORY,
		DR_IS_MESSAGE_REMOVED_FLAG
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

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (imod::IObserver)
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	void UpdateVisualStatus();
	void UpdateItemVisibility(QTreeWidgetItem* itemPtr, const QString& filterText) const;

protected Q_SLOTS:
	virtual void OnAddMessage(const istd::IInformationProvider* messagePtr, bool releaseFlag);
	virtual void OnRemoveMessage(qint64 messageTimeStamp);
	virtual void OnReset();
	virtual void OnMessageModeChanged();

	virtual void OnClearAction();
	virtual void OnExportAction();

	void OnRemoveMessagesTimer();

	void on_FilterText_textChanged(const QString& filterText);
	void on_FilterClearButton_clicked();

Q_SIGNALS:
	void EmitAddMessage(const istd::IInformationProvider* messagePtr, bool releaseFlag);
	void EmitRemoveMessage(qint64);
	void EmitReset();

protected:
	QAction* m_infoAction;
	QAction* m_warningAction;
	QAction* m_errorAction;
	QAction* m_clearAction;
	QAction* m_exportAction;

private:
	I_REF(iser::IFileLoader, m_fileLoaderCompPtr);
	I_ATTR(int, m_defaultModeAttrPtr);
	I_ATTR(bool, m_showLogDescriptionAttrPtr);
	I_ATTR(bool, m_showMessageTextFilterAttrPtr);

	int m_currentMessageMode;

	int m_statusCategory;

	QTimer m_removeMessagesTimer;
	bool m_messagesWereRemoved;
};


} // namespace iqtgui


#endif // !iqtgui_CLogGuiComp_included

