#include "ibase/CRegistryCodeSaverComp.h"


// STL includes
#include <fstream>


namespace ibase
{


CRegistryCodeSaverComp::CRegistryCodeSaverComp()
:	m_indentCount(0)
{
}


// reimplemented (iser::IFileLoader)

bool CRegistryCodeSaverComp::IsOperationSupported(
		const istd::IChangeable* dataObjectPtr,
		const istd::CString* /*filePathPtr*/,
		int flags,
		bool /*beQuiet*/) const
{
	if (dynamic_cast<const icomp::IRegistry*>(dataObjectPtr) == NULL){
		return false;
	}

	return (flags & QF_NO_SAVING) == 0;
}


int CRegistryCodeSaverComp::LoadFromFile(istd::IChangeable& /*data*/, const istd::CString& /*filePath*/) const
{
	return StateFailed;
}


int CRegistryCodeSaverComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	const icomp::IRegistry* registryPtr = dynamic_cast<const icomp::IRegistry*>(&data);
	if (registryPtr != NULL){
		istd::CString headerFilePath;
		std::string className;
		if (ExtractInfoFromFile(filePath, className, headerFilePath)){
			std::ofstream stream(filePath.ToString().c_str());
			std::ofstream headerStream(headerFilePath.ToString().c_str());

			Addresses realAddresses;
			Addresses composedAddresses;

			AppendAddresses(*registryPtr, realAddresses, composedAddresses);

			if (		WriteHeader(className, *registryPtr, composedAddresses, realAddresses, headerStream) &&
						WriteIncludes(className, realAddresses, stream) &&
						WriteRegistryInfo(className, *registryPtr, composedAddresses, realAddresses, stream)){
				return StateOk;
			}
		}
	}

	return StateFailed;
}


bool CRegistryCodeSaverComp::GetFileExtensions(istd::CStringList& result, int flags, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	if ((flags & QF_NO_SAVING) == 0){
		result.push_back("c");
		result.push_back("cpp");
	}

	return true;
}


istd::CString CRegistryCodeSaverComp::GetTypeDescription(const istd::CString* extensionPtr) const
{
	if (		(extensionPtr == NULL) ||
				extensionPtr->IsEqualNoCase("c") ||
				extensionPtr->IsEqualNoCase("cpp")){
		return "C++ source file";
	}

	return "";
}


// protected methods

