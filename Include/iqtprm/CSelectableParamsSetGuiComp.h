#ifndef iqtprm_CSelectableParamsSetGuiComp_included
#define iqtprm_CSelectableParamsSetGuiComp_included


// ACF includes
#include <iqtprm/CSelectableGuiComp.h>


namespace iqtprm
{


class CSelectableParamsSetGuiComp: public iqtprm::CSelectableGuiComp
{
public:
	typedef iqtprm::CSelectableGuiComp BaseClass;

	I_BEGIN_COMPONENT(CSelectableParamsSetGuiComp);
		I_ASSIGN_MULTI_0(m_slaveObserversCompPtr, "Editors", "Editors for the parameters in the parameter set", true);
		I_ASSIGN(m_showSeparatorAttrPtr, "ShowSeparator", "Show separator line between selector and editors", true, false);
	I_END_COMPONENT;

public:
	CSelectableParamsSetGuiComp();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void BeforeUpdate(imod::IModel* modelPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

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
	I_ATTR(bool, m_showSeparatorAttrPtr);
};


} // namespace iqtprm


#endif // !iqtprm_CSelectableParamsSetGuiComp_included


