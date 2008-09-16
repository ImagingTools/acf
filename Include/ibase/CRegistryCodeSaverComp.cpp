#include "ibase/CRegistryCodeSaverComp.h"


#include <fstream>


namespace ibase
{


// reimplemented (iser::IFileLoader)

bool CRegistryCodeSaverComp::IsOperationSupported(
		const istd::IChangeable* dataObjectPtr,
		const istd::CString* /*filePathPtr*/,
		bool /*forLoading*/,
		bool forSaving) const
{
	if (dynamic_cast<const icomp::IRegistry*>(dataObjectPtr) == NULL){
		return false;
	}

	return forSaving;
}


int CRegistryCodeSaverComp::LoadFromFile(istd::IChangeable& /*data*/, const istd::CString& /*filePath*/) const
{
	return StateFailed;
}


int CRegistryCodeSaverComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	const icomp::IRegistry* registryPtr = dynamic_cast<const icomp::IRegistry*>(&data);
	if (registryPtr != NULL){
		std::ofstream stream(filePath.ToString().c_str());

		icomp::IRegistry::Ids ids = registryPtr->GetElementIds();
		stream << "bool SetupRegistry(icomp::CRegistry& registry)" << std::endl;
		stream << "{" << std::endl;
		for (		icomp::IRegistry::Ids::const_iterator iter = ids.begin();
					iter != ids.end();
					++iter){
			std::string elementId = *iter;
			const icomp::IRegistry::ElementInfo* infoPtr = registryPtr->GetElementInfo(elementId);
			I_ASSERT(infoPtr != NULL);	// used element ID was returned by registry, info must exist.

			stream << "\ticomp::IRegistry::ElementInfo* infoPtr = registry.InsertElementInfo(" << std::endl;
			stream << "\t\t\t\t\"" << elementId << "\"," << std::endl;
			stream << "\t\t\t\ticomp::CComponentAddress(\"" << infoPtr->address.GetPackageId();
			stream << "\", \"" << infoPtr->address.GetComponentId() << "\")," << std::endl;
			stream << "\t\t\t\ttrue);" << std::endl;
			stream << "\tif (infoPtr == NULL){" << std::endl;
			stream << "\t\treturn false;" << std::endl;
			stream << "\t}" << std::endl;

			// TODO: add creating of rest code
		}

		stream << "\treturn true;" << std::endl;
		stream << "}" << std::endl;

		return StateOk;
	}

	return StateFailed;
}


const istd::CString& CRegistryCodeSaverComp::GetLastLoadFileName() const
{
	static istd::CString empty;

	return empty;
}


const istd::CString& CRegistryCodeSaverComp::GetLastSaveFileName() const
{
	return m_lastSaveFileName;
}


} // namespace ibase