bool CRegistryCodeSaverComp::AppendAddresses(
			const icomp::IRegistry& registry,
			Addresses& realAddresses,
			Addresses& composedAddresses) const
{
	bool retVal = false;

	icomp::IRegistry::Ids ids = registry.GetElementIds();
	for (		icomp::IRegistry::Ids::const_iterator elementIter = ids.begin();
				elementIter != ids.end();
				++elementIter){
		const std::string& componentId = *elementIter;

		const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(componentId);
		I_ASSERT(infoPtr != NULL);	// used element ID was returned by registry, info must exist.

		if (		(realAddresses.find(infoPtr->address) == realAddresses.end()) &&
					(composedAddresses.find(infoPtr->address) == composedAddresses.end())){
			if (m_registriesManagerCompPtr.IsValid()){
				const icomp::IRegistry* subregistryPtr = m_registriesManagerCompPtr->GetRegistry(infoPtr->address);
				if (subregistryPtr != NULL){
					composedAddresses.insert(infoPtr->address);

					retVal = AppendAddresses(*subregistryPtr, realAddresses, composedAddresses) || retVal;
				}
				else{
					realAddresses.insert(infoPtr->address);

					retVal = true;
				}
			}
		}
	}

	return retVal;
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
	stream << "#include \"icomp/IRegistriesManager.h\"";
	NextLine(stream);
	stream << "#include \"icomp/TComponentWrap.h\"";
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

	stream << std::endl << std::endl;

	istd::CString description = registry.GetDescription();
	if (!description.IsEmpty()){
		NextLine(stream);
		stream << "/**";

		NextLine(stream);
		stream << "\t" << description.ToString() << ".";

		NextLine(stream);
		stream << "*/";
	}

	NextLine(stream);
	stream << "class " << className << ": protected icomp::TComponentWrap<icomp::CCompositeComponent>";
	NextLine(stream);
	stream << "{";
	NextLine(stream);
	stream << "public:";
	ChangeIndent(1);

	NextLine(stream);
	stream << "typedef icomp::TComponentWrap<icomp::CCompositeComponent> BaseClass;";
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

	WriteRegistryClassDeclaration(className, "CMainRegistry", registry, stream);

	Ids composedPackageIds = ExtractPackageIds(composedAddresses);
	if (m_registriesManagerCompPtr.IsValid() && !composedPackageIds.empty()){
		stream << std::endl;

		NextLine(stream);
		stream << "//\tcomposited packages";

		for (		Ids::const_iterator packageIter = composedPackageIds.begin();
					packageIter != composedPackageIds.end();
					++packageIter){
			const std::string& packageId = *packageIter;

			std::string packageName = GetPackageName(packageId);

			stream << std::endl;

			NextLine(stream);
			stream << "class C" << packageName;

			NextLine(stream);
			stream << "{";

			NextLine(stream);
			stream << "public:";
			ChangeIndent(1);

			NextLine(stream);
			stream << "static const icomp::IComponentStaticInfo& EnsureStaticInfoInit(const icomp::IRegistriesManager* managerPtr);";

			Ids componentIds = ExtractComponentIds(composedAddresses, packageId);
			for (		Ids::const_iterator componentIter = componentIds.begin();
						componentIter != componentIds.end();
						++componentIter){
				const std::string& componentId = *componentIter;

				std::string componentName = GetComponentName(componentId);

				icomp::CComponentAddress address(packageId, componentId);

				const icomp::IRegistry* subregistryPtr = m_registriesManagerCompPtr->GetRegistry(address);

				if (subregistryPtr != NULL){
					stream << std::endl;

					WriteRegistryClassDeclaration(className, "C" + componentName, *subregistryPtr, stream);
				}
				else{
					NextLine(stream);
					stream << "// Registry for address '" << address.GetPackageId() << "/" << address.GetComponentId() << "' could not be created";
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
	stream << "class CFactory: public icomp::CPackageStaticInfo, virtual public icomp::IRegistriesManager";
	NextLine(stream);
	stream << "{";
	NextLine(stream);
	stream << "public:";
	ChangeIndent(1);

	NextLine(stream);
	stream << "CFactory();";
	stream << std::endl;

	NextLine(stream);
	stream << "// reimplemented (icomp::IRegistriesManager)";
	NextLine(stream);
	stream << "virtual const icomp::IRegistry* GetRegistry(const icomp::CComponentAddress& address) const;";
	NextLine(stream);
	stream << "virtual istd::CString GetPackageDirPath(const std::string& packageId) const;";
	stream << std::endl;

	ChangeIndent(-1);
	NextLine(stream);
	stream << "private:";
	ChangeIndent(1);

	for (		Ids::const_iterator packageInstanceIter = realPackageIds.begin();
				packageInstanceIter != realPackageIds.end();
				++packageInstanceIter){
		const std::string& packageId = *packageInstanceIter;

		std::string packageName = GetPackageName(packageId);

		NextLine(stream);
		stream << "C" << packageName << " m_sub" << packageName << ";";
	}

	ChangeIndent(-1);
	NextLine(stream);
	stream << "};";
	stream << std::endl;

	ChangeIndent(-1);
	NextLine(stream);
	stream << "private:";
	ChangeIndent(1);

	NextLine(stream);
	stream << "static CFactory s_factory;";

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
	stream << "#include \"icomp/TSingleAttribute.h\"" << std::endl;
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

			stream << "#include \"" << packageId << "/" << packageId << ".h\"" << std::endl;
		}
	}

	stream << std::endl << std::endl;

	return !stream.fail();
}


bool CRegistryCodeSaverComp::WriteRegistryInfo(
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
	stream << "static icomp::CRegistryElement element;";
	NextLine(stream);
	stream << "static icomp::CCompositeComponentContext context(&element, &CMainRegistry::EnsureRegistryInit(&s_factory), &s_factory, NULL);";
	NextLine(stream);
	stream << "SetComponentContext(&context, NULL, false);";

	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";

	stream << std::endl << std::endl;

	NextLine(stream);
	stream << "// static attributes";
	NextLine(stream);
	stream << className << "::CFactory " << className << "::s_factory;";

	stream << std::endl << std::endl;

	WriteRegistryClassBody(className, "CMainRegistry", registry, stream);
	stream << std::endl << std::endl;

	Ids composedPackageIds = ExtractPackageIds(composedAddresses);

	if (m_registriesManagerCompPtr.IsValid()){
		for (		Ids::const_iterator packageIter = composedPackageIds.begin();
					packageIter != composedPackageIds.end();
					++packageIter){
			const std::string& packageId = *packageIter;

			std::string packageName = GetPackageName(packageId);

			NextLine(stream);
			stream << "// Embedded class C" << packageName;

			stream << std::endl;

			NextLine(stream);
			stream << "const icomp::IComponentStaticInfo& " << className << "::C" << packageName << "::EnsureStaticInfoInit(const icomp::IRegistriesManager* managerPtr)";

			NextLine(stream);
			stream << "{";
			ChangeIndent(1);

			NextLine(stream);
			stream << "static istd::TDelPtr<icomp::CCompositePackageStaticInfo> infoPtr;";
			stream << std::endl;

			NextLine(stream);
			stream << "if (!infoPtr.IsValid()){";
			ChangeIndent(1);

			NextLine(stream);
			stream << "icomp::IComponentStaticInfo::Ids ids;";
			stream << std::endl;

			Ids componentIds = ExtractComponentIds(composedAddresses, packageId);
			if (!componentIds.empty()){
				for (		Ids::const_iterator instanceRegIter = componentIds.begin();
							instanceRegIter != componentIds.end();
							++instanceRegIter){
					const std::string& componentId = *instanceRegIter;

					std::string componentName = GetComponentName(componentId);

					NextLine(stream);
					stream << "ids.insert(\"" << componentId << "\");";
				}

				stream << std::endl;
			}

			NextLine(stream);
			stream << "infoPtr.SetPtr(new icomp::CCompositePackageStaticInfo(\"" << packageId << "\", ids, managerPtr));";
			stream << std::endl;

			ChangeIndent(-1);
			NextLine(stream);
			stream << "}";
			stream << std::endl;

			NextLine(stream);
			stream << "return *infoPtr;";

			ChangeIndent(-1);
			NextLine(stream);
			stream << "}";

			stream << std::endl << std::endl;

			for (		Ids::const_iterator componentIter = componentIds.begin();
						componentIter != componentIds.end();
						++componentIter){
				const std::string& componentId = *componentIter;

				std::string componentName = GetComponentName(componentId);

				icomp::CComponentAddress address(packageId, componentId);

				const icomp::IRegistry* subregistryPtr = m_registriesManagerCompPtr->GetRegistry(address);

				if (subregistryPtr != NULL){
					WriteRegistryClassBody(className + "::C" + packageName, "C" + componentName, *subregistryPtr, stream);
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
			stream << "RegisterSubcomponentInfo(\"" << componentId << "\", &" << infoName << ");";
		}

		ChangeIndent(-1);
		NextLine(stream);
		stream << "}";

		stream << std::endl << std::endl;
	}

	NextLine(stream);
	stream << "// Embedded class CFactory";

	stream << std::endl;

	NextLine(stream);
	stream << className << "::CFactory::CFactory()";

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

			std::string packageName = GetPackageName(packageId);

			NextLine(stream);
			stream << "RegisterSubcomponentInfo(\"" << packageId << "\", &m_sub" << packageName << ");";
		}
	}

	if (!composedPackageIds.empty()){
		NextLine(stream);
		stream << "// register composed components";

		for (		Ids::const_iterator registerPackageIter = composedPackageIds.begin();
					registerPackageIter != composedPackageIds.end();
					++registerPackageIter){
			const std::string& packageId = *registerPackageIter;

			std::string packageName = GetPackageName(packageId);

			NextLine(stream);
			stream << "RegisterSubcomponentInfo(\"" << packageId << "\", &C" << packageName << "::EnsureStaticInfoInit(this));";
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
	stream << "const icomp::IRegistry* " << className << "::CFactory::GetRegistry(const icomp::CComponentAddress& address) const";
	NextLine(stream);
	stream << "{";
	ChangeIndent(1);

	if (!composedAddresses.empty()){
		for (		Addresses::const_iterator regTestIter = composedAddresses.begin();
					regTestIter != composedAddresses.end();
					++regTestIter){
			const icomp::CComponentAddress& address = *regTestIter;

			if (regTestIter != composedAddresses.begin()){
				stream << std::endl;
			}

			NextLine(stream);
			stream << "if (address == icomp::CComponentAddress(\"" << address.GetPackageId() << "\", \"" << address.GetComponentId() << "\")){";
			ChangeIndent(1);

			NextLine(stream);
			stream << "return &C" << GetPackageName(address.GetPackageId()) << "::C" << GetComponentName(address.GetComponentId()) << "::EnsureRegistryInit(&s_factory);";

			ChangeIndent(-1);
			NextLine(stream);
			stream << "}";
		}

		stream << std::endl;
	}

	NextLine(stream);
	stream << "return NULL;";

	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";

	stream << std::endl << std::endl;

	NextLine(stream);
	stream << "istd::CString " << className << "::CFactory::GetPackageDirPath(const std::string& /*packageId*/) const";
	NextLine(stream);
	stream << "{";
	ChangeIndent(1);
	NextLine(stream);
	stream << "return istd::CString();";
	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";

	stream << std::endl << std::endl;

	return !stream.fail();
}


bool CRegistryCodeSaverComp::WriteComponentInfo(
			const icomp::IRegistry& registry,
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

		stream << "registry.InsertElementInfo(";
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
						std::string attributeInfoName = "info" + attributeId + "Ptr";

						bool isAttributeValid = attrInfoPtr->attributePtr.IsValid();

						NextLine(stream);
						stream << "icomp::IRegistryElement::AttributeInfo* " << attributeInfoName << " = ";
						stream << elementInfoName << "->elementPtr->InsertAttributeInfo(\"" << attributeId << "\", " << isAttributeValid << ");";

						NextLine(stream);
						stream << "if (" << attributeInfoName << " != NULL){";
						ChangeIndent(1);

						if (!attrInfoPtr->exportId.empty()){
							NextLine(stream);
							stream << attributeInfoName << "->exportId = \"" << attrInfoPtr->exportId << "\";";
						}

						if (isAttributeValid){
							WriteAttribute(attributeId, attributeInfoName, *attrInfoPtr->attributePtr, stream);
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
			const std::string& attributeInfoName,
			const iser::ISerializable& attribute,
			std::ofstream& stream) const
{
	NextLine(stream);
	stream << "I_ASSERT(" << attributeInfoName << "->attributePtr.IsValid());";
	stream << std::endl;

	std::string attributeType;
	std::string valueString;
	std::list<std::string> valueStrings;

	std::string attributeName = "attr" + attributeId + "Ptr";

	if (GetSingleAttributeValue(attribute, valueString, attributeType)){
		NextLine(stream);
		stream << attributeType << "* " << attributeName << " = dynamic_cast<" << attributeType << "*>(" << attributeInfoName << "->attributePtr.GetPtr());";
		NextLine(stream);
		stream << "I_ASSERT(" << attributeName << " != NULL);";
		NextLine(stream);
		stream << attributeName << "->SetValue(" << valueString << ");";

		return true;
	}
	else if (GetMultipleAttributeValue(attribute, valueStrings, attributeType)){
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

		return true;
	}

	return false;
}


bool CRegistryCodeSaverComp::WriteRegistryClassDeclaration(
			const std::string& /*baseClassName*/,
			const std::string& registryClassName,
			const icomp::IRegistry& registry,
			std::ofstream& stream) const
{
	NextLine(stream);
	stream << "class " << registryClassName << ": private icomp::TComponentWrap<icomp::CCompositeComponent>";
	NextLine(stream);
	stream << "{";

	NextLine(stream);
	stream << "public:";
	ChangeIndent(1);

	NextLine(stream);
	stream << "typedef icomp::TComponentWrap<icomp::CCompositeComponent> BaseClass;";

	NextLine(stream);
	stream << "static const icomp::IComponentStaticInfo& EnsureStaticInfoInit(const icomp::IComponentStaticInfo* factoryPtr);";
	NextLine(stream);
	stream << "static const icomp::IRegistry& EnsureRegistryInit(const icomp::IComponentStaticInfo* factoryPtr);";

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
	stream << "const icomp::IComponentStaticInfo& " << baseClassName << "::" << registryClassName << "::EnsureStaticInfoInit(const icomp::IComponentStaticInfo* factoryPtr)";

	NextLine(stream);
	stream << "{";
	ChangeIndent(1);

	NextLine(stream);
	stream << "static istd::TDelPtr<icomp::CCompositeComponentStaticInfo> infoPtr;";
	stream << std::endl;

	NextLine(stream);
	stream << "if (!infoPtr.IsValid()){;";
	ChangeIndent(1);

	NextLine(stream);
	stream << "infoPtr.SetPtr(new icomp::CCompositeComponentStaticInfo(EnsureRegistryInit(factoryPtr)));";

	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";
	stream << std::endl;

	NextLine(stream);
	stream << "return *infoPtr;";

	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";

	stream << std::endl << std::endl;

	NextLine(stream);
	stream << "const icomp::IRegistry& " << baseClassName << "::" << registryClassName << "::EnsureRegistryInit(const icomp::IComponentStaticInfo* factoryPtr)";

	NextLine(stream);
	stream << "{";
	ChangeIndent(1);

	NextLine(stream);
	stream << "static bool isInitialized = false;";
	NextLine(stream);
	stream << "static icomp::CRegistry registry;";
	stream << std::endl;

	NextLine(stream);
	stream << "if (!isInitialized){";
	ChangeIndent(1);

	NextLine(stream);
	stream << "isInitialized = true;";
	stream << std::endl;

	NextLine(stream);
	stream << "registry.SetComponentStaticInfo(factoryPtr);";

	icomp::IRegistry::Ids ids = registry.GetElementIds();
	if (!ids.empty()){
		stream << std::endl;
		for (		icomp::IRegistry::Ids::const_iterator elementIter = ids.begin();
					elementIter != ids.end();
					++elementIter){
			const std::string& componentId = *elementIter;

			const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(componentId);
			I_ASSERT(infoPtr != NULL);	// used element ID was returned by registry, info must exist.

			NextLine(stream);
			stream << "// element '" << componentId << "' of type " << infoPtr->address.GetPackageId() << "::" << infoPtr->address.GetComponentId();

			WriteComponentInfo(registry, componentId, *infoPtr, stream);
		}
	}

	const icomp::IRegistry::ExportedInterfacesMap& interfacesMap = registry.GetExportedInterfacesMap();
	if (!interfacesMap.empty()){
		NextLine(stream);
		stream << "// interface export";
		for (		icomp::IRegistry::ExportedInterfacesMap::const_iterator interfaceIter = interfacesMap.begin();
					interfaceIter != interfacesMap.end();
					++interfaceIter){
			const istd::CClassInfo& info = interfaceIter->first;
			const std::string& componentName = interfaceIter->second;

			NextLine(stream);
			stream << "registry.SetElementInterfaceExported(";
			NextLine(stream);
			stream << "\t\t\t\"" << componentName << "\",";
			NextLine(stream);
			stream << "\t\t\tistd::CClassInfo(\"" << info.GetName() << "\"),";
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
			stream << "registry.SetElementExported(";
			NextLine(stream);
			stream << "\t\t\t\"" << exportedName << "\",";
			NextLine(stream);
			stream << "\t\t\t\"" << componentName << "\");";
		}
	}

	istd::CString description = registry.GetDescription();
	istd::CString keywords = registry.GetKeywords();

	if (!description.empty() || !keywords.empty()){
		stream << std::endl;
		NextLine(stream);
		stream << "// setting of meta info";

		if (!description.empty()){
			NextLine(stream);
			stream << "registry.SetDescription(\"" << description.ToString() << "\");";
		}

		if (!keywords.empty()){
			NextLine(stream);
			stream << "registry.SetKeywords(\"" << keywords.ToString() << "\");";
		}
	}

	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";
	stream << std::endl;

	NextLine(stream);
	stream << "return registry;";
	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";

	return true;
}


bool CRegistryCodeSaverComp::GetSingleAttributeValue(
			const iser::ISerializable& attribute,
			std::string& valueString,
			std::string& typeName) const
{
	const icomp::CBoolAttribute* boolAttribute = dynamic_cast<const icomp::CBoolAttribute*>(&attribute);
	if (boolAttribute != NULL){
		valueString = boolAttribute->GetValue() ? "true": "false";
		typeName = "icomp::CBoolAttribute";

		return true;
	}

	const icomp::CDoubleAttribute* doubleAttribute = dynamic_cast<const icomp::CDoubleAttribute*>(&attribute);
	if (doubleAttribute != NULL){
		valueString = istd::CString::FromNumber(doubleAttribute->GetValue()).ToString();
		typeName = "icomp::CDoubleAttribute";

		return true;
	}

	const icomp::CIntAttribute* intAttribute = dynamic_cast<const icomp::CIntAttribute*>(&attribute);
	if (intAttribute != NULL){
		valueString = istd::CString::FromNumber(intAttribute->GetValue()).ToString();
		typeName = "icomp::CIntAttribute";

		return true;
	}

	const icomp::CStringAttribute* stringAttribute = dynamic_cast<const icomp::CStringAttribute*>(&attribute);
	if (stringAttribute != NULL){
		valueString = GetStringLiteral(stringAttribute->GetValue());
		typeName = "icomp::CStringAttribute";

		return true;
	}

	const icomp::TSingleAttribute<std::string>* idPtr = dynamic_cast<const icomp::TSingleAttribute<std::string>*>(&attribute);
	if (idPtr != NULL){		
		valueString = istd::CString("\"" + idPtr->GetValue() + "\"").ToString();
		typeName = "icomp::TSingleAttribute<std::string>";

		return true;
	}

	return false;
}


bool CRegistryCodeSaverComp::GetMultipleAttributeValue(
			const iser::ISerializable& attribute,
			std::list<std::string>& valueStrings,
			std::string& typeName) const
{
	valueStrings.clear();

	const icomp::CMultiStringAttribute* stringListAttribute = dynamic_cast<const icomp::CMultiStringAttribute*>(&attribute);
	if (stringListAttribute != NULL){
		for (int index = 0; index < stringListAttribute->GetValuesCount(); index++){
			valueStrings.push_back(GetStringLiteral(stringListAttribute->GetValueAt(index)));
		}

		typeName = "icomp::CMultiStringAttribute";

		return true;
	}

	const icomp::CMultiIntAttribute* intListAttribute = dynamic_cast<const icomp::CMultiIntAttribute*>(&attribute);
	if (intListAttribute != NULL){
		for (int index = 0; index < intListAttribute->GetValuesCount(); index++){
			valueStrings.push_back(istd::CString::FromNumber(intListAttribute->GetValueAt(index)).ToString());
		}

		typeName = "icomp::CMultiIntAttribute";

		return true;
	}

	const icomp::CMultiDoubleAttribute* doubleListAttribute = dynamic_cast<const icomp::CMultiDoubleAttribute*>(&attribute);
	if (doubleListAttribute != NULL){
		for (int index = 0; index < doubleListAttribute->GetValuesCount(); index++){
			valueStrings.push_back(istd::CString::FromNumber(doubleListAttribute->GetValueAt(index)).ToString());
		}

		typeName = "icomp::CMultiDoubleAttribute";

		return true;
	}

	const icomp::TMultiAttribute<std::string>* multiIdPtr = dynamic_cast<const icomp::CMultiReferenceAttribute*>(&attribute);
	if (multiIdPtr != NULL){
		for (int index = 0; index < multiIdPtr->GetValuesCount(); index++){
			valueStrings.push_back(istd::CString("\"" + multiIdPtr->GetValueAt(index) + "\"").ToString());
		}

		typeName = "icomp::TMultiAttribute<std::string>";

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


bool CRegistryCodeSaverComp::ExtractInfoFromFile(const istd::CString& filePath, std::string& className, istd::CString& headerFilePath) const
{
	istd::CString::size_type dotPosition = filePath.find_last_of('.');
	if ((dotPosition != istd::CString::npos) && (dotPosition > 0)){
		istd::CString::size_type beginPosition = filePath.find_last_of(istd::CString("/\\"), dotPosition - 1) + 1;

		if (beginPosition == istd::CString::npos){
			beginPosition = 0;
		}

		I_ASSERT(beginPosition < dotPosition);

		className = filePath.ToString().substr(beginPosition, dotPosition - beginPosition);
		headerFilePath = filePath.substr(0, dotPosition) + istd::CString(".h");

		return true;
	}

	return false;
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


std::string CRegistryCodeSaverComp::GetComponentName(const std::string& componentId) const
{
	return componentId + "Comp";
}


std::string CRegistryCodeSaverComp::GetStringLiteral(const istd::CString& text) const
{
	std::string retVal("L\"");

	for (		istd::CString::const_iterator iter = text.begin();
				iter != text.end();
				++iter){
		istd::CString::value_type c = *iter;

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
		else if ((c >= 128) || (c < 32)){
			retVal += "\\x";
			retVal += char('0' + ((c >> 12) & 15));
			retVal += char('0' + ((c >> 8) & 15));
			retVal += char('0' + ((c >> 4) & 15));
			retVal += char('0' + (c & 15));
		}
		else{
			retVal += char(c);
		}
	}

	retVal += "\"";

	return retVal;
}


} // namespace ibase


