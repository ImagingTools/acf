#ifndef iqt_CComposedParamsSetGuiComp_included
#define iqt_CComposedParamsSetGuiComp_included


// Qt includes
#include <QWidget>

#include "istd/CString.h"

#include "imod/TSingleModelObserverBase.h"

#include "iprm/IParamsSet.h"

#include "iqt/IGuiObject.h"
#include "iqt/TGuiObserverWrap.h"
#include "iqt/TGuiComponentBase.h"


namespace iqt
{


class CComposedParamsSetGuiComp: public iqt::TGuiObserverWrap<iqt::TGuiComponentBase<QWidget>, imod::TSingleModelObserverBase<iprm::IParamsSet> >
{
public:
	typedef iqt::TGuiObserverWrap<iqt::TGuiComponentBase<QWidget>, imod::TSingleModelObserverBase<iprm::IParamsSet> > BaseClass;

	I_BEGIN_COMPONENT(CComposedParamsSetGuiComp)
		I_REGISTER_INTERFACE(imod::IObserver)
		I_REGISTER_INTERFACE(imod::IModelEditor)
		I_ASSIGN_MULTI_0(m_guisCompPtr, "Editors", "List of GUI's for parameters edition", true)
		I_ASSIGN_MULTI_0(m_observersCompPtr, "Editors", "List of GUI's for parameters edition", true)
		I_ASSIGN_MULTI_0(m_editorsCompPtr, "Editors", "List of GUI's for parameters edition", true)
		I_ASSIGN_MULTI_0(m_idsAttrPtr, "Ids", "List of parameter ID's according to defined editors", true)
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_MULTIREF(iqt::IGuiObject, m_guisCompPtr);
	I_MULTIREF(imod::IObserver, m_observersCompPtr);
	I_MULTIREF(imod::IModelEditor, m_editorsCompPtr);
	I_MULTIATTR(istd::CString, m_idsAttrPtr);
};


} // namespace iqt


#endif // !iqt_CComposedParamsSetGuiComp_included


