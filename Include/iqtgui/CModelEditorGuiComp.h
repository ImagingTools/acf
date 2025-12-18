#ifndef iqtgui_CModelEditorGuiComp_included
#define iqtgui_CModelEditorGuiComp_included


// ACF includes
#include <imod/IModel.h>
#include <imod/IModelEditor.h>
#include <imod/IObserver.h>

#include <icomp/CComponentBase.h>

#include <iqtgui/IGuiObject.h>


namespace iqtgui
{


/**
	Component realizing static connection between GUI and model components.
	It allows to use boundle of model and GUI-observer components as simple GUI object.
*/
class CModelEditorGuiComp:
			public icomp::CComponentBase,
			virtual public iqtgui::IGuiObject,
			virtual public imod::IModelEditor
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CModelEditorGuiComp);
		I_REGISTER_INTERFACE(iqtgui::IGuiObject);
		I_REGISTER_INTERFACE(imod::IModelEditor);
		I_ASSIGN(m_slaveEditorCompPtr, "SlaveEditorGui", "Slave GUI object will be connected to model", true, "SlaveEditor");
		I_ASSIGN_TO(m_slaveGuiCompPtr, m_slaveEditorCompPtr, true);
		I_ASSIGN_TO(m_slaveObserverCompPtr, m_slaveEditorCompPtr, false);
		I_ASSIGN(m_modelCompPtr, "Model", "Model will be observed and modify using editor", true, "Model");
	I_END_COMPONENT;

	CModelEditorGuiComp();

	// reimplemented (iqtgui::IGuiObject)
	virtual bool IsGuiCreated() const override;
	virtual bool CreateGui(QWidget* parentPtr) override;
	virtual bool DestroyGui() override;
	virtual QWidget* GetWidget() const override;
	virtual void OnTryClose(bool* ignoredPtr = NULL) override;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor(const istd::IChangeable::ChangeSet& changeSet) override;
	virtual void UpdateModelFromEditor() const override;
	virtual bool IsReadOnly() const override;
	virtual void SetReadOnly(bool state) override;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	I_REF(iqtgui::IGuiObject, m_slaveGuiCompPtr);
	I_REF(imod::IObserver, m_slaveObserverCompPtr);
	I_REF(imod::IModelEditor, m_slaveEditorCompPtr);
	I_REF(imod::IModel, m_modelCompPtr);

	bool m_isConnected;
};


} // namespace iqtgui


#endif // !iqtgui_CModelEditorGuiComp_included


