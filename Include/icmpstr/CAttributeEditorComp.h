#ifndef icmpstr_CAttributeEditorComp_included
#define icmpstr_CAttributeEditorComp_included


#include <QtGui/QWidget>
#include <QtGui/QComboBox>
#include <QtGui/QIcon>
#include <QItemDelegate>


#include "istd/TDelPtr.h"
#include "istd/CClassInfo.h"

#include "idoc/IHelpViewer.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CTreeWidgetFilter.h"

#include "icomp/IRegistryElement.h"

#include "icmpstr/IAttributeSelectionObserver.h"
#include "icmpstr/IElementSelectionInfo.h"

#include "icmpstr/Generated/ui_CAttributeEditorComp.h"


namespace icmpstr
{


class CAttributeEditorComp: public iqtgui::TDesignerGuiObserverCompBase<Ui::CAttributeEditorComp, IElementSelectionInfo>
{
    Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CAttributeEditorComp, IElementSelectionInfo> BaseClass;
	I_BEGIN_COMPONENT(CAttributeEditorComp);
		I_ASSIGN(m_attributeSelectionObserverCompPtr, "AttributeSelectionObserver", "Attribute selection observer", false, "AttributeSelectionObserver");
		I_ASSIGN(m_quickHelpViewerCompPtr, "QuickHelpViewer", "Shows object info during selection using its type", false, "QuickHelpViewer");
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
		AttributeId = Qt::UserRole + 2
	};

public:
	CAttributeEditorComp();

	icomp::IRegistry* GetRegistry() const;
	icomp::IRegistryElement* GetRegistryElement() const;
	icomp::IRegistryElement::AttributeInfo* GetRegistryAttribute(const std::string& attributeId) const;
	const icomp::IAttributeStaticInfo* GetStaticAttributeInfo(const std::string& attributeId) const;
	QStringList GetCompatibleComponents(const istd::CClassInfo& interfaceInfo) const;
	QStringList GetExportAliases(const std::string& attributeName) const;

	// reimplemented (TGuiObserverWrap)
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
				const std::string& attributeId,
				const icomp::IAttributeStaticInfo& staticInfo,
				QTreeWidgetItem& attributeItem,
				QTreeWidgetItem& exportItem,
				bool* hasExportPtr,
				bool* isCorrectPtr);
	bool DecodeAttribute(
				const iser::ISerializable& attribute,
				const icomp::IAttributeStaticInfo& staticInfo,
				QString& text,
				int& meaning,
				bool& isCorrect);

	void CreateComponentsTree(
				const std::string& elementId,
				const icomp::IComponentStaticInfo& elementStaticInfo,
				QTreeWidgetItem& rootItem) const;
	QStringList GetCompatibleSubcomponents(
				const std::string& elementId,
				const icomp::IComponentStaticInfo& elementStaticInfo,
				const istd::CClassInfo& interfaceInfo) const;

	void UpdateExportIcon();

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

	// static methods
	static QString DecodeFromEdit(const QString& text);
	static QString EncodeToEdit(const QString& text);

private:
	class AttributeItemDelegate: public QItemDelegate
	{
	public:
		AttributeItemDelegate(CAttributeEditorComp& parent);
		
		// reimplemented (QItemDelegate)
		virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
		virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		virtual void setEditorData(QWidget* editor, const QModelIndex& index ) const;
		virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
		virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

	protected:
		bool SetComponentExportEditor(const std::string& attributeId, QWidget& editor) const;
		bool SetAttributeExportEditor(const std::string& attributeId, QWidget& editor) const;
		bool SetAttributeValueEditor(const std::string& attributeId, int propertyMining, QWidget& editor) const;

		bool SetComponentExportData(const std::string& attributeId, const QWidget& editor) const;
		bool SetAttributeExportData(const std::string& attributeId, const QWidget& editor) const;
		bool SetAttributeValueData(const std::string& attributeId, int propertyMining, const QWidget& editor) const;

	private:
		CAttributeEditorComp& m_parent;
	};

	AttributeItemDelegate m_attributeItemDelegate;

	typedef std::map<istd::CClassInfo, QString> AttributeTypesMap;
	AttributeTypesMap m_attributeTypesMap;
	istd::TDelPtr<iqtgui::CTreeWidgetFilter> m_treeWidgetFilter;

	QIcon m_exportIcon;

	I_REF(IAttributeSelectionObserver, m_attributeSelectionObserverCompPtr);
	I_REF(idoc::IHelpViewer, m_quickHelpViewerCompPtr);
};


} // namespace icmpstr


#endif // !icmpstr_CAttributeEditorComp_included

