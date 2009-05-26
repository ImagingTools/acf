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

			if (		WriteHeader(className, *registryPtr, composedAddresses, headerStream) &&
						WriteIncludes(className, realAddresses, stream) &&
						WriteRegistryInfo(className, *registryPtr, composedAddresses, stream)){
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
		const std::string& componentName = *elementIter;

		const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(componentName);
		I_ASSERT(infoPtr != NULL);	// used element ID was returned by registry, info must exist.

		if (		(realAddresses.find(infoPtr->address) == realAddresses.end()) &&
					(composedAddresses.find(infoPtr->address) == composedAddresses.end())){
			if (m_registriesManagerCompPtr.IsValid()){
				const icomp::IRegistry* subregistryPtr = m_registriesManagerCompPtr->GetRegistry(infoPtr->address);
				if (subregistryPtr != NULL){
					composedAddresses.insert(infoPtr->address);

					retVal = retVal || AppendAddresses(*subregistryPtr, realAddresses, composedAddresses);
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


bool CRegistryCodeSaverComp::WriteHeader(
			const std::string& className,
			const icomp::IRegistry& registry,
			const Addresses& composedAddresses,
			std::ofstream& stream) const
{
	std::string includeDefine = className + "_included";

	stream << "#ifndef " << includeDefine;

	NextLine(stream);
	stream << "#define " << includeDefine;
	stream << std::endl << std::endl;

	NextLine(stream);
	stream << "#include \"icomp/CComponentStaticInfoBase.h\"";
	NextLine(stream);
	stream << "#include \"icomp/CRegistry.h\"";
	NextLine(stream);
	stream << "#include \"icomp/CCompositeComponentStaticInfo.h\"";

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
	stream << "class " << className << ":";

	NextLine(stream);
	stream << "\t\t\tvirtual private icomp::CComponentStaticInfoBase";

	NextLine(stream);
	stream << "{";

	NextLine(stream);
	stream << "public:";
	ChangeIndent(1);

	NextLine(stream);
	stream << "typedef icomp::CComponentBase BaseClass;";
	stream << std::endl;

	NextLine(stream);
	stream << className << "();";
	stream << std::endl;

	ChangeIndent(-1);
	NextLine(stream);
	stream << "protected:";
	ChangeIndent(1);

	WriteRegistryClassDeclaration(className, "MainRegistry", registry, stream);

	if (m_registriesManagerCompPtr.IsValid()){
		for (		Addresses::const_iterator iter = composedAddresses.begin();
					iter != composedAddresses.end();
					++iter){
			const icomp::CComponentAddress& address = *iter;

			const icomp::IRegistry* subregistryPtr = m_registriesManagerCompPtr->GetRegistry(address);

			if (subregistryPtr != NULL){
				stream << std::endl;

				WriteRegistryClassDeclaration(className, GetRegistryName(address), *subregistryPtr, stream);
			}
			else{
				NextLine(stream);
				stream << "// Registry for address '" << address.GetPackageId() << "/" << address.GetComponentId() << "' could not be created";
			}
		}
	}

	stream << std::endl;

	ChangeIndent(-1);
	NextLine(stream);
	stream << "private:";
	ChangeIndent(1);

	NextLine(stream);
	stream << "MainRegistry m_mainRegistry;";

	for (		Addresses::const_iterator iter = composedAddresses.begin();
				iter != composedAddresses.end();
				++iter){
		const icomp::CComponentAddress& address = *iter;

		NextLine(stream);
		std::string subregistryName = GetRegistryName(address);
		stream << subregistryName << " m_sub" << subregistryName << ";";
	}

	ChangeIndent(-1);
	NextLine(stream);
	stream << "};";

	stream << std::endl << std::endl;

	NextLine(stream);
	stream << "#endif // !" << includeDefine;

	return !stream.fail();
}


bool CRegistryCodeSaverComp::WriteIncludes(
			const std::string& className,
			const Addresses& addresses,
			std::ofstream& stream) const
{
	typedef std::set<std::string> PackageIds;
	PackageIds packageIds;

	for (		Addresses::const_iterator addressIter = addresses.begin();
				addressIter != addresses.end();
				++addressIter){
		const icomp::CComponentAddress& address = *addressIter;

		packageIds.insert(address.GetPackageId());
	}

	stream << "#include \"" << className << ".h\"" << std::endl;
	stream << std::endl << std::endl;
	stream << "#include \"icomp/TSingleAttribute.h\"" << std::endl;
	stream << "#include \"icomp/TMultiAttribute.h\"" << std::endl;
	stream << "#include \"icomp/CRegistryElement.h\"" << std::endl;
	stream << std::endl << std::endl;
	for (		PackageIds::const_iterator packageIter = packageIds.begin();
				packageIter != packageIds.end();
				++packageIter){
		const std::string& packageId = *packageIter;

		stream << "#include \"" << packageId << "/" << packageId << ".h\"" << std::endl;
	}

	stream << std::endl;
	stream << std::endl;

	return !stream.fail();
}


bool CRegistryCodeSaverComp::WriteRegistryInfo(
			const std::string& className,
			const icomp::IRegistry& registry,
			const Addresses& composedAddresses,
			std::ofstream& stream) const
{
	NextLine(stream);
	stream << className << "::" << className << "()";

	NextLine(stream);
	stream << ":\tm_mainRegistry(this)";

	for (		Addresses::const_iterator iter = composedAddresses.begin();
				iter != composedAddresses.end();
				++iter){
		const icomp::CComponentAddress& address = *iter;

		stream << ",";

		NextLine(stream);
		std::string subregistryName = GetRegistryName(address);
		stream << "\tm_sub" << subregistryName << "(this)";
	}

	NextLine(stream);
	stream << "{";

	NextLine(stream);
	stream << "}";

	stream << std::endl << std::endl;

	WriteRegistryClassBody(className, "MainRegistry", registry, stream);
	stream << std::endl << std::endl;

	if (m_registriesManagerCompPtr.IsValid()){
		for (		Addresses::const_iterator iter = composedAddresses.begin();
					iter != composedAddresses.end();
					++iter){
			const icomp::CComponentAddress& address = *iter;

			const icomp::IRegistry* subregistryPtr = m_registriesManagerCompPtr->GetRegistry(address);

			if (subregistryPtr != NULL){
				WriteRegistryClassBody(className, GetRegistryName(address), *subregistryPtr, stream);
				stream << std::endl << std::endl;
			}
		}
	}

	return !stream.fail();
}


bool CRegistryCodeSaverComp::WriteComponentInfo(
			const icomp::IRegistry& registry,
			const std::string& componentName,
			const icomp::IRegistry::ElementInfo& componentInfo,
			std::ofstream& stream) const
{
	std::string elementInfoName = "info" + componentName + "Ptr";
	NextLine(stream);
	stream << "icomp::IRegistry::ElementInfo* " << elementInfoName << " = registry.InsertElementInfo(";
	NextLine(stream);
	stream << "\t\t\t\"" << componentName << "\",";
	NextLine(stream);
	stream << "\t\t\ticomp::CComponentAddress(\"" << componentInfo.address.GetPackageId();
	stream << "\", \"" << componentInfo.address.GetComponentId() << "\"),";
	NextLine(stream);
	stream << "\t\t\ttrue);";

	NextLine(stream);
	stream << "if ((" << elementInfoName << " != NULL) && " << elementInfoName << "->elementPtr.IsValid()){";
	ChangeIndent(1);

	if (componentInfo.elementPtr.IsValid()){
		icomp::IRegistryElement& component = *componentInfo.elementPtr;
		icomp::IRegistryElement::Ids attributeIds = component.GetAttributeIds();

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

				NextLine(stream);
				stream << "icomp::IRegistryElement::AttributeInfo* " << attributeInfoName << " = ";
				stream << elementInfoName << "->elementPtr->InsertAttributeInfo(\"" << attributeId << "\", true);";

				NextLine(stream);
				stream << "if (" << attributeInfoName << " == NULL){";
				ChangeIndent(1);
				NextLine(stream);
				stream << "return false;";
				ChangeIndent(-1);
				NextLine(stream);
				stream << "}";

				if (!attrInfoPtr->exportId.empty()){
					NextLine(stream);
					stream << attributeInfoName << "->exportId = \"" << attrInfoPtr->exportId << "\";";
				}

				if (attrInfoPtr->attributePtr.IsValid()){
					WriteAttribute(attributeId, attributeInfoName, *attrInfoPtr->attributePtr, stream);
				}
			}
		}
	}

	ChangeIndent(-1);
	NextLine(stream);
	stream << "}" << std::endl;

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
		NextLine(stream);
		stream << attributeType << "* n" << attributeInfoName << " = dynamic_cast<" << attributeType << "*>(" << attributeInfoName << "->attributePtr.GetPtr());";
		NextLine(stream);
		stream << "I_ASSERT(n" << attributeInfoName << " != NULL);";
		for (		std::list<std::string>::const_iterator iter = valueStrings.begin();
					iter != valueStrings.end();
					++iter){
			NextLine(stream);
			stream << "n" << attributeInfoName << "->InsertValue(" << *iter << ");";
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
	stream << "class " << registryClassName << ":";
	NextLine(stream);
	stream << "\t\t\tpublic icomp::CRegistry,";
	NextLine(stream);
	stream << "\t\t\tpublic icomp::CCompositeComponentStaticInfo";
	NextLine(stream);
	stream << "{";

	NextLine(stream);
	stream << "public:";
	ChangeIndent(1);

	NextLine(stream);
	stream << "typedef icomp::CRegistry BaseClass;";
	NextLine(stream);
	stream << "typedef icomp::CCompositeComponentStaticInfo BaseClass2;";
	stream << std::endl;

	NextLine(stream);
	stream << registryClassName << "(const icomp::IComponentStaticInfo* factoryPtr);";
	stream << std::endl;
	ChangeIndent(-1);

	NextLine(stream);
	stream << "protected:";
	ChangeIndent(1);

	NextLine(stream);
	stream << "static bool SetupRegistry(icomp::CRegistry& registry);";
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
	stream << baseClassName << "::" << registryClassName << "::" << registryClassName << "(const icomp::IComponentStaticInfo* factoryPtr)";

	NextLine(stream);
	stream << ":\tBaseClass(factoryPtr),";
	NextLine(stream);
	stream << "\tBaseClass2(*this)";

	NextLine(stream);
	stream << "{";
	ChangeIndent(1);

	NextLine(stream);
	stream << "SetupRegistry(*this);";

	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";

	stream << std::endl << std::endl;

	NextLine(stream);
	stream << "bool " << baseClassName << "::" << registryClassName << "::SetupRegistry(icomp::CRegistry& registry)";

	NextLine(stream);
	stream << "{";
	ChangeIndent(1);

	icomp::IRegistry::Ids ids = registry.GetElementIds();
	for (		icomp::IRegistry::Ids::const_iterator elementIter = ids.begin();
				elementIter != ids.end();
				++elementIter){
		const std::string& componentName = *elementIter;

		const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(componentName);
		I_ASSERT(infoPtr != NULL);	// used element ID was returned by registry, info must exist.

		if (elementIter != ids.begin()){
			stream << std::endl;
		}

		NextLine(stream);
		stream << "// element '" << componentName << "' of type " << infoPtr->address.GetPackageId() << "::" << infoPtr->address.GetComponentId();

		WriteComponentInfo(registry, componentName, *infoPtr, stream);
	}

	stream << std::endl;

	NextLine(stream);
	stream << "// interface export";
	const icomp::IRegistry::ExportedInterfacesMap& interfacesMap = registry.GetExportedInterfacesMap();
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

	stream << std::endl;

	istd::CString description = registry.GetDescription();
	istd::CString keywords = registry.GetKeywords();

	if (!description.empty() || !keywords.empty()){
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

		stream << std::endl;
	}

	NextLine(stream);
	stream << "return true;";
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


std::string CRegistryCodeSaverComp::GetRegistryName(const icomp::CComponentAddress& address) const
{
	return address.GetComponentId() + "Registry";
//	return address.GetPackageId() + address.GetComponentId() + "Registry";
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


