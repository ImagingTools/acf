#include "iqt/CRegistryCodeSaverComp.h"


// STL includes
#include <fstream>

// Qt includes
#include <QObject>
#include <QDir>

// ACF includes
#include "iqt/iqt.h"


namespace iqt
{


CRegistryCodeSaverComp::CRegistryCodeSaverComp()
:	m_indentCount(0)
{
}


// reimplemented (iser::IFileLoader)

bool CRegistryCodeSaverComp::IsOperationSupported(
		const istd::IChangeable* dataObjectPtr,
		const QString* /*filePathPtr*/,
		int flags,
		bool /*beQuiet*/) const
{
	if (dynamic_cast<const icomp::IRegistry*>(dataObjectPtr) == NULL){
		return false;
	}

	return ((flags & QF_SAVE) != 0) && ((flags & QF_FILE) != 0);
}


int CRegistryCodeSaverComp::LoadFromFile(istd::IChangeable& /*data*/, const QString& /*filePath*/) const
{
	return StateFailed;
}


int CRegistryCodeSaverComp::SaveToFile(const istd::IChangeable& data, const QString& filePath) const
{
	const icomp::IRegistry* registryPtr = dynamic_cast<const icomp::IRegistry*>(&data);
	if (registryPtr == NULL){
		return StateFailed;
	}

	Addresses realAddresses;
	Addresses composedAddresses;
	std::string className;
	QString baseFilePath;
	if (		!AppendAddresses(*registryPtr, realAddresses, composedAddresses) ||
				!ExtractInfoFromFile(filePath, className, baseFilePath)){
		return StateFailed;
	}

	int workingMode = *m_workingModeAttrPtr;
	if (m_workingModeParamCompPtr.IsValid()){
		workingMode = m_workingModeParamCompPtr->GetSelectedOptionIndex();
	}

	if ((workingMode == WM_SOURCES) || (workingMode == WM_SOURCES_AND_WM_DEPENDENCIES)){
		QString headerFilePath = baseFilePath + QString(".h");
		std::ofstream stream(filePath.toStdString().c_str());
		std::ofstream headerStream(headerFilePath.toStdString().c_str());

		if (		!WriteHeader(className, *registryPtr, composedAddresses, realAddresses, headerStream) ||
					!WriteIncludes(className, realAddresses, stream) ||
					!WriteClassDefinitions(className, *registryPtr, composedAddresses, realAddresses, stream)){
			return StateFailed;
		}
	}

	if ((workingMode == WM_DEPENDENCIES) || (workingMode == WM_SOURCES_AND_WM_DEPENDENCIES)){
		QString depsFilePath = baseFilePath + QString(".pri");
		std::ofstream stream(depsFilePath.toStdString().c_str());

		if (!WriteDependencies(className, composedAddresses, realAddresses, stream)){
			return StateFailed;
		}
	}

	return StateOk;
}


// reimplemented (iser::IFileTypeInfo)

bool CRegistryCodeSaverComp::GetFileExtensions(QStringList& result, int flags, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	if ((flags & QF_SAVE) != 0){
		int workingMode = *m_workingModeAttrPtr;
		if (m_workingModeParamCompPtr.IsValid()){
			workingMode = m_workingModeParamCompPtr->GetSelectedOptionIndex();
		}

		switch (workingMode){
		case WM_DEPENDENCIES:
			result.push_back("pri");
			break;

		default:
			result.push_back("c");
			result.push_back("cpp");
			break;
		}
	}

	return true;
}


QString CRegistryCodeSaverComp::GetTypeDescription(const QString* extensionPtr) const
{
	if (extensionPtr == NULL){
		int workingMode = *m_workingModeAttrPtr;
		if (m_workingModeParamCompPtr.IsValid()){
			workingMode = m_workingModeParamCompPtr->GetSelectedOptionIndex();
		}

		switch (workingMode){
		case WM_DEPENDENCIES:
			return QObject::tr("qmake dependency file");

		default:
			return QObject::tr("C++ source file");
		}
	}

	if (		(extensionPtr->compare("c", Qt::CaseInsensitive) == 0) ||
				(extensionPtr->compare("cpp", Qt::CaseInsensitive) == 0)){
		return QObject::tr("C++ source file");
	}
	else if (extensionPtr->compare("pri", Qt::CaseInsensitive) == 0){
		return QObject::tr("qmake dependency file");
	}

	return "";
}


// reimplemented (iprm::ISelectionConstraints)

int CRegistryCodeSaverComp::GetConstraintsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int CRegistryCodeSaverComp::GetOptionsCount() const
{
	return 3;
}


QString CRegistryCodeSaverComp::GetOptionName(int index) const
{
	switch (index){
	case WM_DEPENDENCIES:
		return QObject::tr("Dependencies");

	case WM_SOURCES_AND_WM_DEPENDENCIES:
		return QObject::tr("Sources and dependencies");

	default:
		return QObject::tr("Sources");
	}
}


QString CRegistryCodeSaverComp::GetOptionDescription(int index) const
{
	switch (index){
	case WM_DEPENDENCIES:
		return QObject::tr("List of all component and package files requested by this registry will be saved");

	case WM_SOURCES_AND_WM_DEPENDENCIES:
		return QObject::tr("C++ sources and list of all component and package files requested by this registry will be saved");

	default:
		return QObject::tr("C++ sources will be saved");
	}
}


std::string CRegistryCodeSaverComp::GetOptionId(int index) const
{
	switch (index){
	case WM_DEPENDENCIES:
		return "Dependencies";

	case WM_SOURCES_AND_WM_DEPENDENCIES:
		return "Sources and dependencies";

	default:
		return "Sources";
	}
}


// protected methods

bool CRegistryCodeSaverComp::AppendAddresses(
			const icomp::IRegistry& registry,
			Addresses& realAddresses,
			Addresses& composedAddresses) const
{
	if (!m_packagesManagerCompPtr.IsValid() || !m_registriesManagerCompPtr.IsValid()){
		return false;
	}

	icomp::IRegistry::Ids ids = registry.GetElementIds();
	for (		icomp::IRegistry::Ids::const_iterator elementIter = ids.begin();
				elementIter != ids.end();
				++elementIter){
		const std::string& componentId = *elementIter;

		const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(componentId);
		I_ASSERT(infoPtr != NULL);	// used element ID was returned by registry, info must exist.

		const std::string& packageId = infoPtr->address.GetPackageId();
		if (packageId.empty()){
			continue;	// skip embedded components
		}

		int packageType = m_packagesManagerCompPtr->GetPackageType(packageId);

		switch (packageType){
		case icomp::IPackagesManager::PT_REAL:
			if (realAddresses.find(infoPtr->address) == realAddresses.end()){
				realAddresses.insert(infoPtr->address);
			}
			break;

		case icomp::IPackagesManager::PT_COMPOSED:
			{
				const icomp::IRegistry* registryPtr = m_registriesManagerCompPtr->GetRegistry(infoPtr->address);
				if (registryPtr != NULL){
					composedAddresses.insert(infoPtr->address);

					if (!AppendAddresses(*registryPtr, realAddresses, composedAddresses)){
						return false;
					}
				}
				else{
					SendErrorMessage(
								MI_UNDEFINED_COMPONENT,
								QObject::tr("Composite component '%1' is undefined").arg(infoPtr->address.ToString()));
					return false;
				}
			}
			break;

		default:
			SendErrorMessage(
						MI_UNDEFINED_PACKAGE,
						QObject::tr("Package '%1' is undefined").arg(packageId.c_str()));
			return false;
		}
	}

	return true;
}


CRegistryCodeSaverComp::Ids CRegistryCodeSaverComp::ExtractPackageIds(const Addresses& addresses) const
{
	Ids retVal;

	for (		Addresses::const_iterator addressIter = addresses.begin();
				addressIter != addresses.end();
				++addressIter){
		const icomp::CComponentAddress& address = *addressIter;

		retVal.insert(address.GetPackageId());
	}

	return retVal;
}


CRegistryCodeSaverComp::Ids CRegistryCodeSaverComp::ExtractComponentIds(const Addresses& addresses, const std::string& packageId) const
{
	Ids retVal;

	for (		Addresses::const_iterator addressIter = addresses.begin();
				addressIter != addresses.end();
				++addressIter){
		const icomp::CComponentAddress& address = *addressIter;

		if (address.GetPackageId() == packageId){
			retVal.insert(address.GetComponentId());
		}
	}

	return retVal;
}


bool CRegistryCodeSaverComp::WriteHeader(
			const std::string& className,
			const icomp::IRegistry& registry,
			const Addresses& composedAddresses,
			const Addresses& realAddresses,
			std::ofstream& stream) const
{
	std::string includeDefine = className + "_included";

	stream << "#ifndef " << includeDefine;

	NextLine(stream);
	stream << "#define " << includeDefine;
	stream << std::endl << std::endl;

	NextLine(stream);
	stream << "// ACF includes";
	NextLine(stream);
	stream << "#include \"istd/TDelPtr.h\"";
	NextLine(stream);
	stream << "#include \"icomp/IRegistriesManager.h\"";
	NextLine(stream);
	stream << "#include \"icomp/CRegistry.h\"";
	NextLine(stream);
	stream << "#include \"icomp/CCompositeComponent.h\"";
	NextLine(stream);
	stream << "#include \"icomp/CCompositeComponentContext.h\"";
	NextLine(stream);
	stream << "#include \"icomp/CCompositeComponentStaticInfo.h\"";
	NextLine(stream);
	stream << "#include \"icomp/CPackageStaticInfo.h\"";
	NextLine(stream);
	stream << "#include \"icomp/CCompositePackageStaticInfo.h\"";
	NextLine(stream);
	stream << "#include \"icomp/CEnvironmentManagerBase.h\"";

	stream << std::endl << std::endl;

	QString description = registry.GetDescription();
	if (!description.isEmpty()){
		NextLine(stream);
		stream << "/**";

		NextLine(stream);
		stream << "\t" << description.toStdString() << ".";

		NextLine(stream);
		stream << "*/";
	}

	NextLine(stream);
	stream << "class " << className << ": protected icomp::CCompositeComponent";
	NextLine(stream);
	stream << "{";
	NextLine(stream);
	stream << "public:";
	ChangeIndent(1);

	NextLine(stream);
	stream << "typedef icomp::CCompositeComponent BaseClass;";
	NextLine(stream);

	NextLine(stream);
	stream << "template <class InterfaceType>";
	NextLine(stream);
	stream << "InterfaceType* GetInterface(const std::string& subId = \"\"){return GetComponentInterface<InterfaceType>(subId);}";
	stream << std::endl;

	NextLine(stream);
	stream << className << "();";
	stream << std::endl;

	ChangeIndent(-1);
	NextLine(stream);
	stream << "protected:";
	ChangeIndent(1);

	if (!WriteRegistryClassDeclaration(className, "CMainRegistry", registry, stream)){
		return false;
	}

	Ids composedPackageIds = ExtractPackageIds(composedAddresses);
	if (m_registriesManagerCompPtr.IsValid() && !composedPackageIds.empty()){
		stream << std::endl;

		NextLine(stream);
		stream << "//\tcomposited packages";

		for (		Ids::const_iterator packageIter = composedPackageIds.begin();
					packageIter != composedPackageIds.end();
					++packageIter){
			const std::string& packageId = *packageIter;
			if (packageId.empty()){
				continue;
			}

			std::string packageName = GetPackageName(packageId);

			stream << std::endl;

			NextLine(stream);
			stream << "class C" << packageName << ": public icomp::CCompositePackageStaticInfo";

			NextLine(stream);
			stream << "{";

			NextLine(stream);
			stream << "public:";
			ChangeIndent(1);

			NextLine(stream);
			stream << "C" << packageName << "(const icomp::IComponentEnvironmentManager* managerPtr);";

			Ids componentIds = ExtractComponentIds(composedAddresses, packageId);
			for (		Ids::const_iterator componentIter = componentIds.begin();
						componentIter != componentIds.end();
						++componentIter){
				const std::string& componentId = *componentIter;

				icomp::CComponentAddress address(packageId, componentId);

				const icomp::IRegistry* registryPtr = m_registriesManagerCompPtr->GetRegistry(address, &registry);

				if (registryPtr != NULL){
					stream << std::endl;
					if (!WriteRegistryClassDeclaration(className, "C" + componentId + "Registry", *registryPtr, stream)){
						return false;
					}
				}
				else{
					NextLine(stream);
					stream << "// Registry for address '" << packageId << "/" << componentId << "' could not be created";
				}
			}

			ChangeIndent(-1);
			NextLine(stream);
			stream << "};";
		}
	}

	Ids realPackageIds = ExtractPackageIds(realAddresses);
	if (!realPackageIds.empty()){
		stream << std::endl;

		NextLine(stream);
		stream << "//\treal packages";

		for (		Ids::const_iterator packageIter = realPackageIds.begin();
					packageIter != realPackageIds.end();
					++packageIter){
			const std::string& packageId = *packageIter;
			if (packageId.empty()){
				continue;
			}

			std::string packageName = GetPackageName(packageId);

			stream << std::endl;

			NextLine(stream);
			stream << "class C" << packageName << ": public icomp::CPackageStaticInfo";

			NextLine(stream);
			stream << "{";

			NextLine(stream);
			stream << "public:";
			ChangeIndent(1);

			NextLine(stream);
			stream << "typedef icomp::CPackageStaticInfo BaseClass;";
			stream << std::endl;

			NextLine(stream);
			stream << "C" << packageName << "();";

			ChangeIndent(-1);
			NextLine(stream);
			stream << "};";
		}
	}

	stream << std::endl;

	NextLine(stream);
	stream << "class CLocalEnvironmentManager: public icomp::CEnvironmentManagerBase";
	NextLine(stream);
	stream << "{";
	NextLine(stream);
	stream << "public:";
	ChangeIndent(1);

	NextLine(stream);
	stream << "typedef icomp::CEnvironmentManagerBase BaseClass;";
	stream << std::endl;

	NextLine(stream);
	stream << "CLocalEnvironmentManager();";
	stream << std::endl;

	NextLine(stream);
	stream << "// reimplemented (icomp::IRegistriesManager)";
	NextLine(stream);
	stream << "virtual const icomp::IRegistry* GetRegistry(const icomp::CComponentAddress& address, const icomp::IRegistry* contextRegistryPtr = NULL) const;";

	stream << std::endl;
	ChangeIndent(-1);
	NextLine(stream);
	stream << "protected:";
	ChangeIndent(1);

	NextLine(stream);
	stream << "typedef istd::TDelPtr<icomp::IRegistry> RegistryPtr;";
	stream << "typedef std::map<icomp::CComponentAddress, RegistryPtr> RegistriesMap;";

	stream << std::endl;
	ChangeIndent(-1);
	NextLine(stream);
	stream << "private:";
	ChangeIndent(1);

	if (!realPackageIds.empty()){
		for (		Ids::const_iterator packageInstanceIter = realPackageIds.begin();
					packageInstanceIter != realPackageIds.end();
					++packageInstanceIter){
			const std::string& packageId = *packageInstanceIter;
			if (packageId.empty()){
				continue;
			}

			std::string packageName = GetPackageName(packageId);

			NextLine(stream);
			stream << "C" << packageName << " m_sub" << packageName << ";";
		}

		stream << std::endl;
	}

	if (!composedPackageIds.empty()){
		for (		Ids::const_iterator registerPackageIter = composedPackageIds.begin();
					registerPackageIter != composedPackageIds.end();
					++registerPackageIter){
			const std::string& packageId = *registerPackageIter;
			if (packageId.empty()){
				continue;
			}

			std::string packageName = GetPackageName(packageId);

			NextLine(stream);
			stream << "istd::TDelPtr<icomp::IComponentStaticInfo> m_package" << packageId << "InfoPtr;";
		}

		stream << std::endl;
	}

	NextLine(stream);
	stream << "RegistriesMap m_registriesMap;";

	ChangeIndent(-1);
	NextLine(stream);
	stream << "};";
	stream << std::endl;

	ChangeIndent(-1);
	NextLine(stream);
	stream << "private:";
	ChangeIndent(1);

	NextLine(stream);
	stream << "istd::TDelPtr<icomp::CCompositeComponentContext> m_mainContextPtr;";

	ChangeIndent(-1);
	NextLine(stream);
	stream << "};";

	stream << std::endl << std::endl;

	NextLine(stream);
	stream << "#endif // !" << includeDefine;

	stream << std::endl << std::endl;

	return !stream.fail();
}


bool CRegistryCodeSaverComp::WriteIncludes(
			const std::string& className,
			const Addresses& addresses,
			std::ofstream& stream) const
{
	stream << "#include \"" << className << ".h\"" << std::endl;
	stream << std::endl << std::endl;
	stream << "// ACF includes" << std::endl;
	stream << "#include \"istd/itr.h\"" << std::endl;
	stream << "#include \"icomp/TAttribute.h\"" << std::endl;
	stream << "#include \"icomp/TMultiAttribute.h\"" << std::endl;
	stream << "#include \"icomp/CRegistryElement.h\"" << std::endl;
	stream << "#include \"icomp/CCompositePackageStaticInfo.h\"" << std::endl;

	Ids packageIds = ExtractPackageIds(addresses);
	if (!packageIds.empty()){
		stream << std::endl;
		stream << "// ACF component includes" << std::endl;

		for (		Ids::const_iterator packageIter = packageIds.begin();
					packageIter != packageIds.end();
					++packageIter){
			const std::string& packageId = *packageIter;
			if (packageId.empty()){
				continue;
			}

			stream << "#include \"" << packageId << "/" << packageId << ".h\"" << std::endl;
		}
	}

	stream << std::endl << std::endl;
	stream << "I_DECLARE_TR_FUNCTION(" << className << ")" << std::endl << std::endl;

	return !stream.fail();
}


bool CRegistryCodeSaverComp::WriteClassDefinitions(
			const std::string& className,
			const icomp::IRegistry& registry,
			const Addresses& composedAddresses,
			const Addresses& realAddresses,
			std::ofstream& stream) const
{
	NextLine(stream);
	stream << className << "::" << className << "()";

	NextLine(stream);
	stream << "{";
	ChangeIndent(1);

	NextLine(stream);
	stream << "static icomp::CRegistryElement mainElement;";
	NextLine(stream);
	stream << "static CMainRegistry mainRegistry;";
	NextLine(stream);
	stream << "static CLocalEnvironmentManager localEnvironmentManager;";
	stream << std::endl;
	NextLine(stream);
	stream << "m_mainContextPtr.SetPtr(new icomp::CCompositeComponentContext(&mainElement, &localEnvironmentManager, &mainRegistry, &localEnvironmentManager, NULL));";
	stream << std::endl;
	NextLine(stream);
	stream << "SetComponentContext(m_mainContextPtr.GetPtr(), NULL, false);";

	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";
	stream << std::endl << std::endl;

	if (!WriteRegistryClassBody(className, "CMainRegistry", registry, stream)){
		return false;
	}

	stream << std::endl << std::endl;

	Ids composedPackageIds = ExtractPackageIds(composedAddresses);

	if (m_registriesManagerCompPtr.IsValid()){
		for (		Ids::const_iterator packageIter = composedPackageIds.begin();
					packageIter != composedPackageIds.end();
					++packageIter){
			const std::string& packageId = *packageIter;
			if (packageId.empty()){
				continue;
			}

			std::string packageName = GetPackageName(packageId);

			NextLine(stream);
			stream << "// Embedded class C" << packageName;

			stream << std::endl;

			NextLine(stream);
			stream << className << "::C" << packageName << "::C" << packageName << "(const icomp::IComponentEnvironmentManager* managerPtr)";
			NextLine(stream);
			stream << ":\ticomp::CCompositePackageStaticInfo(\"" << packageId << "\", managerPtr)";
			stream << std::endl;


			NextLine(stream);
			stream << "{";
			ChangeIndent(1);

			Ids componentIds = ExtractComponentIds(composedAddresses, packageId);
			for (		Ids::const_iterator instanceRegIter = componentIds.begin();
						instanceRegIter != componentIds.end();
						++instanceRegIter){
				const std::string& componentId = *instanceRegIter;

				NextLine(stream);
				stream << "RegisterEmbeddedComponent(\"" << componentId << "\");";
			}

			ChangeIndent(-1);
			NextLine(stream);
			stream << "}";

			stream << std::endl << std::endl;

			for (		Ids::const_iterator componentIter = componentIds.begin();
						componentIter != componentIds.end();
						++componentIter){
				const std::string& componentId = *componentIter;

				icomp::CComponentAddress address(packageId, componentId);

				const icomp::IRegistry* registryPtr = m_registriesManagerCompPtr->GetRegistry(address, &registry);
				if (registryPtr != NULL){
					if (!WriteRegistryClassBody(className + "::C" + packageName, "C" + componentId + "Registry", *registryPtr, stream)){
						return false;
					}

					stream << std::endl << std::endl;
				}
			}
		}
	}

	Ids realPackageIds = ExtractPackageIds(realAddresses);

	for (		Ids::const_iterator packageIter = realPackageIds.begin();
				packageIter != realPackageIds.end();
				++packageIter){
		const std::string& packageId = *packageIter;
		if (packageId.empty()){
			continue;
		}

		std::string packageName = GetPackageName(packageId);

		NextLine(stream);
		stream << "// Embedded class C" << packageName;

		stream << std::endl;

		NextLine(stream);
		stream << className << "::C" << packageName << "::C" << packageName << "()";

		NextLine(stream);
		stream << "{";
		ChangeIndent(1);

		Ids componentIds = ExtractComponentIds(realAddresses, packageId);
		for (		Ids::const_iterator instanceRegIter = componentIds.begin();
					instanceRegIter != componentIds.end();
					++instanceRegIter){
			const std::string& componentId = *instanceRegIter;

			std::string infoName = "info" + componentId;

			NextLine(stream);
			stream << "const icomp::IComponentStaticInfo& " << infoName << " = " << packageId << "::" << componentId << "::InitStaticInfo(NULL);";

			NextLine(stream);
			stream << "RegisterEmbeddedComponentInfo(\"" << componentId << "\", &" << infoName << ");";
		}

		ChangeIndent(-1);
		NextLine(stream);
		stream << "}";

		stream << std::endl << std::endl;
	}

	NextLine(stream);
	stream << "// Embedded class CLocalEnvironmentManager";

	stream << std::endl;

	NextLine(stream);
	stream << className << "::CLocalEnvironmentManager::CLocalEnvironmentManager()";

	NextLine(stream);
	stream << "{";
	ChangeIndent(1);

	if (!realPackageIds.empty()){
		NextLine(stream);
		stream << "// register real components";

		for (		Ids::const_iterator registerRealIter = realPackageIds.begin();
					registerRealIter != realPackageIds.end();
					++registerRealIter){
			const std::string& packageId = *registerRealIter;
			if (packageId.empty()){
				continue;
			}

			std::string packageName = GetPackageName(packageId);

			NextLine(stream);
			stream << "RegisterEmbeddedComponentInfo(\"" << packageId << "\", &m_sub" << packageName << ");";
		}
	}

	if (!composedAddresses.empty()){
		stream << std::endl;

		NextLine(stream);
		stream << "// create map for all known registries";
		for (		Addresses::const_iterator regTestIter = composedAddresses.begin();
					regTestIter != composedAddresses.end();
					++regTestIter){
			const icomp::CComponentAddress& address = *regTestIter;

			NextLine(stream);
			stream << "m_registriesMap[icomp::CComponentAddress(\"" << address.GetPackageId() << "\", \"" << address.GetComponentId() << "\")].SetPtr(new C" << GetPackageName(address.GetPackageId()) << "::C" << address.GetComponentId() << "Registry());";
		}
	}

	if (!composedPackageIds.empty()){
		stream << std::endl;

		NextLine(stream);
		stream << "// register composed packages";

		for (		Ids::const_iterator registerPackageIter = composedPackageIds.begin();
					registerPackageIter != composedPackageIds.end();
					++registerPackageIter){
			const std::string& packageId = *registerPackageIter;
			if (packageId.empty()){
				continue;
			}

			if (registerPackageIter != composedPackageIds.begin()){
				stream << std::endl;
			}

			std::string packageName = GetPackageName(packageId);

			NextLine(stream);
			stream << "m_package" << packageId << "InfoPtr.SetPtr(new C" << packageName << "(this));";
			NextLine(stream);
			stream << "if (m_package" << packageId << "InfoPtr.IsValid()){";
			ChangeIndent(1);
			NextLine(stream);
			stream << "RegisterEmbeddedComponentInfo(\"" << packageId << "\", m_package" << packageId << "InfoPtr.GetPtr());";
			ChangeIndent(-1);
			NextLine(stream);
			stream << "}";
		}
	}

	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";

	stream << std::endl << std::endl;

	NextLine(stream);
	stream << "// reimplemented (icomp::IRegistriesManager)";
	stream << std::endl;
	NextLine(stream);
	stream << "const icomp::IRegistry* " << className << "::CLocalEnvironmentManager::GetRegistry(const icomp::CComponentAddress& address, const icomp::IRegistry* contextRegistryPtr) const";
	NextLine(stream);
	stream << "{";
	ChangeIndent(1);

	NextLine(stream);
	stream << "RegistriesMap::const_iterator findIter = m_registriesMap.find(address);";
	NextLine(stream);
	stream << "if (findIter != m_registriesMap.end()){";
	ChangeIndent(1);

	NextLine(stream);
	stream << "return findIter->second.GetPtr();";

	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";

	NextLine(stream);
	stream << "return BaseClass::GetRegistry(address, contextRegistryPtr);";

	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";

	stream << std::endl << std::endl;

	return !stream.fail();
}


bool CRegistryCodeSaverComp::WriteDependencies(
			const std::string& className,
			const Addresses& composedAddresses,
			const Addresses& realAddresses,
			std::ofstream& stream) const
{
	if (!m_packagesManagerCompPtr.IsValid()){
		return false;
	}

	QDir baseDir;
	if (m_baseDependenciesPathCompPtr.IsValid()){
		baseDir.setPath(m_baseDependenciesPathCompPtr->GetPath());
	}

	stream << "#dependencies of class " << className << std::endl;
	if (!composedAddresses.empty() && !realAddresses.empty()){
		stream << "ARXC_DEPENDENCIES +=";

		for (		Addresses::const_iterator addressIter = composedAddresses.begin();
					addressIter != composedAddresses.end();
					++addressIter){
			const icomp::CComponentAddress& address = *addressIter;
			const std::string& packageId = address.GetPackageId();

			QString packagePath = m_packagesManagerCompPtr->GetPackagePath(packageId);
			if (packagePath.isEmpty()){
				SendErrorMessage(
							MI_UNKNOWN_PACKAGE,
							QObject::tr("Composed package '%1' cannot be found").arg(packageId.c_str()));
				return false;
			}

			stream << " \\" << std::endl;
			stream << "\t" << baseDir.relativeFilePath(packagePath).toStdString() << "/" << address.GetComponentId() << ".arx";
		}

		Ids packageIdsList;
		for (		Addresses::const_iterator addressIter = realAddresses.begin();
					addressIter != realAddresses.end();
					++addressIter){
			const icomp::CComponentAddress& address = *addressIter;
			packageIdsList.insert(address.GetPackageId());
		}

		for (		Ids::const_iterator packageIter = packageIdsList.begin();
					packageIter != packageIdsList.end();
					++packageIter){
			const std::string& packageId = *packageIter;

			QString packagePath = m_packagesManagerCompPtr->GetPackagePath(packageId);
			if (packagePath.isEmpty()){
				SendErrorMessage(
							MI_UNKNOWN_PACKAGE,
							QObject::tr("Package '%1' cannot be found").arg(packageId.c_str()));
				return false;
			}

			stream << " \\" << std::endl;
			stream << "\t" << baseDir.relativeFilePath(packagePath).toStdString();
		}
	}

	if (m_extPackagesManagerCompPtr.IsValid()){
		icomp::IExtPackagesManager::PathList configFilesList = m_extPackagesManagerCompPtr->GetConfigurationPathList(icomp::IExtPackagesManager::PT_CONFIG);
		for (		icomp::IExtPackagesManager::PathList::const_iterator pathIter = configFilesList.begin();
					pathIter != configFilesList.end();
					++pathIter){
			const QString& configFilePath = *pathIter;

			stream << " \\" << std::endl;
			stream << "\t" << baseDir.relativeFilePath(configFilePath).toStdString();
		}
	}

	stream << std::endl;

	return true;
}


bool CRegistryCodeSaverComp::WriteRegistryInfo(
			const icomp::IRegistry& registry,
			const std::string& registryCallPrefix,
			std::ofstream& stream) const
{
	icomp::IRegistry::Ids ids = registry.GetElementIds();
	if (!ids.empty()){
		for (		icomp::IRegistry::Ids::const_iterator elementIter = ids.begin();
					elementIter != ids.end();
					++elementIter){
			const std::string& componentId = *elementIter;

			const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(componentId);
			I_ASSERT(infoPtr != NULL);	// used element ID was returned by registry, info must exist.

			NextLine(stream);
			stream << "// element '" << componentId << "' of type " << infoPtr->address.GetPackageId() << "::" << infoPtr->address.GetComponentId();

			if (!WriteComponentInfo(registry, registryCallPrefix, componentId, *infoPtr, stream)){
				return false;
			}
		}
	}

	const icomp::IRegistry::ExportedInterfacesMap& interfacesMap = registry.GetExportedInterfacesMap();
	if (!interfacesMap.empty()){
		NextLine(stream);
		stream << "// interface export";
		for (		icomp::IRegistry::ExportedInterfacesMap::const_iterator interfaceIter = interfacesMap.begin();
					interfaceIter != interfacesMap.end();
					++interfaceIter){
			const std::string& interfaceName = interfaceIter->first;
			const std::string& componentName = interfaceIter->second;

			NextLine(stream);
			stream << registryCallPrefix << "SetElementInterfaceExported(";
			NextLine(stream);
			stream << "\t\t\t\"" << componentName << "\",";
			NextLine(stream);
			stream << "\t\t\t\"" << interfaceName << "\",";
			NextLine(stream);
			stream << "\t\t\ttrue);";
		}
	}

	const icomp::IRegistry::ExportedComponentsMap& componentsMap = registry.GetExportedComponentsMap();
	if (!componentsMap.empty()){
		stream << std::endl;
		NextLine(stream);
		stream << "// component export";
		for (		icomp::IRegistry::ExportedComponentsMap::const_iterator componentIter = componentsMap.begin();
					componentIter != componentsMap.end();
					++componentIter){
			const std::string& exportedName = componentIter->first;
			const std::string& componentName = componentIter->second;

			NextLine(stream);
			stream << registryCallPrefix << "SetElementExported(";
			NextLine(stream);
			stream << "\t\t\t\"" << exportedName << "\",";
			NextLine(stream);
			stream << "\t\t\t\"" << componentName << "\");";
		}
	}


	icomp::IRegistry::Ids embeddedIds = registry.GetEmbeddedRegistryIds();
	for (		icomp::IRegistry::Ids::const_iterator embeddedIter = embeddedIds.begin();
				embeddedIter != embeddedIds.end();
				++embeddedIter){
		const std::string& id = *embeddedIter;

		const icomp::IRegistry* embeddedRegistryPtr = registry.GetEmbeddedRegistry(id);
		if (embeddedRegistryPtr != NULL){
			stream << std::endl;

			std::string embeddedRegName = "embedded" + id + "Ptr";

			NextLine(stream);
			stream << "icomp::IRegistry* " << embeddedRegName << " = " << registryCallPrefix << "InsertEmbeddedRegistry(\"" << id << "\");";
			NextLine(stream);
			stream << "if (" << embeddedRegName << " != NULL){";
			ChangeIndent(1);

			if (!WriteRegistryInfo(*embeddedRegistryPtr, embeddedRegName + "->", stream)){
				return false;
			}

			ChangeIndent(-1);
			NextLine(stream);
			stream << "}";
		}
	}

	return !stream.fail();
}


bool CRegistryCodeSaverComp::WriteComponentInfo(
			const icomp::IRegistry& /*registry*/,
			const std::string& registryCallPrefix,
			const std::string& componentId,
			const icomp::IRegistry::ElementInfo& componentInfo,
			std::ofstream& stream) const
{
	std::string elementInfoName = "info" + componentId + "Ptr";

	if (componentInfo.elementPtr.IsValid()){
		icomp::IRegistryElement& component = *componentInfo.elementPtr;
		icomp::IRegistryElement::Ids attributeIds = component.GetAttributeIds();

		I_DWORD componentFlags = component.GetElementFlags();

		NextLine(stream);
		if (!attributeIds.empty() || (componentFlags != 0)){
			stream << "icomp::IRegistry::ElementInfo* " << elementInfoName << " = ";
		}

		stream << registryCallPrefix << "InsertElementInfo(";
		NextLine(stream);
		stream << "\t\t\t\"" << componentId << "\",";
		NextLine(stream);
		stream << "\t\t\ticomp::CComponentAddress(\"" << componentInfo.address.GetPackageId();
		stream << "\", \"" << componentInfo.address.GetComponentId() << "\"),";
		NextLine(stream);
		stream << "\t\t\ttrue);";

		if (!attributeIds.empty() || (componentFlags != 0)){
			NextLine(stream);
			stream << "if ((" << elementInfoName << " != NULL) && " << elementInfoName << "->elementPtr.IsValid()){";
			ChangeIndent(1);

			if (componentFlags != 0){
				NextLine(stream);
				stream << elementInfoName << "->elementPtr->SetElementFlags(" << componentFlags << ");";
			}

			if (!attributeIds.empty()){
				if (componentFlags != 0){
					stream << std::endl;
				}

				for (		icomp::IRegistryElement::Ids::const_iterator attributeIter = attributeIds.begin();
							attributeIter != attributeIds.end();
							++attributeIter){
					const std::string& attributeId = *attributeIter;

					const icomp::IRegistryElement::AttributeInfo* attrInfoPtr = component.GetAttributeInfo(attributeId);

					if (attributeIter != attributeIds.begin()){
						stream << std::endl;
					}

					NextLine(stream);
					stream << "// create and set attribute value for '" << attributeId << "'";
					if (attrInfoPtr != NULL){
						std::string attributeInfoName = "attrInfo" + attributeId + "Ptr";

						bool isAttributeValid = attrInfoPtr->attributePtr.IsValid();

						NextLine(stream);
						stream << "icomp::IRegistryElement::AttributeInfo* " << attributeInfoName << " = ";
						stream << elementInfoName << "->elementPtr->InsertAttributeInfo(\"" << attributeId << "\", \"" << attrInfoPtr->attributeTypeName << "\");";

						NextLine(stream);
						stream << "if (" << attributeInfoName << " != NULL){";
						ChangeIndent(1);

						if (!attrInfoPtr->exportId.empty()){
							NextLine(stream);
							stream << attributeInfoName << "->exportId = \"" << attrInfoPtr->exportId << "\";";
						}

						if (isAttributeValid){
							if (!WriteAttribute(attributeId, componentId, attributeInfoName, *attrInfoPtr->attributePtr, stream)){
								return false;
							}
						}

						ChangeIndent(-1);
						NextLine(stream);
						stream << "}";
					}
				}
			}

			ChangeIndent(-1);
			NextLine(stream);
			stream << "}" << std::endl;
		}
	}

	return !stream.fail();
}


bool CRegistryCodeSaverComp::WriteAttribute(
			const std::string& attributeId,
			const std::string& componentId,
			const std::string& attributeInfoName,
			const iser::IObject& attribute,
			std::ofstream& stream) const
{
	NextLine(stream);
	stream << attributeInfoName << "->attributePtr.SetPtr(new " << attribute.GetFactoryId() << ");";

	NextLine(stream);
	stream << "I_ASSERT(" << attributeInfoName << "->attributePtr.IsValid());";
	stream << std::endl;

	std::string attributeType;
	std::string valueString;
	std::list<std::string> valueStrings;

	std::string attributeName = "attr" + attributeId + "Ptr";

	if (GetAttributeValue(attributeId, componentId, attribute, valueString, attributeType)){
		NextLine(stream);
		stream << attributeType << "* " << attributeName << " = dynamic_cast<" << attributeType << "*>(" << attributeInfoName << "->attributePtr.GetPtr());";
		NextLine(stream);
		stream << "I_ASSERT(" << attributeName << " != NULL);";
		NextLine(stream);
		stream << attributeName << "->SetValue(" << valueString << ");";
	}
	else if (GetMultiAttributeValue(attributeId, componentId, attribute, valueStrings, attributeType)){
		if (!valueStrings.empty()){
			NextLine(stream);
			stream << attributeType << "* n" << attributeInfoName << " = dynamic_cast<" << attributeType << "*>(" << attributeInfoName << "->attributePtr.GetPtr());";
			NextLine(stream);
			stream << "I_ASSERT(n" << attributeInfoName << " != NULL);";
			stream << std::endl;
			NextLine(stream);
			stream << "n" << attributeInfoName << "->Reset();";

			for (		std::list<std::string>::const_iterator iter = valueStrings.begin();
						iter != valueStrings.end();
						++iter){
				NextLine(stream);
				stream << "n" << attributeInfoName << "->InsertValue(" << *iter << ");";
			}
		}
	}
	else{
		SendErrorMessage(
					MI_UNDEFINED_ATTR_TYPE,
					QObject::tr("Unknown attribute type: %1").arg(attributeType.c_str()));
		return false;
	}

	return true;
}


bool CRegistryCodeSaverComp::WriteRegistryClassDeclaration(
			const std::string& /*baseClassName*/,
			const std::string& registryClassName,
			const icomp::IRegistry& registry,
			std::ofstream& stream) const
{
	QString description = registry.GetDescription();

	if (!description.isEmpty()){
		NextLine(stream);
		stream << "/**";

		NextLine(stream);
		stream << "\t" << description.toStdString() << ".";

		NextLine(stream);
		stream << "*/";
	}

	NextLine(stream);
	stream << "class " << registryClassName << ": public icomp::CRegistry";
	NextLine(stream);
	stream << "{";

	NextLine(stream);
	stream << "public:";
	ChangeIndent(1);

	NextLine(stream);
	stream << registryClassName << "();";

	ChangeIndent(-1);
	NextLine(stream);
	stream << "};";

	return true;
}


bool CRegistryCodeSaverComp::WriteRegistryClassBody(
			const std::string& baseClassName,
			const std::string& registryClassName,
			const icomp::IRegistry& registry,
			std::ofstream& stream) const
{
	NextLine(stream);
	stream << "// Embedded class " << registryClassName;

	stream << std::endl;

	NextLine(stream);
	stream << baseClassName << "::" << registryClassName << "::" << registryClassName << "()";

	NextLine(stream);
	stream << "{";
	ChangeIndent(1);

	if (!WriteRegistryInfo(registry, "", stream)){
		return false;
	}

	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";

	return true;
}


bool CRegistryCodeSaverComp::GetAttributeValue(
			const std::string& attributeId,
			const std::string& componentId,
			const iser::ISerializable& attribute,
			std::string& valueString,
			std::string& typeName) const
{
	const icomp::CBoolAttribute* boolAttribute = dynamic_cast<const icomp::CBoolAttribute*>(&attribute);
	if (boolAttribute != NULL){
		valueString = boolAttribute->GetValue()? "true": "false";
		typeName = "icomp::CBoolAttribute";

		return true;
	}

	const icomp::CDoubleAttribute* doubleAttribute = dynamic_cast<const icomp::CDoubleAttribute*>(&attribute);
	if (doubleAttribute != NULL){
		valueString = QString("%1").arg(doubleAttribute->GetValue()).toStdString();
		typeName = "icomp::CDoubleAttribute";

		return true;
	}

	const icomp::CIntAttribute* intAttribute = dynamic_cast<const icomp::CIntAttribute*>(&attribute);
	if (intAttribute != NULL){
		valueString = QString("%1").arg(intAttribute->GetValue()).toStdString();
		typeName = "icomp::CIntAttribute";

		return true;
	}

	const icomp::CStringAttribute* stringAttribute = dynamic_cast<const icomp::CStringAttribute*>(&attribute);
	if (stringAttribute != NULL){
		valueString = "tr(" + GetStringLiteral(stringAttribute->GetValue()) + ", \"" + componentId + "/" + attributeId + "\")";
		typeName = "icomp::CStringAttribute";

		return true;
	}

	const icomp::CStdStringAttribute* stdStringAttributePtr = dynamic_cast<const icomp::CStdStringAttribute*>(&attribute);
	if (stdStringAttributePtr != NULL){		
		valueString = GetStdStringLiteral(stdStringAttributePtr->GetValue());
		typeName = "icomp::CStdStringAttribute";

		return true;
	}

	return false;
}


bool CRegistryCodeSaverComp::GetMultiAttributeValue(
			const std::string& attributeId,
			const std::string& componentId,
			const iser::ISerializable& attribute,
			std::list<std::string>& valueStrings,
			std::string& typeName) const
{
	valueStrings.clear();

	const icomp::CMultiBoolAttribute* boolListAttribute = dynamic_cast<const icomp::CMultiBoolAttribute*>(&attribute);
	if (boolListAttribute != NULL){
		for (int index = 0; index < boolListAttribute->GetValuesCount(); index++){
			valueStrings.push_back(boolListAttribute->GetValueAt(index)? "true": "false");
		}

		typeName = "icomp::CMultiBoolAttribute";

		return true;
	}

	const icomp::CMultiIntAttribute* intListAttribute = dynamic_cast<const icomp::CMultiIntAttribute*>(&attribute);
	if (intListAttribute != NULL){
		for (int index = 0; index < intListAttribute->GetValuesCount(); index++){
			valueStrings.push_back(QString("%1").arg(intListAttribute->GetValueAt(index)).toStdString());
		}

		typeName = "icomp::CMultiIntAttribute";

		return true;
	}

	const icomp::CMultiDoubleAttribute* doubleListAttribute = dynamic_cast<const icomp::CMultiDoubleAttribute*>(&attribute);
	if (doubleListAttribute != NULL){
		for (int index = 0; index < doubleListAttribute->GetValuesCount(); index++){
			valueStrings.push_back(QString("%1").arg(doubleListAttribute->GetValueAt(index)).toStdString());
		}

		typeName = "icomp::CMultiDoubleAttribute";

		return true;
	}

	const icomp::CMultiStringAttribute* stringListAttribute = dynamic_cast<const icomp::CMultiStringAttribute*>(&attribute);
	if (stringListAttribute != NULL){
		for (int index = 0; index < stringListAttribute->GetValuesCount(); index++){
			valueStrings.push_back("tr(" + GetStringLiteral(stringListAttribute->GetValueAt(index)) + ", \"" + componentId + "/" + attributeId + "\")");
		}

		typeName = "icomp::CMultiStringAttribute";

		return true;
	}

	const icomp::CMultiStdStringAttribute* multiStdStringAttributePtr = dynamic_cast<const icomp::CMultiStdStringAttribute*>(&attribute);
	if (multiStdStringAttributePtr != NULL){
		for (int index = 0; index < multiStdStringAttributePtr->GetValuesCount(); index++){
			valueStrings.push_back(GetStdStringLiteral(multiStdStringAttributePtr->GetValueAt(index)));
		}

		typeName = "icomp::CMultiStdStringAttribute";

		return true;
	}

	return false;
}


bool CRegistryCodeSaverComp::NextLine(std::ofstream& stream) const
{
	stream << std::endl;

	for (int i = 0; i < m_indentCount; ++i){
		stream << "\t";
	}

	return !stream.fail();
}


int CRegistryCodeSaverComp::ChangeIndent(int difference) const
{
	m_indentCount += difference;

	I_ASSERT(m_indentCount >= 0);

	return m_indentCount;
}


bool CRegistryCodeSaverComp::ExtractInfoFromFile(const QString& filePath, std::string& className, QString& headerFilePath) const
{
	QFileInfo fileInfo(filePath);

	className = fileInfo.baseName().toStdString();

	headerFilePath = fileInfo.absolutePath() + "/" + QString::fromStdString(className);
	headerFilePath = QDir::toNativeSeparators(headerFilePath);

	return (!className.empty());
}


std::string CRegistryCodeSaverComp::GetPackageName(const std::string& packageId) const
{
	if ((packageId.length() >= 3) && (packageId.substr(packageId.length() - 3) == "Pck")){
		return packageId;
	}
	else{
		return packageId + "Pck";
	}
}


std::string CRegistryCodeSaverComp::GetStringLiteral(const QString& text) const
{
	static const char* hexCiphers = "0123456789ABCDEF";

	std::string retVal("\"");

	for (		QString::const_iterator iter = text.begin();
				iter != text.end();
				++iter){
		wchar_t c = (*iter).unicode();

		if (c == '\a'){
			retVal += "\\a";
		}
		else if (c == '\b'){
			retVal += "\\b";
		}
		else if (c == '\f'){
			retVal += "\\f";
		}
		else if (c == '\n'){
			retVal += "\\n";
		}
		else if (c == '\r'){
			retVal += "\\r";
		}
		else if (c == '\t'){
			retVal += "\\t";
		}
		else if (c == '\\'){
			retVal += "\\\\";
		}
		else if (c == '\"'){
			retVal += "\\\"";
		}
		else if (c >= 255){
			retVal += ".";
		}
		else if ((c >= 128) || (c < 32)){
			retVal += "\\x";
			retVal += hexCiphers[(c >> 4) & 15];
			retVal += hexCiphers[c & 15];
		}
		else{
			retVal += char(c);
		}
	}

	retVal += "\"";

	return retVal;
}


std::string CRegistryCodeSaverComp::GetStdStringLiteral(const std::string& text) const
{
	static const char* hexCiphers = "0123456789ABCDEF";

	std::string retVal("\"");

	for (		std::string::const_iterator iter = text.begin();
				iter != text.end();
				++iter){
		std::string::value_type c = *iter;

		if (c == '\a'){
			retVal += "\\a";
		}
		else if (c == '\b'){
			retVal += "\\b";
		}
		else if (c == '\f'){
			retVal += "\\f";
		}
		else if (c == '\n'){
			retVal += "\\n";
		}
		else if (c == '\r'){
			retVal += "\\r";
		}
		else if (c == '\t'){
			retVal += "\\t";
		}
		else if (c == '\\'){
			retVal += "\\\\";
		}
		else if (c == '\"'){
			retVal += "\\\"";
		}
		else if ((c >= 128) || (c < 32)){
			retVal += "\\x";
			retVal += hexCiphers[(c >> 4) & 15];
			retVal += hexCiphers[c & 15];
		}
		else{
			retVal += char(c);
		}
	}

	retVal += "\"";

	return retVal;
}


} // namespace iqt


