#ifndef CLogGuiComp_included
#define CLogGuiComp_included


#include <QMutex>
#include <QMap>

#include "ibase/IMessageContainer.h"
#include "ibase/TMessageContainerWrap.h"

#include "iser/IFileSerializer.h"

#include "imod/TSingleModelObserverBase.h"
#include "imod/TModelWrap.h"

#include "iqt/TDesignerGuiComponentBase.h"
#include "iqt/Generated/ui_CLogGuiComp.h"


namespace iqt
{


class CLogGuiComp:
	public iqt::TDesignerGuiComponentBase<Ui::CLogGuiComp>, 
	public imod::TModelWrap<ibase::CMessageContainer>
{
	Q_OBJECT

public:
	typedef iqt::TDesignerGuiComponentBase<Ui::CLogGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CLogGuiComp)
		I_REGISTER_INTERFACE(ibase::IMessageContainer)
		I_ASSIGN(m_fileSerializerCompPtr, "Serializer", "Serializer for log export", false, "Serializer")
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
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();
	
protected slots:
	void OnAddMessage(ibase::IMessage* messagePtr);
	void OnRemoveMessage(ibase::IMessage* messagePtr);
	void OnClear();
	void OnExport();
	void OnCategoryChanged(int category);

private:
	QColor GetMessageColor(const ibase::IMessage& message) const;
	bool NeedToBeHidden(const ibase::IMessage& message) const;

signals:
	void EmitAddMessage(ibase::IMessage* messagePtr);
	void EmitRemoveMessage(ibase::IMessage* messagePtr);

private:
	I_REF(iser::IFileSerializer, m_fileSerializerCompPtr);
	I_ATTR(int, m_maxMessageCountAttrPtr);

	class CMessageItem: public QTreeWidgetItem
	{
	public:
		ibase::IMessage* messagePtr;
	};

	class LogObserver: public imod::TSingleModelObserverBase<ibase::IMessageContainer>
	{
	public:
		typedef imod::TSingleModelObserverBase<ibase::IMessageContainer> BaseClass2;

		LogObserver(CLogGuiComp& parent);

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

	private:
		CLogGuiComp& m_parent;
	};
	friend class LogObserver;


	LogObserver m_logObserver;

	typedef QMap<int, QString> CategoryNameMap;

	CategoryNameMap m_categoryNameMap;
};


} // namespace iqt


#endif // CLogGuiComp_included

