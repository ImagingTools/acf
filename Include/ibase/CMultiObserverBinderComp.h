#ifndef ibase_CMultiObserverBinderComp_included
#define ibase_CMultiObserverBinderComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <imod/IModelEditor.h>
#include <imod/IObserver.h>
#include <imod/IModel.h>


namespace ibase
{

	
/**
	Component for binding of more observers to a data model.

	All methods calls of imod::IModelEditor and imod::IObserver interfaces are delegated to underlaying observers
	specified in multi-component-reference \c m_observersCompPtr and \c m_modelEditorsCompPtr.
	This allows forking of observer functionality from a central location.

	\ingroup ModelObserver
*/
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

	// reimplemented (imod::IObserver)
	virtual bool IsModelAttached(const imod::IModel* modelPtr = NULL) const;
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask);
	virtual bool OnModelDetached(imod::IModel* modelPtr);
	virtual void BeforeUpdate(imod::IModel* modelPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor(const istd::IChangeable::ChangeSet& changeSet);
	virtual void UpdateModelFromEditor() const;
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



