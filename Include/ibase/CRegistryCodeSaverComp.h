#ifndef ibase_CRegistryCodeSaverComp_included
#define ibase_CRegistryCodeSaverComp_included


// STL includes
#include <list>
#include <set>

// ACF includes
#include "istd/itr.h"

#include "iser/IFileLoader.h"

#include "icomp/IRegistry.h"
#include "icomp/IPackagesManager.h"
#include "icomp/IRegistriesManager.h"

#include "ibase/TLoggerCompWrap.h"


namespace ibase
{


/**
	Save registry as C++ code genereting class with the same functionality.
*/
class CRegistryCodeSaverComp: public ibase::CLoggerComponentBase, public iser::IFileLoader
{
	I_DECLARE_TR_FUNCTION(CRegistryCodeSaverComp);
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_UNDEFINED_PACKAGE = 0x341f0,
		MI_UNDEFINED_COMPONENT,
		MI_UNDEFINED_ATTR_TYPE
	};

	I_BEGIN_COMPONENT(CRegistryCodeSaverComp);
		I_REGISTER_INTERFACE(iser::IFileTypeInfo);
		I_REGISTER_INTERFACE(iser::IFileLoader);
		I_ASSIGN(m_packagesManagerCompPtr, "PackagesManager", "Packages manager providing access to package informations", true, "PackagesManager");
		I_ASSIGN(m_registriesManagerCompPtr, "RegistriesManager", "Registries manager providing access to all composite component registries", true, "RegistriesManager");
	I_END_COMPONENT;

	CRegistryCodeSaverComp();

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;

	// reimplemented (iser::IFileTypeInfo)
	virtual bool GetFileExtensions(istd::CStringList& result, int flags = -1, bool doAppend = false) const;
	virtual istd::CString GetTypeDescription(const istd::CString* extensionPtr = NULL) const;

protected:
	typedef std::set<icomp::CComponentAddress> Addresses;
	typedef std::set<std::string> Ids;

	/**
		Get set of used addresses used by this registry and its subcomponents.
	*/
	bool AppendAddresses(
				const icomp::IRegistry& registry,
				Addresses& realAddresses,
				Addresses& composedAddresses) const;
	Ids ExtractPackageIds(const Addresses& addresses) const;
	Ids ExtractComponentIds(const Addresses& addresses, const std::string& packageId) const;

	bool WriteHeader(
				const std::string& className,
				const icomp::IRegistry& registry,
				const Addresses& composedAddresses,
				const Addresses& realAddresses,
				std::ofstream& stream) const;
	bool WriteIncludes(
				const std::string& className,
				const Addresses& addresses,
				std::ofstream& stream) const;
	bool WriteClassDefinitions(
				const std::string& className,
				const icomp::IRegistry& registry,
				const Addresses& composedAddresses,
				const Addresses& realAddresses,
				std::ofstream& stream) const;
	bool WriteRegistryInfo(
				const icomp::IRegistry& registry,
				const std::string& registryCallPrefix,
				std::ofstream& stream) const;
	bool WriteComponentInfo(
				const icomp::IRegistry& registry,
				const std::string& registryCallPrefix,
				const std::string& componentId,
				const icomp::IRegistry::ElementInfo& componentInfo,
				std::ofstream& stream) const;
	bool WriteAttribute(
				const std::string& attributeId,
				const std::string& componentId,
				const std::string& attributeName,
				const iser::IObject& attribute,
				std::ofstream& stream) const;

	bool WriteRegistryClassDeclaration(
				const std::string& baseClassName,
				const std::string& registryClassName,
				const icomp::IRegistry& registry,
				std::ofstream& stream) const;
	bool WriteRegistryClassBody(
				const std::string& baseClassName,
				const std::string& registryClassName,
				const icomp::IRegistry& registry,
				std::ofstream& stream) const;

	bool GetAttributeValue(
				const std::string& attributeId,
				const std::string& componentId,
				const iser::ISerializable& attribute,
				std::string& valueString,
				std::string& typeName) const;
	bool GetMultiAttributeValue(
				const std::string& attributeId,
				const std::string& componentId,
				const iser::ISerializable& attribute,
				std::list<std::string>& valueStrings,
				std::string& typeName) const;

	bool NextLine(std::ofstream& stream) const;
	int ChangeIndent(int difference) const;

	bool ExtractInfoFromFile(const istd::CString& filePath, std::string& className, istd::CString& headerFilePath) const;

	std::string GetPackageName(const std::string& packageId) const;

	/**
		Get some wide string as C++ string literal.
	*/
	std::string GetStringLiteral(const istd::CString& text) const;
	/**
		Get some string as C++ string literal.
	*/
	std::string GetStdStringLiteral(const std::string& text) const;

private:
	I_REF(icomp::IPackagesManager, m_packagesManagerCompPtr);
	I_REF(icomp::IRegistriesManager, m_registriesManagerCompPtr);

	mutable int m_indentCount;
};


} // namespace ibase


#endif // !ibase_CRegistryCodeSaverComp_included


