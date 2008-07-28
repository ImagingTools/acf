#ifndef imil_CMilEngine_included
#define imil_CMilEngine_included


#include "imil/imil.h"


#include "inat/CCriticalSection.h"


namespace imil
{


class CMilEngine
{
public:
	CMilEngine();
	~CMilEngine();

	MIL_ID GetSystemId() const;

private:
	static inat::CCriticalSection s_criticalSection;
	static MIL_ID s_milSystemId;
	static MIL_ID s_milApplication;
	static int s_referenceCount;
};


} // namespace imil


#endif // !imil_CMilEngine_included


