#include <ipackage/CRegistryCodeSaverComp.h>


// STL includes
#include <iostream>

// Qt includes
#include <QtCore/QObject>
#include <QtCore/QDir>

// ACF includes
#include <icomp/icomp.h>
#include <icomp/CCachedEnvironmentManager.h>
#include <iser/CMemoryWriteArchive.h>
#include <ibase/IProgressManager.h>


namespace ipackage
{


CRegistryCodeSaverComp::CRegistryCodeSaverComp()
:	m_indentCount(0)
{
}


// reimplemented (ifile::IFilePersistence)

bool CRegistryCodeSaverComp::IsOperationSupported(
		const istd::IChangeable* dataObjectPtr,
		const QString* filePathPtr,
		int flags,
		bool /*beQuiet*/) const
{
	if (dynamic_cast<const icomp::IRegistry*>(dataObjectPtr) == NULL){	// check if object type is supported
		return false;
	}

	if (		(*m_workingModeAttrPtr == WM_SOURCES) &&
				(filePathPtr != NULL) &&
				(filePathPtr->isEmpty())){	// check if path is set for source generating
		return false;
	}

	return ((flags & QF_SAVE) != 0) && ((flags & QF_FILE) != 0);
}


int CRegistryCodeSaverComp::LoadFromFile(
			istd::IChangeable& /*data*/,
			const QString& /*filePath*/,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	return OS_FAILED;
}


int CRegistryCodeSaverComp::SaveToFile(
			const istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	const icomp::IRegistry* registryPtr = dynamic_cast<const icomp::IRegistry*>(&data);
	if (registryPtr == NULL){
		SendErrorMessage(0, "No valid component diagram", "Source code generator");
		return OS_FAILED;
	}

	Addresses realAddresses;
	Addresses composedAddresses;
	if (!AppendAddresses(*registryPtr, QStringList(), realAddresses, composedAddresses)){
		SendErrorMessage(0, "Component tree could not be created", "Source code generator");

		return OS_FAILED;
	}

	if (*m_workingModeAttrPtr == WM_SOURCES){
		QByteArray className;
		QString baseFilePath;
		if (!ExtractInfoFromFile(filePath, className, baseFilePath)){
			SendErrorMessage(0, "Class name could not be extracted", "Source code generator");

			return OS_FAILED;
		}

		QFile headerFile(baseFilePath + ".h");
		QFile codeFile(filePath);

		if (		!codeFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) ||
					!headerFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
			headerFile.remove();
			codeFile.remove();

			SendErrorMessage(0, "Output files could not be created", "Source code generator");

			return OS_FAILED;
		}

		QTextStream headerStream(&headerFile);
		QTextStream codeStream(&codeFile);
		if (		!WriteHeader(className, *registryPtr, composedAddresses, realAddresses, headerStream) ||
					!WriteIncludes(className, realAddresses, codeStream) ||
					!WriteClassDefinitions(className, *registryPtr, composedAddresses, realAddresses, codeStream)){

			headerFile.remove();
			codeFile.remove();

			SendErrorMessage(0, "Source code components could not be written", "Source code generator");

			return OS_FAILED;
		}
	}
	else if ((*m_workingModeAttrPtr == WM_DEPENDENCIES) || (*m_workingModeAttrPtr == WM_DEEP_DEPENDENCIES)){
		bool allDependencies = (*m_workingModeAttrPtr == WM_DEEP_DEPENDENCIES);

		if (filePath.isEmpty()){
			QTextStream depsStream(stdout);
			if (!WriteDependencies(composedAddresses, realAddresses, allDependencies, depsStream)){
				SendErrorMessage(0, "Build dependencies could not be written", "Source code generator");

				return OS_FAILED;
			}
		}
		else{
			QFile depsFile(filePath);

			if (!depsFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
				depsFile.remove();
				SendErrorMessage(0, "Dependency file could not be opened", "Source code generator");

				return OS_FAILED;
			}

			QTextStream depsStream(&depsFile);
			if (!WriteDependencies(composedAddresses, realAddresses, allDependencies, depsStream)){
				depsFile.remove();

				SendErrorMessage(0, "Build dependencies could not be written", "Source code generator");

				return OS_FAILED;
			}
		}
	}

	return OS_OK;
}


// reimplemented (ifile::IFileTypeInfo)

bool CRegistryCodeSaverComp::GetFileExtensions(QStringList& result, const istd::IChangeable* /*dataObjectPtr*/, int flags, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	if ((flags & QF_SAVE) != 0){
		switch (*m_workingModeAttrPtr){
		case WM_DEPENDENCIES:
		case WM_DEEP_DEPENDENCIES:
			result.push_back("txt");
			break;

		default:
				result.push_back("cpp"); // default
				result.push_back("C"); // capital C for C++, not C
			break;
		}
	}

	return true;
}


QString CRegistryCodeSaverComp::GetTypeDescription(const QString* extensionPtr) const
{
	if (extensionPtr == NULL){
		switch (*m_workingModeAttrPtr){
		case WM_DEPENDENCIES:
		case WM_DEEP_DEPENDENCIES:
			return QObject::tr("dependency file");

		default:
			return QObject::tr("C++ source file");
		}
	}

	if (		(extensionPtr->compare("c", Qt::CaseInsensitive) == 0) ||
				(extensionPtr->compare("cpp", Qt::CaseInsensitive) == 0)){
		return QObject::tr("C++ source file");
	}
	else if (extensionPtr->compare("txt", Qt::CaseInsensitive) == 0){
		return QObject::tr("qmake dependency file");
	}

	return "";
}


// protected methods

