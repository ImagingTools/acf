#ifndef icmpstr_CRegistryPropEditorComp_included
#define icmpstr_CRegistryPropEditorComp_included


// ACF includes
#include "icomp/IRegistry.h"
#include "icomp/IComponentEnvironmentManager.h"

#include "ibase/IMessageConsumer.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CCheckableComboBox.h"

#include "icmpstr/IRegistryConsistInfo.h"
#include "icmpstr/Generated/ui_CRegistryPropEditorComp.h"


namespace icmpstr
{


class CRegistryPropEditorComp: public iqtgui::TDesignerGuiObserverCompBase<Ui::CRegistryPropEditorComp, icomp::IRegistry>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CRegistryPropEditorComp, icomp::IRegistry> BaseClass;

	enum ItemType
	{
		IT_EXPORTED_INTERFACE,
		IT_EXPORTED_COMPONENT
	};

	enum DataRole
	{
		/**
			ID of the interface or 
		*/
		DR_COMPONENT_ID = Qt::UserRole + 1,
		DR_ITEM_TYPE = Qt::UserRole + 2,
		DR_EXPORT_ID = Qt::UserRole + 3
	};

	I_BEGIN_COMPONENT(CRegistryPropEditorComp);
		I_ASSIGN(m_consistInfoCompPtr, "ConsistencyInfo", "Allows to check consistency of registries and attributes", false, "ConsistencyInfo");
		I_ASSIGN(m_envManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
		I_ASSIGN_TO(m_envManagerModelCompPtr, m_envManagerCompPtr, false);
	I_END_COMPONENT;

protected:
	class TextLog:
				public QString,
				virtual public ibase::IMessageConsumer
	{
	public:
		// reimplemented (ibase::IMessageConsumer)
		virtual bool IsMessageSupported(
					int messageCategory = -1,
					int messageId = -1,
					const istd::IInformationProvider* messagePtr = NULL) const;
		virtual void AddMessage(const MessagePtr& messagePtr);
	};

	void CreateOverview();

	// reimplemented (imod::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

protected Q_SLOTS:
	void OnUpdateExportsTree();
	void OnUpdateKeywords();
	void on_DescriptionEdit_editingFinished();
	void on_OverviewTree_itemChanged(QTreeWidgetItem* itemPtr, int column);

signals:
	void AfterExportsChanged();

private:
	void UpdateConsistencyStatus(
				QTreeWidgetItem* exportedtemPtr,
				const icomp::IRegistry& registry,
				const QByteArray& componentId,
				const QByteArray& exportId,
				ItemType itemType);

	static QString ConvertToKeyword(const QString& input, const QString& key = QString());

private:
	I_REF(IRegistryConsistInfo, m_consistInfoCompPtr);
	I_REF(icomp::IComponentEnvironmentManager, m_envManagerCompPtr);
	I_REF(imod::IModel, m_envManagerModelCompPtr);

	iqtgui::CCheckableComboBox* m_categoryComboBox;
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryPropEditorComp_included

