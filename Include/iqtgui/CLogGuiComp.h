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


class CLogGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CLogGuiComp>, 
			public ibase::CMessageContainer
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CLogGuiComp> BaseClass;
	typedef ibase::CMessageContainer BaseClass2;

	I_BEGIN_COMPONENT(CLogGuiComp)
		I_REGISTER_INTERFACE(ibase::IMessageConsumer)
		I_REGISTER_INTERFACE(ibase::IMessageContainer)
		I_ASSIGN(m_fileLoaderCompPtr, "Serializer", "Serializer for log export", false, "Serializer")
		I_ASSIGN(m_maxMessageCountAttrPtr, "MaxMessageCount", "Maximal number of messages", false, 1000)
	I_END_COMPONENT

	enum
	{
		TimeColumn = 0,
		SourceColumn = 1,
		MessageColumn = 2
	};

public:
	CLogGuiComp();

	class CMessageInfo: public ibase::CMessage
	{
	public:
		CMessageInfo(int messageId, MessageCategory category, int id, const istd::CString& text, const istd::CString& source, int flags = 0)
			:ibase::CMessage(category, id, text, source, flags),
			m_messageId(messageId)
		{
		}

		int m_messageId;
	};

	class CMessageTreeItem: public QTreeWidgetItem
	{
	public:
		istd::TDelPtr<CMessageInfo> m_messagePtr;
	};

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	
	// reimplemented (istd::IChangeable)
	virtual void OnBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

protected slots:
	void OnAddMessage(CMessageInfo* messagePtr);
	void OnRemoveMessage(int messageId);
	void OnReset();

	void on_ClearButton_clicked();
	void on_ExportButton_clicked();
	void on_CategorySlider_valueChanged(int category);

private:
	QColor GetMessageColor(const CMessageTreeItem& messageItem) const;
	bool NeedToBeHidden(const CMessageTreeItem& messageItem) const;

signals:
	void EmitAddMessage(CMessageInfo* messagePtr);
	void EmitRemoveMessage(int messageId);
	void EmitReset();

private:
	I_REF(iser::IFileLoader, m_fileLoaderCompPtr);
	I_ATTR(int, m_maxMessageCountAttrPtr);

	typedef QMap<int, QString> CategoryNameMap;

	CategoryNameMap m_categoryNameMap;
};


} // namespace iqtgui


#endif // !iqtgui_CLogGuiComp_included

