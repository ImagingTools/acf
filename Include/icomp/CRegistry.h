#ifndef icomp_CRegistry_included
#define icomp_CRegistry_included


#include "istd/TChangeDelegator.h"

#include "imod/TModelWrap.h"

#include "icomp/IRegistry.h"
#include "icomp/CRegistryElement.h"


namespace icomp
{


/**
	Standard implementation of registry.
*/
class CRegistry: virtual public IRegistry
{
public:
	// reimplemented (icomp::IRegistry)
	virtual Ids GetElementIds() const;
	virtual ElementInfo* InsertElementInfo(
				const std::string& elementId,
				const icomp::CComponentAddress& address,
				bool ensureElementCreated = true);
	virtual const ElementInfo* GetElementInfo(const std::string& elementId) const;
	virtual bool RemoveElementInfo(const std::string& elementId);
	virtual const ExportedInterfacesMap& GetExportedInterfacesMap() const;
	virtual const ExportedComponentsMap& GetExportedComponentsMap() const;
	virtual void SetElementInterfaceExported(
				const std::string& elementId,
				const istd::CClassInfo& exportInterfaceInfo,
				bool state);
	virtual void SetElementExported(
				const std::string& exportId,
				const std::string& elementId);
	virtual const istd::CString& GetDescription() const;
	virtual void SetDescription(const istd::CString& description);
	virtual const istd::CString& GetKeywords() const;
	virtual void SetKeywords(const istd::CString& keywords);
	
	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
	virtual I_DWORD GetMinimalVersion(int versionId = iser::IVersionInfo::UserVersionId) const;

protected:
	typedef imod::TModelWrap<istd::TChangeDelegator<CRegistryElement> > Element;

	/**
		Called to create instance of registry element.
	*/
	virtual icomp::IRegistryElement* CreateRegistryElement(
				const std::string& elementId,
				const icomp::CComponentAddress& address) const;
	virtual bool SerializeComponents(iser::IArchive& archive);
	virtual bool SerializeExportedInterfaces(iser::IArchive& archive);
	virtual bool SerializeExportedComponents(iser::IArchive& archive);

private:
	typedef std::map<std::string, ElementInfo> ComponentsMap;

	ComponentsMap m_componentsMap;
	ExportedInterfacesMap m_exportedInterfacesMap;
	ExportedComponentsMap m_exportedComponentsMap;

	istd::CString m_description;
	istd::CString m_keywords;
};


}//namespace icomp


#endif // !icomp_CRegistry_included


