#ifndef iprm_CParamsManagerComp_included
#define iprm_CParamsManagerComp_included


// STL includes
#include <vector>

#include "istd/TPointerVector.h"

#include "icomp/CComponentBase.h"

#include "iprm/IParamsManager.h"


namespace iprm
{


/**
	Implementation of parameter manager.
*/
class CParamsManagerComp:
			public icomp::CComponentBase,
			virtual public IParamsManager
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CParamsManagerComp)
		I_REGISTER_INTERFACE(IParamsManager)
		I_ASSIGN_MULTI_0(m_fixedParamSetsCompPtr, "FixedParamSets", "List of refernces to fixed parameter set", false)
		I_ASSIGN_MULTI_0(m_fixedSetNamesCompPtr, "FixedSetNames", "List of fixed parameter names", false)
		I_ASSIGN(m_defaultSetNameCompPtr, "DefaultSetName", "Default name of parameter set", true, "<noname>")
		I_ASSIGN(m_paramSetsFactPtr, "ParamsSetFactory", "Factory of variable parameter set", false, "ParamsSet")
	I_END_COMPONENT

	bool SetSetsCount(int count);

	// reimplemented (iprm::IParamsManager)
	virtual int GetManagerFlags() const;
	virtual int GetSetsCount() const;
	virtual bool InsertSet(int index = -1);
	virtual bool RemoveSet(int index);
	virtual IParamsSet* GetParamsSet(int index) const;
	virtual const istd::CString& GetSetName(int index) const;
	virtual bool SetSetName(int index, const istd::CString& name);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	I_MULTIREF(IParamsSet, m_fixedParamSetsCompPtr);
	I_MULTIATTR(istd::CString, m_fixedSetNamesCompPtr);
	I_ATTR(istd::CString, m_defaultSetNameCompPtr);
	I_FACT(IParamsSet, m_paramSetsFactPtr);

	istd::TPointerVector<IParamsSet> m_paramSets;
	typedef std::vector<istd::CString> Names;
	Names m_names;
};


} // namespace iprm


#endif // !iprm_CParamsManagerComp_included


