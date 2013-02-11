#ifndef icmpstr_CComponentPromotorDialogComp_included
#define icmpstr_CComponentPromotorDialogComp_included


// Qt includes
#include <QtGui/QDialog>

// ACF includes
#include "imod/TSingleModelObserverBase.h"
#include "icomp/IMetaInfoManager.h"
#include "icomp/CComponentBase.h"
#include "ibase/ICommandsProvider.h"
#include "ibase/TModelObserverCompWrap.h"
#include "iqtgui/CHierarchicalCommand.h"
#include "icmpstr/IElementSelectionInfo.h"

#include "icmpstr/Generated/ui_CComponentPromotorDialogComp.h"


namespace icmpstr
{


class CComponentPromotorDialogCompBase:
			public icomp::CComponentBase,
			public imod::TSingleModelObserverBase<IElementSelectionInfo>
{
public:
};


class CComponentPromotorDialogComp:
			public QDialog,
			public Ui::CComponentPromotorDialogComp,
			public ibase::TModelObserverCompWrap<CComponentPromotorDialogCompBase>,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT

public:
	typedef ibase::TModelObserverCompWrap<CComponentPromotorDialogCompBase> BaseClass;
	typedef imod::TSingleModelObserverBase<IElementSelectionInfo> BaseClass2;
	typedef QDialog BaseClass3;

	I_BEGIN_COMPONENT(CComponentPromotorDialogComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_metaInfoManagerCompPtr, "MetaInfoManager", "Access to manager of components meta information", true, "MetaInfoManager");
	I_END_COMPONENT;

public:
	CComponentPromotorDialogComp();

	// reimpemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

protected:
	enum GroupId
	{
		GI_COMPONENT = 0x5430
	};

	/**
		Insert supported and used features to list of component filter.
	*/
	void InsertElementToFilters(
				const icomp::IRegistry& registry,
				const QByteArray& elementName,
				const icomp::IRegistry::ElementInfo& elementInfo);

	void CreatePackagesList();
	void CreateComponentsList();
	void CalcFilteredComponents();
	void DoRetranslate();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

protected Q_SLOTS:
	void on_InterfacesPolicyCB_currentIndexChanged(int index);
	void on_AttributesPolicyCB_currentIndexChanged(int index);
	void on_SubcomponentsPolicyCB_currentIndexChanged(int index);
	void on_PackageNameCB_currentIndexChanged(int index);
	void on_ComponentNameCB_currentIndexChanged(int index);
	void OnPromoteCommand();

private:
	enum FilterType
	{
		FT_IGNORED,
		FT_USED,
		FT_SUPPORTED
	};

	struct Filter
	{
		icomp::IElementStaticInfo::Ids elements;
		icomp::IElementStaticInfo::Ids interfaces;
		icomp::IElementStaticInfo::Ids attributes;
		icomp::IElementStaticInfo::Ids subcomponents;
	};

	I_REF(icomp::IMetaInfoManager, m_metaInfoManagerCompPtr);

	Filter m_supportedFilter;
	Filter m_usedFilter;

	icomp::IMetaInfoManager::ComponentAddresses m_filteredComponents;

	iqtgui::CHierarchicalCommand m_rootMenuCommand;
	iqtgui::CHierarchicalCommand m_registryMenu;
	iqtgui::CHierarchicalCommand m_promoteComponentCommand;
};


} // namespace icmpstr


#endif // !icmpstr_CComponentPromotorDialogComp_included