bool CRegistryCodeSaverComp::AppendAddresses(
			const icomp::IRegistry& registry,
			const QStringList& registryPath,
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
		const QByteArray& componentId = *elementIter;

		QStringList componentPath = registryPath;
		componentPath << componentId;

		const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(componentId);
		Q_ASSERT(infoPtr != NULL);	// used element ID was returned by registry, info must exist.

		const QByteArray& packageId = infoPtr->address.GetPackageId();

		if (!packageId.isEmpty()){
			// real and composite components
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

						if (!AppendAddresses(*registryPtr, componentPath, realAddresses, composedAddresses)){
							return false;
						}
					}
					else{
						SendErrorMessage(
									MI_UNDEFINED_COMPONENT,
									QObject::tr("%1: Composite component '%2' is undefined").arg(componentPath.join("/")).arg(infoPtr->address.ToString()));

						return false;
					}
				}
				break;

			default:
				if (*m_workingModeAttrPtr != WM_DEPENDENCIES){
					SendErrorMessage(
								MI_UNDEFINED_PACKAGE,
								QObject::tr("%1: Package '%2' is undefined").arg(componentPath.join("/")).arg(QString(packageId)));
					return false;
				}
				break;
			}
		}
		else{
			// embedded components will be processed recursive
			const icomp::IRegistry* registryPtr = registry.GetEmbeddedRegistry(infoPtr->address.GetComponentId());
			if (registryPtr != NULL){
				if (!AppendAddresses(*registryPtr, componentPath, realAddresses, composedAddresses)){
					return false;
				}
			}
			else{
				SendErrorMessage(
						MI_UNDEFINED_COMPONENT,
						QObject::tr("%1: Embedded component '%2' is undefined").arg(componentPath.join("/")).arg(infoPtr->address.ToString()));

				return false;
			}
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


CRegistryCodeSaverComp::Ids CRegistryCodeSaverComp::ExtractComponentIds(const Addresses& addresses, const QByteArray& packageId) const
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
			const QByteArray& className,
			const icomp::IRegistry& registry,
			const Addresses& composedAddresses,
			const Addresses& realAddresses,
			QTextStream& stream) const
{
	QByteArray includeDefine = className + "_included";

	stream << "#ifndef " << includeDefine;

	NextLine(stream);
	stream << "#define " << includeDefine;
	stream << "\n\n";

	NextLine(stream);
	stream << "// ACF includes";
	NextLine(stream);
	stream << "#include \"istd/TDelPtr.h\"";
	NextLine(stream);
	stream << "#include \"icomp/CRegistry.h\"";
	NextLine(stream);
	stream << "#include \"icomp/CCompositeComponent.h\"";
	NextLine(stream);
	stream << "#include \"icomp/CCompositeComponentContext.h\"";
	NextLine(stream);
	if (*m_useBinaryCodeAttrPtr){
		stream << "#include \"icomp/CCachedEnvironmentManager.h\"";
	}
	else{
		stream << "#include \"icomp/CCompositeComponentStaticInfo.h\"";
	}
	NextLine(stream);
	stream << "#include \"icomp/CPackageStaticInfo.h\"";
	if (*m_useBinaryCodeAttrPtr){
		NextLine(stream);
		stream << "#include \"icomp/CCachedEnvironmentManager.h\"";
	}
	else{
		NextLine(stream);
		stream << "#include \"icomp/CCompositePackageStaticInfo.h\"" << "\n";
		NextLine(stream);
		stream << "#include \"icomp/CEnvironmentManagerBase.h\"";
	}

	stream << "\n\n";

	QString description = registry.GetDescription();
	if (!description.isEmpty()){
		NextLine(stream);
		stream << "/**";

		NextLine(stream);
		stream << "\t" << description.toLocal8Bit() << ".";

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
	stream << "InterfaceType* GetInterface(const QByteArray& subId = \"\"){return GetComponentInterface<InterfaceType>(subId);}";
	stream << "\n";

	NextLine(stream);
	stream << className << "(const icomp::IRegistryElement* userMainElementPtr = NULL);";
	stream << "\n";

	ChangeIndent(-1);
	NextLine(stream);
	stream << "protected:";
	ChangeIndent(1);

	if (!WriteRegistryClassDeclaration(className, "CMainRegistry", registry, stream)){
		return false;
	}

	Ids composedPackageIds = ExtractPackageIds(composedAddresses);
	if (m_registriesManagerCompPtr.IsValid() && !composedPackageIds.isEmpty() && !*m_useBinaryCodeAttrPtr){
		stream << "\n";

		NextLine(stream);
		stream << "//\tcomposite packages";

		for (		Ids::const_iterator packageIter = composedPackageIds.begin();
					packageIter != composedPackageIds.end();
					++packageIter){
			const QByteArray& packageId = *packageIter;
			if (packageId.isEmpty()){
				continue;
			}

			QByteArray packageName = GetPackageName(packageId);

			stream << "\n";

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
				const QByteArray& componentId = *componentIter;

				icomp::CComponentAddress address(packageId, componentId);

				const icomp::IRegistry* registryPtr = m_registriesManagerCompPtr->GetRegistry(address, &registry);

				if (registryPtr != NULL){
					stream << "\n";
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
	if (!realPackageIds.isEmpty()){
		stream << "\n";

		NextLine(stream);
		stream << "//\treal packages";

		for (		Ids::const_iterator packageIter = realPackageIds.begin();
					packageIter != realPackageIds.end();
					++packageIter){
			const QByteArray& packageId = *packageIter;
			if (packageId.isEmpty()){
				continue;
			}

			QByteArray packageName = GetPackageName(packageId);

			stream << "\n";

			NextLine(stream);
			stream << "class C" << packageName << ": public icomp::CPackageStaticInfo";

			NextLine(stream);
			stream << "{";

			NextLine(stream);
			stream << "public:";
			ChangeIndent(1);

			NextLine(stream);
			stream << "typedef icomp::CPackageStaticInfo BaseClass;";
			stream << "\n";

			NextLine(stream);
			stream << "C" << packageName << "();";

			ChangeIndent(-1);
			NextLine(stream);
			stream << "};";
		}
	}

	stream << "\n";

	NextLine(stream);
	if (*m_useBinaryCodeAttrPtr){
		stream << "class CLocalEnvironmentManager: public icomp::CCachedEnvironmentManager";
	}
	else{
		stream << "class CLocalEnvironmentManager: public icomp::CEnvironmentManagerBase";
	}
	NextLine(stream);
	stream << "{";
	NextLine(stream);
	stream << "public:";
	ChangeIndent(1);

	NextLine(stream);
	stream << "typedef icomp::CEnvironmentManagerBase BaseClass;";
	stream << "\n";

	NextLine(stream);
	stream << "CLocalEnvironmentManager();";
	stream << "\n";

	if (!*m_useBinaryCodeAttrPtr){
		NextLine(stream);
		stream << "// reimplemented (icomp::IRegistriesManager)";
		NextLine(stream);
		stream << "virtual const icomp::IRegistry* GetRegistry(const icomp::CComponentAddress& address, const icomp::IRegistry* contextRegistryPtr = NULL) const;";

		stream << "\n";
		ChangeIndent(-1);
		NextLine(stream);
		stream << "protected:";
		ChangeIndent(1);

		NextLine(stream);
		stream << "typedef istd::TDelPtr<icomp::IRegistry> RegistryPtr;";
		NextLine(stream);
		stream << "typedef QMap<icomp::CComponentAddress, RegistryPtr> RegistriesMap;";
		stream << "\n";
	}

	ChangeIndent(-1);
	NextLine(stream);
	stream << "private:";
	ChangeIndent(1);

	for (		Ids::const_iterator packageInstanceIter = realPackageIds.begin();
				packageInstanceIter != realPackageIds.end();
				++packageInstanceIter){
		const QByteArray& packageId = *packageInstanceIter;
		if (packageId.isEmpty()){
			continue;
		}

		QByteArray packageName = GetPackageName(packageId);

		NextLine(stream);
		stream << "C" << packageName << " m_sub" << packageName << ";";
	}

	if (!*m_useBinaryCodeAttrPtr){
		stream << "\n";

		if (!composedPackageIds.isEmpty()){
			for (		Ids::const_iterator registerPackageIter = composedPackageIds.begin();
						registerPackageIter != composedPackageIds.end();
						++registerPackageIter){
				const QByteArray& packageId = *registerPackageIter;
				if (packageId.isEmpty()){
					continue;
				}

				QByteArray packageName = GetPackageName(packageId);

				NextLine(stream);
				stream << "istd::TDelPtr<icomp::IComponentStaticInfo> m_package" << packageId << "InfoPtr;";
			}

			stream << "\n";
		}

		NextLine(stream);
		stream << "RegistriesMap m_registriesMap;";
	}

	ChangeIndent(-1);
	NextLine(stream);
	stream << "};";
	stream << "\n";

	ChangeIndent(-1);
	NextLine(stream);
	stream << "private:";
	ChangeIndent(1);

	NextLine(stream);
	stream << "istd::TDelPtr<icomp::CCompositeComponentContext> m_mainContextPtr;";

	ChangeIndent(-1);
	NextLine(stream);
	stream << "};";

	stream << "\n\n";

	NextLine(stream);
	stream << "#endif // !" << includeDefine;

	stream << "\n\n\n";

	return stream.status() == QTextStream::Ok;
}


bool CRegistryCodeSaverComp::WriteIncludes(
			const QByteArray& className,
			const Addresses& addresses,
			QTextStream& stream) const
{
	stream << "#include \"" << className << ".h\"" << "\n";
	stream << "\n\n";
	stream << "// ACF includes" << "\n";
	if (*m_useBinaryCodeAttrPtr){
		stream << "#include \"iser/CMemoryReadArchive.h\"" << "\n";
	}
	stream << "#include \"icomp/CRegistryElement.h\"" << "\n";

	Ids packageIds = ExtractPackageIds(addresses);
	if (!packageIds.isEmpty()){
		stream << "\n";
		stream << "// ACF component includes" << "\n";

		for (		Ids::const_iterator packageIter = packageIds.begin();
					packageIter != packageIds.end();
					++packageIter){
			const QByteArray& packageId = *packageIter;
			if (packageId.isEmpty()){
				continue;
			}

			stream << "#include \"" << packageId << "/" << packageId << ".h\"" << "\n";
		}
	}

	stream << "\n";

	return stream.status() == QTextStream::Ok;
}


bool CRegistryCodeSaverComp::WriteClassDefinitions(
			const QByteArray& className,
			const icomp::IRegistry& registry,
			const Addresses& composedAddresses,
			const Addresses& realAddresses,
			QTextStream& stream) const
{
	NextLine(stream);
	stream << className << "::" << className << "(const icomp::IRegistryElement* userMainElementPtr)";

	NextLine(stream);
	stream << "{";
	ChangeIndent(1);

	NextLine(stream);
	stream << "static icomp::CRegistryElement mainElement;";
	NextLine(stream);
	stream << "static CMainRegistry mainRegistry;";
	NextLine(stream);
	stream << "static CLocalEnvironmentManager localEnvironmentManager;";
	stream << "\n";
	NextLine(stream);
	stream << "static icomp::CCompositeComponentStaticInfo mainComponentStaticInfo(mainRegistry, localEnvironmentManager, NULL);";
	stream << "\n";
	NextLine(stream);
	stream << "m_mainContextPtr.SetPtr(new icomp::CCompositeComponentContext((userMainElementPtr != NULL)? userMainElementPtr: &mainElement, &mainComponentStaticInfo, &mainRegistry, &localEnvironmentManager, NULL, \"\"));";
	stream << "\n";
	NextLine(stream);
	stream << "SetComponentContext(m_mainContextPtr.GetPtr(), NULL, false);";

	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";
	stream << "\n\n";

	if (!WriteRegistryClassBody(className, "CMainRegistry", registry, stream)){
		return false;
	}

	stream << "\n\n";

	Ids composedPackageIds = ExtractPackageIds(composedAddresses);

	if (!*m_useBinaryCodeAttrPtr){
		if (m_registriesManagerCompPtr.IsValid()){
			for (		Ids::const_iterator packageIter = composedPackageIds.begin();
						packageIter != composedPackageIds.end();
						++packageIter){
				const QByteArray& packageId = *packageIter;
				if (packageId.isEmpty()){
					continue;
				}

				QByteArray packageName = GetPackageName(packageId);

				NextLine(stream);
				stream << "// Embedded class C" << packageName;

				stream << "\n";

				NextLine(stream);
				stream << className << "::C" << packageName << "::C" << packageName << "(const icomp::IComponentEnvironmentManager* managerPtr)";
				NextLine(stream);
				stream << ":\ticomp::CCompositePackageStaticInfo(\"" << packageId << "\", managerPtr)";
				stream << "\n";


				NextLine(stream);
				stream << "{";
				ChangeIndent(1);

				Ids componentIds = ExtractComponentIds(composedAddresses, packageId);
				for (		Ids::const_iterator instanceRegIter = componentIds.begin();
							instanceRegIter != componentIds.end();
							++instanceRegIter){
					const QByteArray& componentId = *instanceRegIter;

					NextLine(stream);
					stream << "RegisterEmbeddedComponent(\"" << componentId << "\");";
				}

				ChangeIndent(-1);
				NextLine(stream);
				stream << "}";

				stream << "\n\n";

				for (		Ids::const_iterator componentIter = componentIds.begin();
							componentIter != componentIds.end();
							++componentIter){
					const QByteArray& componentId = *componentIter;

					icomp::CComponentAddress address(packageId, componentId);

					const icomp::IRegistry* registryPtr = m_registriesManagerCompPtr->GetRegistry(address, &registry);
					if (registryPtr != NULL){
						if (!WriteRegistryClassBody(className + "::C" + packageName, "C" + componentId + "Registry", *registryPtr, stream)){
							return false;
						}

						stream << "\n\n";
					}
				}
			}
		}
	}

	Ids realPackageIds = ExtractPackageIds(realAddresses);

	for (		Ids::const_iterator packageIter = realPackageIds.begin();
				packageIter != realPackageIds.end();
				++packageIter){
		const QByteArray& packageId = *packageIter;
		if (packageId.isEmpty()){
			continue;
		}

		QByteArray packageName = GetPackageName(packageId);

		NextLine(stream);
		stream << "// Embedded class C" << packageName;

		stream << "\n";

		NextLine(stream);
		stream << className << "::C" << packageName << "::C" << packageName << "()";

		NextLine(stream);
		stream << "{";
		ChangeIndent(1);

		Ids componentIds = ExtractComponentIds(realAddresses, packageId);
		for (		Ids::const_iterator instanceRegIter = componentIds.begin();
					instanceRegIter != componentIds.end();
					++instanceRegIter){
			const QByteArray& componentId = *instanceRegIter;

			QByteArray infoName = "info" + componentId;

			NextLine(stream);
			stream << "const icomp::IComponentStaticInfo& " << infoName << " = " << packageId << "::" << componentId << "::InitStaticInfo(NULL);";

			NextLine(stream);
			stream << "RegisterEmbeddedComponentInfo(\"" << componentId << "\", &" << infoName << ");";
		}

		ChangeIndent(-1);
		NextLine(stream);
		stream << "}";

		stream << "\n\n";
	}

	NextLine(stream);
	stream << "// Embedded class CLocalEnvironmentManager";

	stream << "\n";

	NextLine(stream);
	stream << className << "::CLocalEnvironmentManager::CLocalEnvironmentManager()";

	NextLine(stream);
	stream << "{";
	ChangeIndent(1);

	if (!realPackageIds.isEmpty()){
		NextLine(stream);
		stream << "// register real components";

		for (		Ids::const_iterator registerRealIter = realPackageIds.begin();
					registerRealIter != realPackageIds.end();
					++registerRealIter){
			const QByteArray& packageId = *registerRealIter;
			if (packageId.isEmpty()){
				continue;
			}

			QByteArray packageName = GetPackageName(packageId);

			NextLine(stream);
			stream << "RegisterEmbeddedComponentInfo(\"" << packageId << "\", &m_sub" << packageName << ");";
		}
		stream << "\n";
	}

	if (*m_useBinaryCodeAttrPtr){
		bool isTranslationFound = false;
		icomp::CCachedEnvironmentManager cachedManager;

		for (		Addresses::const_iterator addressIter = composedAddresses.begin();
					addressIter != composedAddresses.end();
					++addressIter){
			const icomp::CComponentAddress& address = *addressIter;
			Q_ASSERT(!address.GetPackageId().isEmpty());

			const icomp::IRegistry* registryPtr = m_registriesManagerCompPtr->GetRegistry(address);
			if (registryPtr != NULL){
				WriteRegistryTranslation(*registryPtr, isTranslationFound, stream);

				cachedManager.AddComposedComponent(address, *registryPtr);
			}
		}

		if (isTranslationFound){
			stream << "\n";
		}

		WriteDeserializingCode(cachedManager, stream);
	}
	else{
		if (!composedAddresses.isEmpty()){
			NextLine(stream);
			stream << "// create map for all known registries";
			for (		Addresses::const_iterator regTestIter = composedAddresses.begin();
						regTestIter != composedAddresses.end();
						++regTestIter){
				const icomp::CComponentAddress& address = *regTestIter;

				NextLine(stream);
				stream << "m_registriesMap[icomp::CComponentAddress(\"" <<
						address.GetPackageId() << "\", \"" <<
						address.GetComponentId() << "\")].SetPtr(new C" <<
						GetValidIdentifier(GetPackageName(address.GetPackageId())) <<
						"::C" << GetValidIdentifier(address.GetComponentId()) << "Registry());";
			}
			stream << "\n";
		}

		if (!composedPackageIds.isEmpty()){
			NextLine(stream);
			stream << "// register composed packages";

			for (		Ids::const_iterator registerPackageIter = composedPackageIds.begin();
						registerPackageIter != composedPackageIds.end();
						++registerPackageIter){
				const QByteArray& packageId = *registerPackageIter;
				if (packageId.isEmpty()){
					continue;
				}

				if (registerPackageIter != composedPackageIds.begin()){
					stream << "\n";
				}

				QByteArray packageName = GetPackageName(packageId);

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
	}

	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";

	if (!*m_useBinaryCodeAttrPtr){
		stream << "\n\n";

		NextLine(stream);
		stream << "// reimplemented (icomp::IRegistriesManager)";
		stream << "\n";
		NextLine(stream);
		stream << "const icomp::IRegistry* " << className << "::CLocalEnvironmentManager::GetRegistry(const icomp::CComponentAddress& address, const icomp::IRegistry* contextRegistryPtr) const";
		NextLine(stream);
		stream << "{";
		ChangeIndent(1);

		NextLine(stream);
		stream << "RegistriesMap::ConstIterator findIter = m_registriesMap.constFind(address);";
		NextLine(stream);
		stream << "if (findIter != m_registriesMap.constEnd()){";
		ChangeIndent(1);

		NextLine(stream);
		stream << "return findIter.value().GetPtr();";

		ChangeIndent(-1);
		NextLine(stream);
		stream << "}";

		NextLine(stream);
		stream << "return BaseClass::GetRegistry(address, contextRegistryPtr);";

		ChangeIndent(-1);
		NextLine(stream);
		stream << "}";
	}

	stream << "\n\n\n";

	return stream.status() == QTextStream::Ok;
}


bool CRegistryCodeSaverComp::WriteDependencies(
			const Addresses& composedAddresses,
			const Addresses& realAddresses,
			bool allDependencies,
			QTextStream& stream) const
{
	if (!m_packagesManagerCompPtr.IsValid()){
		return false;
	}

	if (!composedAddresses.isEmpty() && !realAddresses.isEmpty()){
		QList<icomp::CComponentAddress> sortedComponentAdresses = composedAddresses.toList();
		qSort(sortedComponentAdresses);

		for (		QList<icomp::CComponentAddress>::const_iterator addressIter = sortedComponentAdresses.constBegin();
					addressIter != sortedComponentAdresses.constEnd();
					++addressIter){
			const icomp::CComponentAddress& address = *addressIter;
			const QByteArray& packageId = address.GetPackageId();

			QDir packageDir(QDir::cleanPath(m_packagesManagerCompPtr->GetPackagePath(packageId)));

			stream << packageDir.absoluteFilePath(address.GetComponentId() + ".acc") << "\n";
		}

		if (allDependencies){
			Ids packageIdsList;
			for (		Addresses::const_iterator addressIter = realAddresses.constBegin();
						addressIter != realAddresses.constEnd();
						++addressIter){
				const icomp::CComponentAddress& address = *addressIter;
				packageIdsList.insert(address.GetPackageId());
			}

			QList<QByteArray> sortedPackageIdsList = packageIdsList.toList();
			qSort(sortedPackageIdsList);

			for (		QList<QByteArray>::const_iterator packageIter = sortedPackageIdsList.begin();
						packageIter != sortedPackageIdsList.end();
						++packageIter){
				const QByteArray& packageId = *packageIter;

				QFileInfo packageFilePath(QDir::cleanPath(m_packagesManagerCompPtr->GetPackagePath(packageId)));

				stream << packageFilePath.absoluteFilePath() << "\n";
			}
		}
	}

	if (m_extPackagesManagerCompPtr.IsValid()){
		icomp::IExtPackagesManager::PathList configFilesList = m_extPackagesManagerCompPtr->GetConfigurationPathList(icomp::IExtPackagesManager::PT_CONFIG);

		QStringList sortedConfigFilesList = configFilesList.toList();
		qSort(sortedConfigFilesList);

		for (		QStringList::const_iterator pathIter = sortedConfigFilesList.constBegin();
					pathIter != sortedConfigFilesList.constEnd();
					++pathIter){
			QFileInfo configFilePath(*pathIter);

			stream << configFilePath.absoluteFilePath().toLocal8Bit().constData() << "\n";
		}
	}

	return true;
}


bool CRegistryCodeSaverComp::WriteRegistryInfo(
			const icomp::IRegistry& registry,
			const QByteArray& registryCallPrefix,
			QTextStream& stream) const
{
	icomp::IRegistry::Ids ids = registry.GetElementIds();
	for (		icomp::IRegistry::Ids::const_iterator elementIter = ids.begin();
				elementIter != ids.end();
				++elementIter){
		const QByteArray& componentId = *elementIter;

		const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(componentId);
		Q_ASSERT(infoPtr != NULL);	// used element ID was returned by registry, info must exist.

		NextLine(stream);
		stream << "// element '" << componentId << "' of type " << infoPtr->address.GetPackageId() << "::" << infoPtr->address.GetComponentId();

		if (!WriteComponentInfo(registry, registryCallPrefix, componentId, *infoPtr, stream)){
			return false;
		}
	}

	const icomp::IRegistry::ExportedInterfacesMap& interfacesMap = registry.GetExportedInterfacesMap();
	if (!interfacesMap.isEmpty()){
		NextLine(stream);
		stream << "// interface export";
		for (		icomp::IRegistry::ExportedInterfacesMap::const_iterator interfaceIter = interfacesMap.begin();
					interfaceIter != interfacesMap.end();
					++interfaceIter){
			const QByteArray& interfaceName = interfaceIter.key();
			const QByteArray& componentName = interfaceIter.value();

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

	const icomp::IRegistry::ExportedElementsMap& componentsMap = registry.GetExportedElementsMap();
	if (!componentsMap.isEmpty()){
		stream << "\n";
		NextLine(stream);
		stream << "// component export";
		for (		icomp::IRegistry::ExportedElementsMap::const_iterator componentIter = componentsMap.begin();
					componentIter != componentsMap.end();
					++componentIter){
			const QByteArray& exportedName = componentIter.key();
			const QByteArray& componentName = componentIter.value();

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
		const QByteArray& id = *embeddedIter;

		const icomp::IRegistry* embeddedRegistryPtr = registry.GetEmbeddedRegistry(id);
		if (embeddedRegistryPtr != NULL){
			stream << "\n";

			QByteArray embeddedRegName = "embedded" + GetValidIdentifier(id) + "Ptr";

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

	return stream.status() == QTextStream::Ok;
}


bool CRegistryCodeSaverComp::WriteComponentInfo(
			const icomp::IRegistry& /*registry*/,
			const QByteArray& registryCallPrefix,
			const QByteArray& componentId,
			const icomp::IRegistry::ElementInfo& componentInfo,
			QTextStream& stream) const
{
	QByteArray elementInfoName = "info" + GetValidIdentifier(componentId) + "Ptr";

	if (componentInfo.elementPtr.IsValid()){
		icomp::IRegistryElement& component = *componentInfo.elementPtr;
		iattr::IAttributesProvider::AttributeIds attributeIds = component.GetAttributeIds();

		quint32 componentFlags = component.GetElementFlags();

		NextLine(stream);
		if (!attributeIds.isEmpty() || (componentFlags != 0)){
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

		if (!attributeIds.isEmpty() || (componentFlags != 0)){
			NextLine(stream);
			stream << "if ((" << elementInfoName << " != NULL) && " << elementInfoName << "->elementPtr.IsValid()){";
			ChangeIndent(1);

			if (componentFlags != 0){
				NextLine(stream);
				stream << elementInfoName << "->elementPtr->SetElementFlags(" << componentFlags << ");";
			}

			if (!attributeIds.isEmpty()){
				if (componentFlags != 0){
					stream << "\n";
				}

				for (		iattr::IAttributesProvider::AttributeIds::ConstIterator attributeIter = attributeIds.constBegin();
							attributeIter != attributeIds.constEnd();
							++attributeIter){
					const QByteArray& attributeId = *attributeIter;

					const icomp::IRegistryElement::AttributeInfo* attrInfoPtr = component.GetAttributeInfo(attributeId);

					if (attributeIter != attributeIds.begin()){
						stream << "\n";
					}

					NextLine(stream);
					stream << "// create and set attribute value for '" << attributeId << "'";
					if (attrInfoPtr != NULL){
						QByteArray attributeInfoName = "attrInfo" + attributeId + "Ptr";

						bool isAttributeValid = attrInfoPtr->attributePtr.IsValid();

						NextLine(stream);
						stream << "icomp::IRegistryElement::AttributeInfo* " << attributeInfoName << " = ";
						stream << elementInfoName << "->elementPtr->InsertAttributeInfo(\"" << attributeId << "\", \"" << attrInfoPtr->attributeTypeName << "\");";

						NextLine(stream);
						stream << "if (" << attributeInfoName << " != NULL){";
						ChangeIndent(1);

						if (!attrInfoPtr->exportId.isEmpty()){
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
			stream << "}" << "\n";
		}
	}

	return stream.status() == QTextStream::Ok;
}


bool CRegistryCodeSaverComp::WriteAttribute(
			const QByteArray& attributeId,
			const QByteArray& /*componentId*/,
			const QByteArray& attributeInfoName,
			const iser::IObject& attribute,
			QTextStream& stream) const
{
	QByteArray attributeType;
	QByteArray valueString;
	QList<QByteArray> valueStrings;

	QByteArray attributeName = "attr" + attributeId + "Ptr";

	if (GetAttributeValue(attribute, valueString, attributeType)){
		NextLine(stream);
		stream << attributeInfoName << "->attributePtr.SetPtr(new " << attributeType << ");";

		NextLine(stream);
		stream << "Q_ASSERT(" << attributeInfoName << "->attributePtr.IsValid());";
		stream << "\n";

		NextLine(stream);
		stream << attributeType << "* " << attributeName << " = dynamic_cast<" << attributeType << "*>(" << attributeInfoName << "->attributePtr.GetPtr());";
		NextLine(stream);
		stream << "Q_ASSERT(" << attributeName << " != NULL);";
		NextLine(stream);
		stream << attributeName << "->SetValue(" << valueString << ");";
	}
	else if (GetMultiAttributeValue(attribute, valueStrings, attributeType)){
		NextLine(stream);
		stream << attributeInfoName << "->attributePtr.SetPtr(new " << attributeType << ");";

		NextLine(stream);
		stream << "Q_ASSERT(" << attributeInfoName << "->attributePtr.IsValid());";
		stream << "\n";

		if (!valueStrings.isEmpty()){
			NextLine(stream);
			stream << attributeType << "* n" << attributeInfoName << " = dynamic_cast<" << attributeType << "*>(" << attributeInfoName << "->attributePtr.GetPtr());";
			NextLine(stream);
			stream << "Q_ASSERT(n" << attributeInfoName << " != NULL);";
			stream << "\n";
			NextLine(stream);
			stream << "n" << attributeInfoName << "->Reset();";

			for (		QList<QByteArray>::const_iterator iter = valueStrings.begin();
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
					QObject::tr("Unknown attribute type: %1").arg(QString(attributeType)));
		return false;
	}

	return true;
}


bool CRegistryCodeSaverComp::WriteRegistryTranslation(
			const icomp::IRegistry& registry,
			bool& translationFound,
			QTextStream& stream) const
{
	icomp::IRegistry::Ids ids = registry.GetElementIds();
	for (		icomp::IRegistry::Ids::const_iterator elementIter = ids.begin();
				elementIter != ids.end();
				++elementIter){
		const QByteArray& componentId = *elementIter;

		const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(componentId);
		Q_ASSERT(infoPtr != NULL);	// used element ID was returned by registry, info must exist.

		if (!WriteComponentTranslation(componentId, *infoPtr, translationFound, stream)){
			return false;
		}
	}

	icomp::IRegistry::Ids embeddedIds = registry.GetEmbeddedRegistryIds();
	for (		icomp::IRegistry::Ids::const_iterator embeddedIter = embeddedIds.begin();
				embeddedIter != embeddedIds.end();
				++embeddedIter){
		const QByteArray& id = *embeddedIter;

		const icomp::IRegistry* embeddedRegistryPtr = registry.GetEmbeddedRegistry(id);
		if (embeddedRegistryPtr != NULL){
			if (!WriteRegistryTranslation(*embeddedRegistryPtr, translationFound, stream)){
				return false;
			}
		}
	}

	return stream.status() == QTextStream::Ok;
}


bool CRegistryCodeSaverComp::WriteComponentTranslation(
			const QByteArray& componentId,
			const icomp::IRegistry::ElementInfo& componentInfo,
			bool& translationFound,
			QTextStream& stream) const
{
	if (componentInfo.elementPtr.IsValid()){
		icomp::IRegistryElement& component = *componentInfo.elementPtr;
		iattr::IAttributesProvider::AttributeIds attributeIds = component.GetAttributeIds();

		quint32 componentFlags = component.GetElementFlags();

		if (!attributeIds.isEmpty() || (componentFlags != 0)){
			if (!attributeIds.isEmpty()){
				for (		iattr::IAttributesProvider::AttributeIds::ConstIterator attributeIter = attributeIds.constBegin();
							attributeIter != attributeIds.constEnd();
							++attributeIter){
					const QByteArray& attributeId = *attributeIter;

					const icomp::IRegistryElement::AttributeInfo* attrInfoPtr = component.GetAttributeInfo(attributeId);

					if ((attrInfoPtr != NULL) && attrInfoPtr->attributePtr.IsValid()){
						const iattr::CStringAttribute* stringAttribute = dynamic_cast<const iattr::CStringAttribute*>(attrInfoPtr->attributePtr.GetPtr());
						if (stringAttribute != NULL){
							if (!translationFound){
								NextLine(stream);
								stream << "// translations";
							}

							NextLine(stream);
							stream << "#define TRANSLATED_TEXT QT_TRANSLATE_NOOP(\"Attribute\", " << GetStringLiteral(stringAttribute->GetValue()) << "); // Attribute " << attributeId << " in " << componentId;
							NextLine(stream);
							stream << "#undef TRANSLATED_TEXT";

							translationFound = true;
							continue;
						}

						const iattr::CStringListAttribute* stringAttributePtr = dynamic_cast<const iattr::CStringListAttribute*>(attrInfoPtr->attributePtr.GetPtr());
						if (stringAttributePtr != NULL){
							for (int index = 0; index < stringAttributePtr->GetValuesCount(); index++){
								if (!translationFound){
									NextLine(stream);
									stream << "// translations";
								}

								NextLine(stream);
								stream << "#define TRANSLATED_TEXT QT_TRANSLATE_NOOP(\"Attribute\", " << GetStringLiteral(stringAttributePtr->GetValueAt(index)) << "); // Attribute " << attributeId << " in " << componentId;
								NextLine(stream);
								stream << "#undef TRANSLATED_TEXT";

								translationFound = true;
							}
							continue;
						}
					}
				}
			}
		}
	}

	return stream.status() == QTextStream::Ok;
}


bool CRegistryCodeSaverComp::WriteRegistryClassDeclaration(
			const QByteArray& /*baseClassName*/,
			const QByteArray& registryClassName,
			const icomp::IRegistry& registry,
			QTextStream& stream) const
{
	QString description = registry.GetDescription();

	if (!description.isEmpty()){
		NextLine(stream);
		stream << "/**";

		NextLine(stream);
		stream << "\t" << description.toLocal8Bit() << ".";

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
			const QByteArray& baseClassName,
			const QByteArray& registryClassName,
			const icomp::IRegistry& registry,
			QTextStream& stream) const
{
	NextLine(stream);
	stream << "// Embedded class " << registryClassName;

	stream << "\n";

	NextLine(stream);
	stream << baseClassName << "::" << registryClassName << "::" << registryClassName << "()";

	NextLine(stream);
	stream << "{";
	ChangeIndent(1);

	if (*m_useBinaryCodeAttrPtr){
		bool isTranslationFound = false;
		WriteRegistryTranslation(registry, isTranslationFound, stream);
		if (isTranslationFound){
			stream << "\n";
		}

		WriteDeserializingCode(registry, stream);
	}
	else{
		if (!WriteRegistryInfo(registry, "", stream)){
			return false;
		}
	}

	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";

	return true;
}


bool CRegistryCodeSaverComp::WriteDeserializingCode(const iser::ISerializable& object, QTextStream& stream) const
{
	iser::CMemoryWriteArchive memArchive(NULL, false);

	if (!const_cast<iser::ISerializable&>(object).Serialize(memArchive)){
		return false;
	}

	NextLine(stream);
	stream << "static const unsigned char data[] = {";
	ChangeIndent(3);

	int bufferSize = memArchive.GetBufferSize();
	const quint8* bufferPtr = (const quint8*)memArchive.GetBuffer();
	for (int i = 0; i < bufferSize; ++i){
		if ((i % 16) == 0){
			NextLine(stream);
		}

		int byteValue = bufferPtr[i];
		stream << "0x" << QString::number(byteValue, 16);

		if (i < bufferSize - 1){
			stream << ", ";
		}
	}

	ChangeIndent(-3);

	stream << "};";

	stream << "\n";

	NextLine(stream);
	stream << "iser::CMemoryReadArchive archive(data, sizeof(data), false);";

	stream << "\n";

	NextLine(stream);
	stream << "Serialize(archive);";

	return true;
}


bool CRegistryCodeSaverComp::GetAttributeValue(
			const iser::ISerializable& attribute,
			QByteArray& valueString,
			QByteArray& typeName) const
{
	const iattr::CBooleanAttribute* boolAttribute = dynamic_cast<const iattr::CBooleanAttribute*>(&attribute);
	if (boolAttribute != NULL){
		valueString = boolAttribute->GetValue()? "true": "false";
		typeName = "iattr::CBooleanAttribute";

		return true;
	}

	const iattr::CRealAttribute* doubleAttribute = dynamic_cast<const iattr::CRealAttribute*>(&attribute);
	if (doubleAttribute != NULL){
		valueString = QString("%1").arg(doubleAttribute->GetValue()).toLocal8Bit();
		typeName = "iattr::CRealAttribute";

		return true;
	}

	const iattr::CIntegerAttribute* intAttribute = dynamic_cast<const iattr::CIntegerAttribute*>(&attribute);
	if (intAttribute != NULL){
		valueString = QString("%1").arg(intAttribute->GetValue()).toLocal8Bit();
		typeName = "iattr::CIntegerAttribute";

		return true;
	}

	const iattr::CStringAttribute* stringAttribute = dynamic_cast<const iattr::CStringAttribute*>(&attribute);
	if (stringAttribute != NULL){
		valueString = "QT_TRANSLATE_NOOP(\"Attribute\", " + GetStringLiteral(stringAttribute->GetValue()) + ")";
		typeName = "iattr::CStringAttribute";

		return true;
	}

	const iattr::CIdAttribute* idAttributePtr = dynamic_cast<const iattr::CIdAttribute*>(&attribute);
	if (idAttributePtr != NULL){
		valueString = GetIdValueLiteral(idAttributePtr->GetValue());

		const icomp::CFactoryAttribute* factoryAttributePtr = dynamic_cast<const icomp::CFactoryAttribute*>(&attribute);
		if (factoryAttributePtr != NULL){
			typeName = "icomp::CFactoryAttribute";

			return true;
		}

		const icomp::CReferenceAttribute* referenceAttributePtr = dynamic_cast<const icomp::CReferenceAttribute*>(&attribute);
		if (referenceAttributePtr != NULL){
			typeName = "icomp::CReferenceAttribute";

			return true;
		}

		typeName = "iattr::CIdAttribute";

		return true;
	}

	return false;
}


bool CRegistryCodeSaverComp::GetMultiAttributeValue(
			const iser::ISerializable& attribute,
			QList<QByteArray>& valueStrings,
			QByteArray& typeName) const
{
	valueStrings.clear();

	const iattr::CBooleanListAttribute* boolAttribute = dynamic_cast<const iattr::CBooleanListAttribute*>(&attribute);
	if (boolAttribute != NULL){
		for (int index = 0; index < boolAttribute->GetValuesCount(); index++){
			valueStrings.push_back(boolAttribute->GetValueAt(index)? "true": "false");
		}

		typeName = "iattr::CBooleanListAttribute";

		return true;
	}

	const iattr::CIntegerListAttribute* intAttribute = dynamic_cast<const iattr::CIntegerListAttribute*>(&attribute);
	if (intAttribute != NULL){
		for (int index = 0; index < intAttribute->GetValuesCount(); index++){
			valueStrings.push_back(QString("%1").arg(intAttribute->GetValueAt(index)).toLocal8Bit());
		}

		typeName = "iattr::CIntegerListAttribute";

		return true;
	}

	const iattr::CRealListAttribute* doubleAttribute = dynamic_cast<const iattr::CRealListAttribute*>(&attribute);
	if (doubleAttribute != NULL){
		for (int index = 0; index < doubleAttribute->GetValuesCount(); index++){
			valueStrings.push_back(QString("%1").arg(doubleAttribute->GetValueAt(index)).toLocal8Bit());
		}

		typeName = "iattr::CRealListAttribute";

		return true;
	}

	const iattr::CStringListAttribute* stringAttributePtr = dynamic_cast<const iattr::CStringListAttribute*>(&attribute);
	if (stringAttributePtr != NULL){
		for (int index = 0; index < stringAttributePtr->GetValuesCount(); index++){
			valueStrings.push_back("QT_TRANSLATE_NOOP(\"Attribute\", " + GetStringLiteral(stringAttributePtr->GetValueAt(index)) + ")");
		}

		typeName = "iattr::CStringListAttribute";

		return true;
	}

	const iattr::CIdListAttribute* idAttributePtr = dynamic_cast<const iattr::CIdListAttribute*>(&attribute);
	if (idAttributePtr != NULL){
		for (int index = 0; index < idAttributePtr->GetValuesCount(); index++){
			valueStrings.push_back(GetIdValueLiteral(idAttributePtr->GetValueAt(index)));
		}

		const icomp::CMultiFactoryAttribute* factoryAttributePtr = dynamic_cast<const icomp::CMultiFactoryAttribute*>(&attribute);
		if (factoryAttributePtr != NULL){
			typeName = "icomp::CMultiFactoryAttribute";

			return true;
		}

		const icomp::CMultiReferenceAttribute* referenceAttributePtr = dynamic_cast<const icomp::CMultiReferenceAttribute*>(&attribute);
		if (referenceAttributePtr != NULL){
			typeName = "icomp::CMultiReferenceAttribute";

			return true;
		}

		typeName = "iattr::CIdListAttribute";

		return true;
	}

	return false;
}


bool CRegistryCodeSaverComp::NextLine(QTextStream& stream) const
{
	stream << "\n";

	for (int i = 0; i < m_indentCount; ++i){
		stream << "\t";
	}

	return stream.status() == QTextStream::Ok;
}


int CRegistryCodeSaverComp::ChangeIndent(int difference) const
{
	m_indentCount += difference;

	Q_ASSERT(m_indentCount >= 0);

	return m_indentCount;
}


bool CRegistryCodeSaverComp::ExtractInfoFromFile(const QString& filePath, QByteArray& className, QString& headerFilePath) const
{
	QFileInfo fileInfo(filePath);

	className = fileInfo.baseName().toLocal8Bit();

	headerFilePath = fileInfo.absolutePath() + "/" + className;
	headerFilePath = QDir::toNativeSeparators(headerFilePath);

	return (!className.isEmpty());
}


QByteArray CRegistryCodeSaverComp::GetPackageName(const QByteArray& packageId) const
{
	if (packageId.endsWith("Pck")){
		return packageId;
	}
	else{
		return packageId + "Pck";
	}
}


QByteArray CRegistryCodeSaverComp::GetStringLiteral(const QString& text) const
{
	static const char* hexCiphers = "0123456789ABCDEF";

	QByteArray retVal("\"");

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


QByteArray CRegistryCodeSaverComp::GetIdValueLiteral(const QByteArray& text) const
{
	static const char* hexCiphers = "0123456789ABCDEF";

	QByteArray retVal("\"");

	for (		QByteArray::const_iterator iter = text.begin();
				iter != text.end();
				++iter){
		QByteArray::value_type c = *iter;

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
		else if ((c >= 127) || (c < 32)){
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


QByteArray CRegistryCodeSaverComp::GetValidIdentifier(const QByteArray& identifier) const
{
	QByteArray result = identifier;

	for (int i = 0; i < identifier.size(); i++){
		char c = identifier[0];
		if (!isalpha(c)){
			if ((i == 0) || !isdigit(c)){	// if first character or not numeric -> we set some legal character
				result[i] = '_';
			}
		}
	}

	return result;
}


} // namespace ipackage


