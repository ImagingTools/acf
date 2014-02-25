#ifndef idoc_CDocumentManagerListenerComp_included
#define idoc_CDocumentManagerListenerComp_included


// ACF includes
#include "imod/IModelSelection.h"
#include "imod/TSingleModelObserverBase.h"
#include "icomp/CComponentBase.h"
#include "idoc/IDocumentManager.h"


namespace idoc
{		


class CDocumentManagerListenerComp:
			public icomp::CComponentBase,
			public imod::TSingleModelObserverBase<idoc::IDocumentManager>,
			virtual public imod::IModelSelection
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<idoc::IDocumentManager> BaseClass2;

	I_BEGIN_COMPONENT(CDocumentManagerListenerComp);
		I_REGISTER_INTERFACE(imod::IModelSelection);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_REGISTER_INTERFACE(imod::IModel);
		I_ASSIGN(m_useModelFromViewAttrPtr, "UseModelFromView", "Try to get the observed model from view", false, false);
	I_END_COMPONENT;

	CDocumentManagerListenerComp();

	// reimplemented (imod::IModelSelection)
	virtual imod::IModel* GetSelectedModel() const;
	virtual void SetSelectedModel(imod::IModel* modelPtr);

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	imod::IModel* m_currentDocumentModelPtr;

	I_ATTR(bool, m_useModelFromViewAttrPtr);
};


} // namespace idoc


#endif // !idoc_CDocumentManagerListenerComp_included

