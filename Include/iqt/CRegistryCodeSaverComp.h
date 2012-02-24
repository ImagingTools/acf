#ifndef iqt_CRegistryCodeSaverComp_included
#define iqt_CRegistryCodeSaverComp_included


// STL includes
#include <list>
#include <set>

// ACF includes
#include "iser/IFileLoader.h"

#include "icomp/IRegistry.h"
#include "icomp/IExtPackagesManager.h"
#include "icomp/IRegistriesManager.h"

#include "iprm/ISelectionParam.h"
#include "iprm/ISelectionConstraints.h"
#include "iprm/IFileNameParam.h"

#include "ibase/TLoggerCompWrap.h"


namespace iqt
{


/**
	Save registry as C++ code (it generates C++ class with the same functionality) and/or dependencies list.
*/
class CRegistryCodeSaverComp:
			public ibase::CLoggerComponentBase,
			public iser::IFileLoader,
			public iprm::ISelectionConstraints
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_UNDEFINED_PACKAGE = 0x341f0,
		MI_UNDEFINED_COMPONENT,
		MI_UNDEFINED_ATTR_TYPE,
		MI_UNKNOWN_PACKAGE
	};

	enum WorkingMode
	{
		WM_SOURCES = 1,
		WM_DEPENDENCIES,
		WM_SOURCES_AND_WM_DEPENDENCIES
	};

	I_BEGIN_COMPONENT(CRegistryCodeSaverComp);
		I_REGISTER_INTERFACE(iser::IFileTypeInfo);
		I_REGISTER_INTERFACE(iser::IFileLoader);
		I_REGISTER_INTERFACE(iprm::ISelectionConstraints);
		I_ASSIGN(m_packagesManagerCompPtr, "PackagesManager", "Packages manager providing access to package informations", true, "PackagesManager");
		I_ASSIGN_TO(m_extPackagesManagerCompPtr, m_packagesManagerCompPtr, false);
		I_ASSIGN(m_registriesManagerCompPtr, "RegistriesManager", "Registries manager providing access to all composite component registries", true, "RegistriesManager");
		I_ASSIGN(m_workingModeParamCompPtr, "WorkingModeParam", "Parameter controlling working mode, if not set default working mode will be used", false, "WorkingModeParam");
		I_ASSIGN(m_baseDependenciesPathCompPtr, "BaseDependenciesPath", "All dependencies will be generated relative to this path", false, "BaseDependenciesPath");
		I_ASSIGN(m_workingModeAttrPtr, "WorkingMode", "Working mode used if WorkingModeParam not specified\n1 - save sources\n2 - save dependencies\n3 - save sources and dependencies", true, 1);
	I_END_COMPONENT;

	CRegistryCodeSaverComp();

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const QString& filePath = QString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const QString& filePath = QString()) const;

	// reimplemented (iser::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

	// reimplemented (iprm::ISelectionConstraints)
	virtual int GetConstraintsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual std::string GetOptionId(int index) const;

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
	bool WriteDependencies(
				const std::string& className,
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

	bool ExtractInfoFromFile(const QString& filePath, std::string& className, QString& baseFilePath) const;

	std::string GetPackageName(const std::string& packageId) const;

	/**
		Get some wide string as C++ string literal.
	*/
	std::string GetStringLiteral(const QString& text) const;
	/**
		Get some string as C++ string literal.
	*/
	std::string GetStdStringLiteral(const std::string& text) const;

private:
	I_REF(icomp::IPackagesManager, m_packagesManagerCompPtr);
	I_REF(icomp::IExtPackagesManager, m_extPackagesManagerCompPtr);
	I_REF(icomp::IRegistriesManager, m_registriesManagerCompPtr);
	I_REF(iprm::ISelectionParam, m_workingModeParamCompPtr);
	I_REF(iprm::IFileNameParam, m_baseDependenciesPathCompPtr);
	I_ATTR(int, m_workingModeAttrPtr);

	mutable int m_indentCount;
};


} // namespace iqt


#endif // !iqt_CRegistryCodeSaverComp_included


