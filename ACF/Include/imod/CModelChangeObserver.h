#ifndef imod_CModelChangeObserver_included
#define imod_CModelChangeObserver_included


#include "imod/imod.h"

#include "imod/TSingleModelObserverBase.h"


namespace imod
{


/**
	\ingroup istd

	This class catchs the updating methods of a given model and marks it as dirty.
	To clear this flag you must call SetDirty(false) manually.
*/
class CModelChangeObserver: virtual public istd::IPolymorphic
{
public:
	CModelChangeObserver();
	CModelChangeObserver(imod::IModel* modelPtr);

	virtual void SetModelPtr(imod::IModel* modelPtr);

protected:
	virtual void SetDirty(bool isDirty = true);
	virtual bool IsDirty() const;

	class Observer: public imod::TSingleModelObserverBase<imod::IModel>
	{
	public:
		Observer(CModelChangeObserver& parent)
		:	m_parent(parent)
		{
		}

		// reimplemented (TSingleModelObserverBase)
		virtual void OnUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL)
		{
			m_parent.SetDirty(true);
		}

		using CSingleModelObserverBase::EnsureDetached;

	private:
		CModelChangeObserver& m_parent;
	};

	friend class Observer;

private:
	bool m_isDirty;

	Observer m_modelObserver;
};


} // namespace imod


#endif // !imod_CModelChangeObserver_included


