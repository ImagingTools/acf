#ifndef iqtprm_CParamsManagerGuiComp_included
#define iqtprm_CParamsManagerGuiComp_included


// ACF includes
#include <iqtprm/CParamsManagerGuiCompBase.h>


namespace iqtprm
{


/**
	Simple parameter manager editor using single paramer element editor.
	Due of single editor instance only one kind of elements is supported.
*/
class CParamsManagerGuiComp: public CParamsManagerGuiCompBase
{
public:
	typedef CParamsManagerGuiCompBase BaseClass;

	I_BEGIN_COMPONENT(CParamsManagerGuiComp);
		I_ASSIGN(m_paramsGuiCompPtr, "ParamsGui", "GUI object used to display parameters", false, "ParamsGui");
		I_ASSIGN_TO(m_paramsObserverCompPtr, m_paramsGuiCompPtr, false);
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


