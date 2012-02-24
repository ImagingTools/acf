#ifndef iqtprm_CVariableParamGuiComp_included
#define iqtprm_CVariableParamGuiComp_included


// ACF includes
#include <QString>

#include "iser/IFileLoader.h"

#include "iprm/IVariableParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqt2d/ISceneExtender.h"

#include "iqtprm/iqtprm.h"
#include "iqtprm/Generated/ui_CVariableParamGuiComp.h"


namespace iqtprm
{


class CVariableParamGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<Ui::CVariableParamGuiComp, iprm::IVariableParam>,
			public iqt2d::ISceneExtender
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CVariableParamGuiComp, iprm::IVariableParam> BaseClass;

	I_BEGIN_COMPONENT(CVariableParamGuiComp);
		I_REGISTER_INTERFACE(iqt2d::ISceneExtender);
		I_ASSIGN_MULTI_0(m_editorsCompPtr, "Editors", "List of editor GUI's for each parameter type ID", true);
		I_ASSIGN_TO(m_guisCompPtr, m_editorsCompPtr, true);
		I_ASSIGN_TO(m_observersCompPtr, m_editorsCompPtr, true);
		I_ASSIGN_TO(m_extendersCompPtr, m_editorsCompPtr, false);
		I_ASSIGN_MULTI_0(m_typeIdsAttrPtr, "TypeIds", "List of ID for each variable type", true);
		I_ASSIGN_MULTI_0(m_typeNamesAttrPtr, "TypeNames", "List of ID for each variable type", false);
	I_END_COMPONENT;

	CVariableParamGuiComp();


	// reimplemented (imod::IObserver)
	virtual bool OnDetached(imod::IModel* modelPtr);

	// reimplemented (iqt2d::ISceneExtender)
	virtual void AddItemsToScene(iqt2d::ISceneProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::ISceneProvider* providerPtr);

protected:
	void AttachCurrentType();
	void DetachCurrentType();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

protected Q_SLOTS:
	void on_TypeSelectorCB_currentIndexChanged(int index);

private:
	I_MULTIREF(imod::IModelEditor, m_editorsCompPtr);
	I_MULTIREF(iqtgui::IGuiObject, m_guisCompPtr);
	I_MULTIREF(imod::IObserver, m_observersCompPtr);
	I_MULTIREF(iqt2d::ISceneExtender, m_extendersCompPtr);
	I_MULTIATTR(QString, m_typeIdsAttrPtr);
	I_MULTIATTR(QString, m_typeNamesAttrPtr);

	typedef std::map<iqt2d::ISceneProvider*, int> ConnectedSceneFlags;	// maps connected scene provider to connection flags
	ConnectedSceneFlags m_connectedSceneFlags;

	imod::IModel* m_lastParamModelPtr;
	int m_currentTypeIndex;
};


} // namespace iqtprm


#endif // !iqtprm_CVariableParamGuiComp_included


