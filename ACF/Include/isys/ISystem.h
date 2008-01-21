#ifndef isys_ISystem_h_included
#define isys_ISystem_h_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"


namespace isys
{


/**
	Interface for access of some system dependent features and infos.
*/
class ISystem: virtual public istd::IPolymorphic
{
public:
	/**
	* Returns the standard temp path, that will be used by the application.
	*/
	virtual istd::CString GetTempDirPath() const = 0;

	/**
	* Returns the working directory of application.
	*/
	virtual istd::CString GetWorkingDirectory() const = 0;

	/**
	* Returns default system pointer.
	*/
	static isys::ISystem* GetDefaultSystemPtr();

protected:
	static void SetDefaultSystemPtr(isys::ISystem* systemPtr);

protected:
	static isys::ISystem* s_defaultSystemPtr;
};


// public static members

inline isys::ISystem* ISystem::GetDefaultSystemPtr()
{
	return s_defaultSystemPtr;
}


// protected static members

inline void ISystem::SetDefaultSystemPtr(isys::ISystem* systemPtr)
{
	s_defaultSystemPtr = systemPtr;
}


} // namespace isys


#endif // !isys_ISystem_h_included
