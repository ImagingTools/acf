#ifndef ibase_CMultiObserverBinderComp_included
#define ibase_CMultiObserverBinderComp_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "imod/IModelEditor.h"
#include "imod/IObserver.h"
#include "imod/IModel.h"


namespace ibase
{


class CMultiObserverBinderComp: 
		public icomp::CComponentBase,
		virtual public imod::IModelEditor,
		virtual public imod::IObserver
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CMultiObserverBinderComp);
		I_REGISTER_INTERFACE(IModelEditor);
		I_REGISTER_INTERFACE(IObserver);
		I_ASSIGN_MULTI_0(m_observersCompPtr, "SlaveObservers", "List of components that observe the model", true);
		I_ASSIGN_TO(m_modelEditorsCompPtr, m_observersCompPtr, false);
	I_END_COMPONENT;

	CMultiObserverBinderComp();

	// reimplemented (IObserver)
	virtual bool IsModelAttached(const imod::IModel* modelPtr = NULL) const;
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (IModelEditor)
	virtual void UpdateEditor(int updateFlags = 0);
	virtual void UpdateModel() const;
	virtual bool IsReadOnly() const;
	virtual void SetReadOnly(bool state);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentDestroyed();

private:
	void EnsureModelDetached();

private:
	I_MULTIREF(imod::IModelEditor, m_modelEditorsCompPtr);
	I_MULTIREF(imod::IObserver, m_observersCompPtr);

	imod::IModel* m_attachedModelPtr;
};


} // namespace ibase


#endif // !ibase_CMultiObserverBinderComp_included



