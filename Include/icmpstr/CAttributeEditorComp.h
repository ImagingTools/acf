#ifndef icmpstr_CAttributeEditorComp_included
#define icmpstr_CAttributeEditorComp_included


// Qt includes
#include <QtGui/QWidget>
#include <QtGui/QIcon>
#include <QtGui/QItemDelegate>


// ACF includes
#include "istd/TDelPtr.h"
#include "istd/CClassInfo.h"

#include "icomp/IMetaInfoManager.h"
#include "icomp/IRegistryElement.h"

#include "idoc/IHelpViewer.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CTreeWidgetFilter.h"
#include "iqtgui/CItemDelegate.h"

#include "icmpstr/IAttributeSelectionObserver.h"
#include "icmpstr/IElementSelectionInfo.h"
#include "icmpstr/IRegistryConsistInfo.h"

#include "icmpstr/Generated/ui_CAttributeEditorComp.h"


namespace icmpstr
{


class CAttributeEditorComp: public iqtgui::TDesignerGuiObserverCompBase<Ui::CAttributeEditorComp, IElementSelectionInfo>
{
    Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CAttributeEditorComp, IElementSelectionInfo> BaseClass;

	I_BEGIN_COMPONENT(CAttributeEditorComp);
		I_ASSIGN(m_metaInfoManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
		I_ASSIGN(m_attributeSelectionObserverCompPtr, "AttributeSelectionObserver", "Attribute selection observer", false, "AttributeSelectionObserver");
		I_ASSIGN(m_quickHelpViewerCompPtr, "QuickHelpViewer", "Shows object info during selection using its type", false, "QuickHelpViewer");
		I_ASSIGN(m_consistInfoCompPtr, "ConsistencyInfo", "Allows to check consistency of registries and attributes", false, "ConsistencyInfo");
		I_ASSIGN(m_registryPropGuiCompPtr, "RegistryPropGui", "Display and edit registry properties if no element is selected", false, "RegistryPropGui");
		I_ASSIGN(m_registryPropObserverCompPtr, "RegistryPropGui", "Display and edit registry properties if no element is selected", false, "RegistryPropGui");
	I_END_COMPONENT;

	enum TabIndex
	{
		TI_GENERAL,
		TI_ATTRIBUTES,
		TI_INTERFACES,
		TI_FLAGS,
		TI_EXPORTS
	};

	enum AttrMeaning
	{
		Reference = 0x1,
		MultipleReference,
		Attribute,
		SelectableAttribute,
		MultipleAttribute,
		AttributeExport,
		ComponentExport
	};

	enum AttributeColumns
	{
		NameColumn = 0,
		ValueColumn = 1
	};

	enum
	{
		AttributeMining = Qt::UserRole + 1,
		AttributeId,
		AttributeType
	};

public:
	CAttributeEditorComp();

	icomp::IRegistry* GetRegistry() const;
	icomp::IRegistryElement* GetRegistryElement() const;
	const std::string& GetRegistryElementName() const;
	icomp::CComponentAddress GetComponentAddress() const;
	icomp::IRegistryElement::AttributeInfo* GetRegistryAttribute(const std::string& id) const;
	const icomp::IAttributeStaticInfo* GetStaticAttributeInfo(const std::string& id) const;
	QStringList GetExportAliases(const std::string& attributeName) const;

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void OnGuiModelDetached();

	// reimplmented (imod::IModelEditor)
	virtual void UpdateEditor(int updateFlags = 0);
	virtual void UpdateModel() const;

protected slots:
	void on_AttributeTree_itemSelectionChanged();
	void on_AttributeTree_itemChanged(QTreeWidgetItem* item, int column);
	void on_InterfacesTree_itemSelectionChanged();
	void on_InterfacesTree_itemChanged(QTreeWidgetItem* item, int column);
	void on_AutoInstanceCB_toggled(bool checked);

protected:
	bool SetAttributeToItems(
				const std::string& id,
				const icomp::IAttributeStaticInfo& staticInfo,
				QTreeWidgetItem& attributeItem,
				QTreeWidgetItem& exportItem,
				bool* hasExportPtr);
	bool DecodeAttribute(
				const iser::ISerializable& attribute,
				QString& text,
				int& meaning);

	void CreateComponentsTree(
				const std::string& elementId,
				const icomp::IComponentStaticInfo& elementStaticInfo,
				QTreeWidgetItem& rootItem) const;

	void UpdateExportIcon();

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// static methods
	static QString DecodeFromEdit(const QString& text);
	static QString EncodeToEdit(const QString& text);

private:
	class AttributeItemDelegate: public iqtgui::CItemDelegate
	{
	public:
		typedef iqtgui::CItemDelegate BaseClass;

		AttributeItemDelegate(CAttributeEditorComp& parent);
		
		// reimplemented (QItemDelegate)
		virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		virtual void setEditorData(QWidget* editor, const QModelIndex& index ) const;
		virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

	protected:
		bool SetComponentExportEditor(const std::string& attributeId, QWidget& editor) const;
		bool SetAttributeExportEditor(const std::string& id, QWidget& editor) const;
		bool SetAttributeValueEditor(const std::string& id, int propertyMining, QWidget& editor) const;

		bool SetComponentExportData(const std::string& attributeId, const QWidget& editor) const;
		bool SetAttributeExportData(
					const std::string& id,
					const std::string& typeName,
					const QWidget& editor) const;
		bool SetAttributeValueData(
					const std::string& id,
					const std::string& typeName,
					int propertyMining,
					const QWidget& editor) const;

	private:
		CAttributeEditorComp& m_parent;
	};

	AttributeItemDelegate m_attributeItemDelegate;

	typedef std::map<std::string, QString> AttributeTypesMap;
	AttributeTypesMap m_attributeTypesMap;
	istd::TDelPtr<iqtgui::CTreeWidgetFilter> m_treeWidgetFilter;

	imod::IModel* m_lastRegistryModelPtr;

	QIcon m_exportIcon;

	I_REF(icomp::IMetaInfoManager, m_metaInfoManagerCompPtr);
	I_REF(IAttributeSelectionObserver, m_attributeSelectionObserverCompPtr);
	I_REF(idoc::IHelpViewer, m_quickHelpViewerCompPtr);
	I_REF(IRegistryConsistInfo, m_consistInfoCompPtr);
	I_REF(iqtgui::IGuiObject, m_registryPropGuiCompPtr);
	I_REF(imod::IObserver, m_registryPropObserverCompPtr);
};


} // namespace icmpstr


#endif // !icmpstr_CAttributeEditorComp_included

