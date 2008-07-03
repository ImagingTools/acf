#ifndef iqt_CLogGuiComp_included
#define iqt_CLogGuiComp_included


#include <QMutex>
#include <QMap>

#include "ibase/IMessageContainer.h"
#include "ibase/TMessageContainerWrap.h"

#include "iser/IFileLoader.h"

#include "imod/TSingleModelObserverBase.h"
#include "imod/TModelWrap.h"

#include "iqt/TDesignerGuiCompBase.h"

#include "iqt/Generated/ui_CLogGuiComp.h"


namespace iqt
{


class CLogGuiComp:
			public iqt::TDesignerGuiCompBase<Ui::CLogGuiComp>, 
			public ibase::CMessageContainer
{
	Q_OBJECT

public:
	typedef iqt::TDesignerGuiCompBase<Ui::CLogGuiComp> BaseClass;
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
		TextColumn = 1,
		SourceColumn = 2
	};

public:
	CLogGuiComp();

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
	
	// reimplemented (istd::IChangeable)
	virtual void OnBeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

protected slots:
	void OnAddMessage(ibase::IMessage* messagePtr);
	void OnRemoveMessage(ibase::IMessage* messagePtr);
	void on_ClearButton_clicked();
	void on_ExportButton_clicked();
	void on_CategorySlider_valueChanged(int category);

private:
	QColor GetMessageColor(const ibase::IMessage& message) const;
	bool NeedToBeHidden(const ibase::IMessage& message) const;

signals:
	void EmitAddMessage(ibase::IMessage* messagePtr);
	void EmitRemoveMessage(ibase::IMessage* messagePtr);

private:
	I_REF(iser::IFileLoader, m_fileLoaderCompPtr);
	I_ATTR(int, m_maxMessageCountAttrPtr);

	class CMessageItem: public QTreeWidgetItem
	{
	public:
		ibase::IMessage* messagePtr;
	};

	typedef QMap<int, QString> CategoryNameMap;

	CategoryNameMap m_categoryNameMap;
};


} // namespace iqt


#endif // !iqt_CLogGuiComp_included

