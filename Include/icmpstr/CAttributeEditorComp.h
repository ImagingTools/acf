#ifndef icmpstr_CAttributeEditorComp_included
#define icmpstr_CAttributeEditorComp_included


// Qt includes
#include <QWidget>
#include <QIcon>
#include <QItemDelegate>


// ACF includes
#include "istd/CClassInfo.h"

#include "idoc/IHelpViewer.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CTreeWidgetFilter.h"
#include "iqtgui/CItemDelegate.h"

#include "icmpstr/CElementSelectionInfoManagerBase.h"
#include "icmpstr/CMultiAttributeEditor.h"
#include "icmpstr/Generated/ui_CAttributeEditorComp.h"


namespace icmpstr
{


class CAttributeEditorComp:
			public ibase::TModelObserverCompWrap<
						iqtgui::TGuiObserverWrap<
									iqtgui::TDesignerGuiCompBase<Ui::CAttributeEditorComp>, CElementSelectionInfoManagerBase> >
{
    Q_OBJECT

public:
	typedef ibase::TModelObserverCompWrap<
				iqtgui::TGuiObserverWrap<
							iqtgui::TDesignerGuiCompBase<Ui::CAttributeEditorComp>, CElementSelectionInfoManagerBase> > BaseClass;

	I_BEGIN_COMPONENT(CAttributeEditorComp);
		I_REGISTER_INTERFACE(imod::IModelEditor);
		I_ASSIGN(m_metaInfoManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
		I_ASSIGN(m_attributeSelectionObserverCompPtr, "AttributeSelectionObserver", "Attribute selection observer", false, "AttributeSelectionObserver");
		I_ASSIGN(m_quickHelpViewerCompPtr, "QuickHelpGui", "Shows object info during selection using its type", false, "QuickHelpGui");
		I_ASSIGN(m_consistInfoCompPtr, "ConsistencyInfo", "Allows to check consistency of registries and attributes", false, "ConsistencyInfo");
		I_ASSIGN(m_registryPropGuiCompPtr, "RegistryPropGui", "Display and edit registry properties if no element is selected", false, "RegistryPropGui");
		I_ASSIGN_TO(m_registryPropObserverCompPtr, m_registryPropGuiCompPtr, false);
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
		AM_NONE = 0,
		AM_REFERENCE,
		AM_MULTI_REFERENCE,
		AM_ATTRIBUTE,
		AM_SELECTABLE_ATTRIBUTE,
		AM_MULTI_ATTRIBUTE,
		AM_EXPORTED_ATTR,
		AM_EXPORTED_COMP,
		AM_MULTI
	};

	enum AttributeColumns
	{
		AC_NAME = 0,
		AC_VALUE = 1
	};

	enum AttributeRole
	{
		AttributeMining = Qt::UserRole + 1,
		AttributeId,
		AttributeValue,
		AttributeTypeId,
		ElementId,
		InterfaceName
	};

public:
	CAttributeEditorComp();

	// reimplemented (CElementSelectionInfoManagerBase)
	virtual const icomp::IComponentEnvironmentManager* GetMetaInfoManagerPtr() const;
	virtual const icmpstr::IRegistryConsistInfo* GetConsistencyInfoPtr() const;

protected Q_SLOTS:
	void on_AttributeTree_itemSelectionChanged();
	void on_AttributeTree_itemChanged(QTreeWidgetItem* item, int column);
	void on_InterfacesTree_itemSelectionChanged();
	void on_InterfacesTree_itemChanged(QTreeWidgetItem* item, int column);
	void on_AutoInstanceCB_toggled(bool checked);
	void UpdateGeneralView();
	void UpdateAttributesView();
	void UpdateInterfacesView();
	void UpdateFlagsView();
	void UpdateSubcomponentsView();

Q_SIGNALS:
	void AfterAttributesChange();
	void AfterInterfacesChange();
	void AfterSubcomponentsChange();

protected:
	bool SetAttributeToItem(
				QTreeWidgetItem& attributeItem,
				const icomp::IRegistry& registry,
				const std::string& attributeId,
				const ElementIdToAttrInfoMap& infos,
				bool* hasErrorPtr = NULL,
				bool* hasWarningPtr = NULL,
				bool* hasExportPtr = NULL) const;
	bool SetInterfaceToItem(
				QTreeWidgetItem& item,
				const icomp::IRegistry& registry,
				const std::string& elementId,
				const std::string& interfaceName,
				bool useFullPath,
				bool* hasWarningPtr = NULL,
				bool* hasExportPtr = NULL) const;
	bool DecodeAttribute(
				const iser::ISerializable& attribute,
				QString& text,
				int& meaning) const;
	bool EncodeAttribute(
				const QString& text,
				int meaning,
				iser::ISerializable& result) const;

	void CreateExportedComponentsTree(
				const std::string& elementId,
				const std::string& globalElementId,
				const icomp::IElementStaticInfo* elementMetaInfoPtr,
				QTreeWidgetItem& item,
				bool* hasWarningPtr = NULL,
				bool* hasExportPtr = NULL) const;

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

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

		AttributeItemDelegate(CAttributeEditorComp* parentPtr);

		template <class AttributeImpl>
		static QString GetMultiAttributeValueAsString(const AttributeImpl& attribute);
		
		// reimplemented (QItemDelegate)
		virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		virtual void setEditorData(QWidget* editor, const QModelIndex& index ) const;
		virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

	protected:
		bool SetComponentExportEditor(const std::string& attributeId, const QString& defaultValue, QWidget& editor) const;
		bool SetAttributeExportEditor(const std::string& id, const std::string& exportId, QWidget& editor) const;
		bool SetAttributeValueEditor(const std::string& id, int propertyMining, QWidget& editor) const;

		bool SetComponentExportData(const std::string& attributeId, const QWidget& editor) const;

	private:
		CAttributeEditorComp& m_parent;
	};

