#ifndef iqtprm_CSelectableParamsSetGuiComp_included
#define iqtprm_CSelectableParamsSetGuiComp_included


// ACF includes
#include "iqtprm/CSelectableGuiComp.h"


namespace iqtprm
{


class CSelectableParamsSetGuiComp: public iqtprm::CSelectableGuiComp
{
public:
	typedef iqtprm::CSelectableGuiComp BaseClass;

	I_BEGIN_COMPONENT(CSelectableParamsSetGuiComp);
		I_ASSIGN_MULTI_0(m_slaveObserversCompPtr, "Editors", "Editors for the parameters in the parameter set", true);
	I_END_COMPONENT;

public:
	CSelectableParamsSetGuiComp();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

private:
	void EnsureDetachLastEditor();
	void ConnectCurrentEditor();

private:
	int m_currentParamsSetIndex;

	I_MULTIREF(imod::IObserver, m_slaveObserversCompPtr);
};


} // namespace iqtprm


#endif // !iqtprm_CSelectableParamsSetGuiComp_included


