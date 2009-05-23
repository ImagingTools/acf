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

			if (		WriteHeader(className, *registryPtr, headerStream) &&
						WriteRegistryIncludes(className, *registryPtr, stream) &&
						WriteRegistryInfo(className, *registryPtr, stream)){
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

bool CRegistryCodeSaverComp::WriteHeader(
			const std::string& className,
			const icomp::IRegistry& registry,
			std::ofstream& stream) const
{
	std::string includeDefine = className + "_included";

	stream << "#ifndef " << includeDefine << std::endl;
	stream << "#define " << includeDefine << std::endl;
	stream << std::endl << std::endl;

	stream << "#include \"icomp/CComponentBase.h\"" << std::endl;
	stream << "#include \"icomp/CRegistry.h\"" << std::endl;
	stream << std::endl << std::endl;

	istd::CString description = registry.GetDescription();
	if (!description.IsEmpty()){
		stream << "/**" << std::endl;
		stream << "\t" << description.ToString() << "." << std::endl;
		stream << "*/" << std::endl;
	}

	stream << "class " << className << ": public icomp::CComponentBase" << std::endl;
	stream << "{" << std::endl;
	stream << "public:" << std::endl;
	stream << "\ttypedef icomp::CComponentBase BaseClass;" << std::endl;
	stream << "\t" << className << "();" << std::endl;
	stream << std::endl;
	stream << "protected:" << std::endl;
	stream << "\tclass Registry: public icomp::CRegistry" << std::endl;
	stream << "\t{" << std::endl;
	stream << "\tpublic:" << std::endl;
	stream << "\t\tRegistry();" << std::endl;
	stream << std::endl;
	stream << "\tprotected:" << std::endl;
	stream << "\t\tbool SetupRegistry(icomp::CRegistry& registry) const;" << std::endl;
	stream << "\t};" << std::endl;
	stream << std::endl;
	stream << "private:" << std::endl;
	stream << "\t// static members" << std::endl;
	stream << "\tstatic Registry s_registry;" << std::endl;
	stream << "};" << std::endl;

	stream << std::endl << std::endl;
	stream << "#endif // !" << includeDefine << std::endl;

	return !stream.fail();
}


bool CRegistryCodeSaverComp::WriteRegistryIncludes(
			const std::string& className,
			const icomp::IRegistry& registry,
			std::ofstream& stream) const
{
	typedef std::set<std::string> PackageIds;

	PackageIds packageIds;

	icomp::IRegistry::Ids ids = registry.GetElementIds();
	for (		icomp::IRegistry::Ids::const_iterator elementIter = ids.begin();
				elementIter != ids.end();
				++elementIter){
		const std::string& componentName = *elementIter;

		const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(componentName);
		I_ASSERT(infoPtr != NULL);	// used element ID was returned by registry, info must exist.

		const std::string& packageId = infoPtr->address.GetPackageId();

		packageIds.insert(packageId);
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
			std::ofstream& stream) const
{
	NextLine(stream);
	stream << className << "::Registry::Registry()";
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
	stream << "bool " << className << "::Registry::SetupRegistry(icomp::CRegistry& registry) const";
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

	NextLine(stream);
	stream << "// setting of meta info";
	NextLine(stream);
	stream << "registry.SetDescription(\"" << registry.GetDescription().ToString() << "\");";
	NextLine(stream);
	stream << "registry.SetKeywords(\"" << registry.GetKeywords().ToString() << "\");";
	stream << std::endl;
	NextLine(stream);
	stream << "return true;";
	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";

	stream << std::endl << std::endl;

	NextLine(stream);
	stream << "// static members";
	NextLine(stream);
	stream << className << "::Registry " << className << ":: s_registry;";

	stream << std::endl << std::endl;

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
	stream << "\t\t\tfalse);";

	NextLine(stream);
	stream << "if (" << elementInfoName << " == NULL){";
	ChangeIndent(1);
	NextLine(stream);
	stream << "return false;";
	ChangeIndent(-1);
	NextLine(stream);
	stream << "}" << std::endl;


	std::string elementName = "element" + componentName + "Ptr";

	NextLine(stream);
	stream << "icomp::CRegistryElement* " << elementName << " = new icomp::CRegistryElement;";
	stream << std::endl;

	NextLine(stream);
	stream << "if (" << elementName << " != NULL){";
	ChangeIndent(1);

	NextLine(stream);
	stream << elementName << "->Initialize(&";
	stream << componentInfo.address.GetPackageId() << "::" << componentInfo.address.GetComponentId() << "::InitStaticInfo(NULL));";
	stream << std::endl;

	NextLine(stream);
	stream << elementInfoName << "->elementPtr.SetPtr(" << elementName << ");";
	stream << std::endl;

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
	stream << "}";

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
		valueString = (istd::CString("\"") + stringAttribute->GetValue() + "\"").ToString();
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
			valueStrings.push_back((istd::CString("\"") + stringListAttribute->GetValueAt(index) + "\"").ToString());
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


} // namespace ibase


