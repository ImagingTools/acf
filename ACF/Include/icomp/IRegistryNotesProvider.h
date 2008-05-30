#ifndef icomp_IRegistryNotesProvider_included
#define icomp_IRegistryNotesProvider_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"


namespace icomp
{


class IRegistryNotesProvider: virtual public istd::IPolymorphic
{
public:
	virtual istd::CString GetComponentNote(const std::string& componentRole) const = 0;
	virtual void SetComponentNote(const std::string& componentRole, const istd::CString& componentNote) = 0;
	virtual void RemoveComponentNote(const std::string& componentRole) = 0;
};


} // namespace icomp


#endif // !icomp_IRegistryNotesProvider_included