	class RegistryObserver: public imod::CSingleModelObserverBase
	{
	public:
		RegistryObserver(CAttributeEditorComp* parentPtr);

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CAttributeEditorComp& m_parent;
	};

	I_REF(icomp::IComponentEnvironmentManager, m_metaInfoManagerCompPtr);
	I_REF(IAttributeSelectionObserver, m_attributeSelectionObserverCompPtr);
	I_REF(idoc::IHelpViewer, m_quickHelpViewerCompPtr);
	I_REF(IRegistryConsistInfo, m_consistInfoCompPtr);
	I_REF(iqtgui::IGuiObject, m_registryPropGuiCompPtr);
	I_REF(imod::IObserver, m_registryPropObserverCompPtr);

	AttributeItemDelegate m_attributeItemDelegate;
	RegistryObserver m_registryObserver;

	typedef std::map<std::string, QString> AttributeTypesMap;
	AttributeTypesMap m_attributeTypesMap;
	istd::TDelPtr<iqtgui::CTreeWidgetFilter> m_attributesTreeFilter;
	istd::TDelPtr<iqtgui::CTreeWidgetFilter> m_interfacesTreeFilter;
	istd::TDelPtr<iqtgui::CTreeWidgetFilter> m_subcomponentsTreeFilter;

	imod::IModel* m_lastRegistryModelPtr;

	QIcon m_invalidIcon;
	QIcon m_warningIcon;
	QIcon m_exportIcon;
	QIcon m_importIcon;
};


// public static methods

template <class AttributeImpl>
QString CAttributeEditorComp::AttributeItemDelegate::GetMultiAttributeValueAsString(const AttributeImpl& attribute)
{
	QString valuesString;
	for (int index = 0; index < attribute.GetValuesCount(); index++){
		if (!valuesString.isEmpty()){
			valuesString += ";";
		}

		valuesString += iqt::GetQString(attribute.GetValueAt(index));
	}

	return valuesString;
}


} // namespace icmpstr


#endif // !icmpstr_CAttributeEditorComp_included

