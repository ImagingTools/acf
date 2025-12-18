#ifndef iqtprm_CSelectableParamsSetGuiComp_included
#define iqtprm_CSelectableParamsSetGuiComp_included


// ACF includes
#include <iprm/IParamsSet.h>
#include <iqtprm/CSelectableGuiComp.h>


namespace iqtprm
{


/**
	Editor for the selected parameter in the parameter manager.
	\sa iprm::IParamsManager
*/
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
	virtual void BeforeUpdate(imod::IModel* modelPtr) override;
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet) override;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached() override;
	virtual void OnGuiModelDetached() override;

private:
	void EnsureDetachLastEditor();
	void ConnectCurrentEditor();

	iprm::IParamsSet* ExtractParamsSetPtr(imod::IModel* modelPtr) const;

private:
	int m_currentParamsSetIndex;

	I_MULTIREF(imod::IObserver, m_slaveObserversCompPtr);
};


} // namespace iqtprm


#endif // !iqtprm_CSelectableParamsSetGuiComp_included


