#ifndef iprm_CParamsManagerComp_included
#define iprm_CParamsManagerComp_included


// STL includes
#include <vector>


// ACF includes
#include "istd/TSmartPtr.h"

#include "imod/CMultiModelBridgeBase.h"

#include "icomp/CComponentBase.h"

#include "iprm/IParamsSet.h"
#include "iprm/ISelectionParam.h"
#include "iprm/ISelectionConstraints.h"
#include "iprm/IParamsManager.h"


namespace iprm
{


/**
	Implementation of parameter manager.
*/
class CParamsManagerComp:
			public icomp::CComponentBase,
			virtual public IParamsManager,
			protected imod::CMultiModelBridgeBase,
			virtual protected ISelectionConstraints
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CParamsManagerComp)
		I_REGISTER_INTERFACE(iprm::IParamsManager)
		I_REGISTER_INTERFACE(iser::ISerializable)
		I_ASSIGN_MULTI_0(m_fixedParamSetsCompPtr, "FixedParamSets", "List of refernces to fixed parameter set", false)
		I_ASSIGN_MULTI_0(m_fixedSetNamesCompPtr, "FixedSetNames", "List of fixed parameter names", false)
		I_ASSIGN(m_defaultSetNameCompPtr, "DefaultSetName", "Default name of parameter set", true, "<noname>")
		I_ASSIGN(m_paramSetsFactPtr, "ParamsSetFactory", "Factory of variable parameter set", false, "ParamsSet")
	I_END_COMPONENT

	CParamsManagerComp();

	bool SetSetsCount(int count);

	// reimplemented (iprm::IParamsManager)
	virtual int GetManagerFlags() const;
	virtual int GetParamsSetsCount() const;
	virtual bool InsertParamsSet(int index = -1);
	virtual bool RemoveParamsSet(int index);
	virtual IParamsSet* GetParamsSet(int index) const;
	virtual const istd::CString& GetParamsSetName(int index) const;
	virtual bool SetParamsSetName(int index, const istd::CString& name);

	// reimplemented (iprm::ISelectionParam)
	virtual const ISelectionConstraints* GetConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual ISelectionParam* GetActiveSubselection() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (iprm::ISelectionConstraints)
	virtual int GetOptionsCount() const;
	virtual const istd::CString& GetOptionName(int index) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_MULTIREF(IParamsSet, m_fixedParamSetsCompPtr);
	I_MULTIATTR(istd::CString, m_fixedSetNamesCompPtr);
	I_ATTR(istd::CString, m_defaultSetNameCompPtr);
	I_FACT(IParamsSet, m_paramSetsFactPtr);

	struct ParamSet
	{
		istd::TSmartPtr<IParamsSet> paramSetPtr;
		istd::CString name;
	};

	typedef std::vector<ParamSet> ParamSets;

	ParamSets m_paramSets;

	int m_selectedIndex;
};


} // namespace iprm


#endif // !iprm_CParamsManagerComp_included


