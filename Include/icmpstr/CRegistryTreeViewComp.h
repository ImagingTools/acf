#ifndef icmpstr_CRegistryTreeViewComp_included
#define icmpstr_CRegistryTreeViewComp_included


// Qt includes
#include <QtCore/QMap>
#include <QtCore/QTimer>

// ACF includes
#include "iser/IFileLoader.h"
#include "iser/CArchiveTag.h"

#include "imod/TModelWrap.h"

#include "icomp/IRegistry.h"
#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/CRegistryElement.h"

#include "idoc/IDocumentManager.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "icmpstr/IRegistryConsistInfo.h"
#include "icmpstr/IElementSelectionInfo.h"
#include "icmpstr/Generated/ui_CRegistryTreeViewComp.h"


namespace icmpstr
{


class CRegistryTreeViewComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CRegistryTreeViewComp, icomp::IRegistry>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CRegistryTreeViewComp, icomp::IRegistry> BaseClass;

	enum DataRole
	{
		DR_MESSAGE_LIST = Qt::UserRole + 1,
		DR_ELEMENT_NAME = Qt::UserRole + 2,
		DR_ELEMENT_ID = Qt::UserRole + 3,
		DR_ELEMENT_PACKAGE_ID = Qt::UserRole + 4,
		DR_REGISTRY = Qt::UserRole + 5
	};

	enum ColumnTypes
	{
		CT_NAME,
		CT_ID,
		CT_PACKAGE
	};

	I_BEGIN_COMPONENT(CRegistryTreeViewComp);
		I_REGISTER_SUBELEMENT(SelectionInfo);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectionInfo, IElementSelectionInfo, ExtractSelectionInterface);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectionInfo, imod::IModel, ExtractSelectionInterfaceModel);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectionInfo, istd::IChangeable, ExtractSelectionInterfaceChangeable);
		I_ASSIGN(m_envManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
		I_ASSIGN_TO(m_envManagerModelCompPtr, m_envManagerCompPtr, false);
		I_ASSIGN(m_consistInfoCompPtr, "ConsistencyInfo", "Allows to check consistency of registries and attributes", false, "ConsistencyInfo");
		I_ASSIGN(m_documentManagerCompPtr, "DocumentManager", "Document manager allowing to load files on double click", false, "DocumentManager");
	I_END_COMPONENT;

	CRegistryTreeViewComp();
	
	void AddSubcomponents(
				const icomp::CComponentAddress& address,
				QTreeWidgetItem* registryElementItemPtr);

	void CreateRegistryTree(const icomp::IRegistry& registry, QTreeWidgetItem* registryRootItemPtr);

protected:
	class EnvironmentObserver: public imod::TSingleModelObserverBase<icomp::IComponentEnvironmentManager>
	{
	public:
		EnvironmentObserver(CRegistryTreeViewComp* parentPtr);

	protected:
		// reimplemented (imod::TSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CRegistryTreeViewComp& m_parent;
	};

	/**
		Create instance of shape representing some element.
		The shape will be automatically conncted to element using model/observer pattern.
		\sa imod.
	*/
	QTreeWidgetItem* AddRegistryElementItem(
				const icomp::IRegistry& registry,
				const icomp::IRegistry::ElementInfo* elementPtr,
				const QByteArray& elementId,
				QTreeWidgetItem* parentItemPtr);

	/**
		Calculate visual status of the entire registry tree.
	*/
	bool IsRegistryValid(const icomp::IRegistry& registry) const;

	void UpdateRegistryStatus();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags);
	virtual void OnGuiModelDetached();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// static methods
	static IElementSelectionInfo* ExtractSelectionInterface(CRegistryTreeViewComp& component);
	static imod::IModel* ExtractSelectionInterfaceModel(CRegistryTreeViewComp& component);
	static istd::IChangeable* ExtractSelectionInterfaceChangeable(CRegistryTreeViewComp& component);

protected Q_SLOTS:
	void on_RegistryTree_itemSelectionChanged();
	void on_RegistryTree_itemDoubleClicked(QTreeWidgetItem* itemPtr, int column);

private:
	class SelectionInfoImpl: virtual public IElementSelectionInfo
	{
	public:
		void SetParent(CRegistryTreeViewComp* parentPtr);

		// reimplemented (icmpstr::IElementSelectionInfo)
		virtual icomp::IRegistry* GetSelectedRegistry() const;
		virtual Elements GetSelectedElements() const;

	private:
		CRegistryTreeViewComp* m_parentPtr;
	};

	friend class imod::TModelWrap<SelectionInfoImpl>;

	I_REF(icomp::IComponentEnvironmentManager, m_envManagerCompPtr);
	I_REF(imod::IModel, m_envManagerModelCompPtr);
	I_REF(IRegistryConsistInfo, m_consistInfoCompPtr);
	I_REF(idoc::IDocumentManager, m_documentManagerCompPtr);

	typedef QSet<QByteArray> ElementIds;
	
	struct SelectedElements
	{
		SelectedElements()
			:registryPtr(NULL)
		{
		}

		ElementIds selectedElementIds;
		icomp::IRegistry* registryPtr;
	};

	SelectedElements m_selectedElements;

	EnvironmentObserver m_environmentObserver;

	imod::TModelWrap<SelectionInfoImpl> m_selectionInfo;
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryTreeViewComp_included


