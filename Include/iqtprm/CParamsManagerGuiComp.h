#ifndef iqtprm_CParamsManagerGuiComp_included
#define iqtprm_CParamsManagerGuiComp_included


// ACF includes
#include "iqtprm/CParamsManagerGuiCompBase.h"


namespace iqtprm
{


class CParamsManagerGuiComp: public CParamsManagerGuiCompBase
{
public:
	typedef CParamsManagerGuiCompBase BaseClass;

	I_BEGIN_COMPONENT(CParamsManagerGuiComp);
		I_ASSIGN(m_paramsGuiCompPtr, "ParamsGui", "GUI object used to display parameters", false, "ParamsGui");
		I_ASSIGN(m_paramsObserverCompPtr, "ParamsGui", "GUI object used to display parameters", false, "ParamsGui");
	I_END_COMPONENT;

protected:
	// reimplemented (CParamsManagerGuiCompBase)
	virtual imod::IObserver* GetObserverPtr(const iprm::IParamsSet* paramsSetPtr) const;
	virtual iqtgui::IGuiObject* GetEditorGuiPtr(const iprm::IParamsSet* paramsSetPtr) const;
	
	// reimplemented (iqtgui::CComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(iqtgui::IGuiObject, m_paramsGuiCompPtr);
	I_REF(imod::IObserver, m_paramsObserverCompPtr);
};


} // namespace iqtprm


#endif // !iqtprm_CParamsManagerGuiComp_included


