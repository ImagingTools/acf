#ifndef iproc_CTestIdManagerComp_included
#define iproc_CTestIdManagerComp_included


#include "icomp/CComponentBase.h"

#include "iproc/IIdManager.h"


namespace iproc
{


class CTestIdManagerComp: public icomp::CComponentBase, virtual public IIdManager
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CTestIdManagerComp);
		I_REGISTER_INTERFACE(IIdManager);
	I_END_COMPONENT;

	CTestIdManagerComp();

	// reimplemented (iproc::IIdManager)
	virtual bool SkipToNextId(I_DWORD& result);
	virtual bool GetCurrentId(I_DWORD& result) const;
	virtual isys::ITimer* GetTimeStamp(I_DWORD id) const;
	virtual bool GetPreviousIdFromOffset(int offset, I_DWORD& result) const;
	virtual bool GetOffsetFromPreviousId(I_DWORD id, int& result) const;

private:
	I_DWORD m_currentId;
	bool m_isInitialized;
};


} // namespace acf


#endif // !iproc_CTestIdManagerComp_included


