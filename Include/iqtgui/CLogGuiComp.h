#ifndef iqtgui_CLogGuiComp_included
#define iqtgui_CLogGuiComp_included


// ACF includes
#include "ibase/IMessageContainer.h"
#include "ibase/CMessage.h"

#include "iser/IFileLoader.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/Generated/ui_CLogGuiComp.h"


namespace iqtgui
{


/**
	Message container displaying messages as log list.
*/
class CLogGuiComp:
	public iqtgui::TDesignerGuiObserverCompBase<
				Ui::CLogGuiComp, ibase::IMessageContainer>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CLogGuiComp, ibase::IMessageContainer> BaseClass;

	I_BEGIN_COMPONENT(CLogGuiComp);
		I_ASSIGN(m_fileLoaderCompPtr, "Exporter", "File loader used for log export", false, "Exporter");
		I_ASSIGN(m_defaultModeAttrPtr, "DefaultMode", "Default display mode,\n 0 - info,\n 1 - warning,\n 2 - error", true, 0);
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
		MM_INFO = istd::ILogger::MC_INFO,
		MM_WARNING = istd::ILogger::MC_WARNING,
		MM_ERROR = istd::ILogger::MC_ERROR,
		MM_ALL = istd::ILogger::MC_CRITICAL
	};

	enum DataRole
	{
		DR_MESSAGE_ID = Qt::UserRole,
		DR_CATEGORY
	};

	/**
		Create GUI item corresponding to specified message.
	*/
	QTreeWidgetItem* CreateGuiItem(const ibase::IMessage& message);
	void UpdateItemState(QTreeWidgetItem& item) const;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (imod::IObserver)
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

protected Q_SLOTS:
	void OnAddMessage(QTreeWidgetItem* itemPtr);
	void OnRemoveMessage(QVariant messageId);
	void OnReset();
	void OnMessageModeChanged();

	void OnClearAction();
	void OnExportAction();

Q_SIGNALS:
	void EmitAddMessage(QTreeWidgetItem* itemPtr);
	void EmitRemoveMessage(QVariant);
	void EmitReset();

private:
	I_REF(iser::IFileLoader, m_fileLoaderCompPtr);
	I_ATTR(int, m_defaultModeAttrPtr);

	QIcon m_infoIcon;
	QIcon m_warningIcon;
	QIcon m_errorIcon;

	int m_currentMessageMode;
};


} // namespace iqtgui


#endif // !iqtgui_CLogGuiComp_included

