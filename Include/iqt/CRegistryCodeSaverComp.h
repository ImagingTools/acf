#ifndef iqt_CRegistryCodeSaverComp_included
#define iqt_CRegistryCodeSaverComp_included


// Qt includes
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QTextStream>

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
	virtual QByteArray GetOptionId(int index) const;

protected:
	typedef QSet<icomp::CComponentAddress> Addresses;
	typedef QSet<QByteArray> Ids;

	/**
		Get set of used addresses used by this registry and its subcomponents.
	*/
	bool AppendAddresses(
				const icomp::IRegistry& registry,
				Addresses& realAddresses,
				Addresses& composedAddresses) const;
	Ids ExtractPackageIds(const Addresses& addresses) const;
	Ids ExtractComponentIds(const Addresses& addresses, const QByteArray& packageId) const;

	bool WriteHeader(
				const QByteArray& className,
				const icomp::IRegistry& registry,
				const Addresses& composedAddresses,
				const Addresses& realAddresses,
				QTextStream& stream) const;
	bool WriteIncludes(
				const QByteArray& className,
				const Addresses& addresses,
				QTextStream& stream) const;
	bool WriteClassDefinitions(
				const QByteArray& className,
				const icomp::IRegistry& registry,
				const Addresses& composedAddresses,
				const Addresses& realAddresses,
				QTextStream& stream) const;
	bool WriteDependencies(
				const QByteArray& className,
				const Addresses& composedAddresses,
				const Addresses& realAddresses,
				QTextStream& stream) const;
	bool WriteRegistryInfo(
				const icomp::IRegistry& registry,
				const QByteArray& registryCallPrefix,
				QTextStream& stream) const;
	bool WriteComponentInfo(
				const icomp::IRegistry& registry,
				const QByteArray& registryCallPrefix,
				const QByteArray& componentId,
				const icomp::IRegistry::ElementInfo& componentInfo,
				QTextStream& stream) const;
	bool WriteAttribute(
				const QByteArray& attributeId,
				const QByteArray& componentId,
				const QByteArray& attributeName,
				const iser::IObject& attribute,
				QTextStream& stream) const;

	bool WriteRegistryClassDeclaration(
				const QByteArray& baseClassName,
				const QByteArray& registryClassName,
				const icomp::IRegistry& registry,
				QTextStream& stream) const;
	bool WriteRegistryClassBody(
				const QByteArray& baseClassName,
				const QByteArray& registryClassName,
				const icomp::IRegistry& registry,
				QTextStream& stream) const;

	bool GetAttributeValue(
				const QByteArray& attributeId,
				const QByteArray& componentId,
				const iser::ISerializable& attribute,
				QByteArray& valueString,
				QByteArray& typeName) const;
	bool GetMultiAttributeValue(
				const QByteArray& attributeId,
				const QByteArray& componentId,
				const iser::ISerializable& attribute,
				QList<QByteArray>& valueStrings,
				QByteArray& typeName) const;

	bool NextLine(QTextStream& stream) const;
	int ChangeIndent(int difference) const;

	bool ExtractInfoFromFile(const QString& filePath, QByteArray& className, QString& baseFilePath) const;

	QByteArray GetPackageName(const QByteArray& packageId) const;

	/**
		Get some wide string as C++ string literal.
	*/
	QByteArray GetStringLiteral(const QString& text) const;
	/**
		Get some string as C++ string literal.
	*/
	QByteArray GetIdValueLiteral(const QByteArray& text) const;

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


