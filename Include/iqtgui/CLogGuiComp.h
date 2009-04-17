#ifndef iqtgui_CLogGuiComp_included
#define iqtgui_CLogGuiComp_included


#include <QMutex>
#include <QMap>

#include "ibase/IMessageContainer.h"
#include "ibase/TMessageContainerWrap.h"

#include "iser/IFileLoader.h"

#include "imod/TSingleModelObserverBase.h"
#include "imod/TModelWrap.h"

#include "iqtgui/TDesignerGuiCompBase.h"
#include "iqtgui/Generated/ui_CLogGuiComp.h"

#include "ibase/CMessage.h"


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
		I_REGISTER_INTERFACE(ibase::IMessageContainer);
		I_ASSIGN(m_fileLoaderCompPtr, "Exporter", "File loader used for log export", false, "Exporter");
		I_ASSIGN(m_maxMessageCountAttrPtr, "MaxMessageCount", "Maximal number of messages", false, 1000);
	I_END_COMPONENT;

	CLogGuiComp();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

protected:
	enum Column
	{
		TimeColumn = 0,
		SourceColumn = 1,
		MessageColumn = 2
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

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	
	// reimplemented (istd::IChangeable)
	virtual void OnBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

protected slots:
	void OnAddMessage(QTreeWidgetItem* itemPtr);
	void OnRemoveMessage(QVariant messageId);
	void OnReset();

	void on_ClearButton_clicked();
	void on_ExportButton_clicked();
	void on_CategorySlider_valueChanged(int category);

signals:
	void EmitAddMessage(QTreeWidgetItem* itemPtr);
	void EmitRemoveMessage(QVariant);
	void EmitReset();

private:
	I_REF(iser::IFileLoader, m_fileLoaderCompPtr);
	I_ATTR(int, m_maxMessageCountAttrPtr);

	typedef QMap<int, QString> CategoryNameMap;

	CategoryNameMap m_categoryNameMap;
};


} // namespace iqtgui


#endif // !iqtgui_CLogGuiComp_included

