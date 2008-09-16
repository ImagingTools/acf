#ifndef iqtdoc_CModelEditorGuiComp_included
#define iqtdoc_CModelEditorGuiComp_included


#include "imod/IModel.h"
#include "imod/IModelEditor.h"

#include "icomp/CComponentBase.h"

#include "iqt/IGuiObject.h"

#include "iqt2d/ISceneExtender.h"

#include "iqtdoc/iqtdoc.h"


namespace iqtdoc
{


/**
	Component realizing static connection between GUI and model components.
	It allows to use boundle of model and GUI-observer components as simple GUI object.
*/
class CModelEditorGuiComp:
			public icomp::CComponentBase,
			virtual public iqt::IGuiObject,
			virtual public imod::IModelEditor,
			virtual public iqt2d::ISceneExtender
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CModelEditorGuiComp)
		I_REGISTER_INTERFACE(iqt::IGuiObject)
		I_REGISTER_INTERFACE(imod::IModelEditor)
		I_REGISTER_INTERFACE(iqt2d::ISceneExtender)
		I_ASSIGN(m_slaveEditorCompPtr, "SlaveEditorGui", "Slave GUI object will be connected to model", true, "SlaveEditor");
		I_ASSIGN(m_slaveGuiCompPtr, "SlaveEditorGui", "Slave GUI object will be connected to model", true, "SlaveEditor");
		I_ASSIGN(m_slaveObserverCompPtr, "SlaveEditorGui", "Slave GUI object will be connected to model", false, "SlaveEditor");
		I_ASSIGN(m_slaveExtenderCompPtr, "SlaveEditorGui", "Slave GUI object will be connected to model", false, "SlaveEditor");
		I_ASSIGN(m_modelCompPtr, "Model", "Model will be observed and modify using editor", true, "Model");
	I_END_COMPONENT

	CModelEditorGuiComp();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (iqt::IGuiObject)
	virtual bool IsGuiCreated() const;
	virtual bool CreateGui(QWidget* parentPtr);
	virtual bool DestroyGui();
	virtual QWidget* GetWidget() const;
	virtual void OnTryClose(bool* ignoredPtr = NULL);

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor();
	virtual void UpdateModel() const;
	virtual bool IsReadOnly() const;
	virtual void SetReadOnly(bool state);

	// reimplemented (iqt2d::ISceneExtender)
	virtual void AddItemsToScene(iqt2d::ISceneProvider* providerPtr, int flags);
	virtual void RemoveItemsFromScene(iqt2d::ISceneProvider* providerPtr);

private:
	I_REF(iqt::IGuiObject, m_slaveGuiCompPtr);
	I_REF(imod::IObserver, m_slaveObserverCompPtr);
	I_REF(imod::IModelEditor, m_slaveEditorCompPtr);
	I_REF(iqt2d::ISceneExtender, m_slaveExtenderCompPtr);
	I_REF(imod::IModel, m_modelCompPtr);

	bool m_isConnected;
};


} // namespace iqtdoc


#endif // !iqtdoc_CModelEditorGuiComp_included


