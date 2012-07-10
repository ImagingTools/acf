#ifndef iqtgui_CLogGuiComp_included
#define iqtgui_CLogGuiComp_included


// Qt includes
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

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (imod::IObserver)
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	void UpdateVisualStatus();

protected Q_SLOTS:
	virtual void OnAddMessage(const istd::IInformationProvider* messagePtr, bool releaseFlag);
	virtual void OnRemoveMessage(QVariant messageId);
	virtual void OnReset();
	virtual void OnMessageModeChanged();

	virtual void OnClearAction();
	virtual void OnExportAction();

Q_SIGNALS:
	void EmitAddMessage(const istd::IInformationProvider* messagePtr, bool releaseFlag);
	void EmitRemoveMessage(QVariant);
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

	int m_currentMessageMode;

	int m_statusCategory;
};


} // namespace iqtgui


#endif // !iqtgui_CLogGuiComp_included

