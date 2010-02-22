#include "icmpstr/CComponentHelpFileProviderComp.h"


// Qt includes
#include <QFileInfo>
#include <QDir>

#include "iqt/iqt.h"


namespace icmpstr
{


// reimplemented (idoc::IHelpFileProvider)

double CComponentHelpFileProviderComp::GetHelpQuality(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const
{
	icomp::CComponentAddress address;

	if (ExtractComponentAddress(contextText, contextObjectPtr, address)){
		istd::CString filePath = GetInfoFilePath(address);

		if (!filePath.IsEmpty() && QFileInfo(iqt::GetQString(filePath)).exists()){
			return 1.0;
		}

		filePath = GetSlaveFilePath(address);

		if (!filePath.IsEmpty() && QFileInfo(iqt::GetQString(filePath)).exists()){
			return 0.5;
		}
	}

	if (m_classHelpProviderCompPtr.IsValid()){
		return m_classHelpProviderCompPtr->GetHelpQuality(contextText, contextObjectPtr);
	}

	return 0;
}


istd::CString CComponentHelpFileProviderComp::GetHelpFilePath(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const
{
	icomp::CComponentAddress address;

	if (ExtractComponentAddress(contextText, contextObjectPtr, address)){
		istd::CString filePath = GetInfoFilePath(address);

		if (!filePath.IsEmpty() && QFileInfo(iqt::GetQString(filePath)).exists()){
			return filePath;
		}

		filePath = GetSlaveFilePath(address);

		if (!filePath.IsEmpty() && QFileInfo(iqt::GetQString(filePath)).exists()){
			return filePath;
		}
	}

	if (m_classHelpProviderCompPtr.IsValid()){
		return m_classHelpProviderCompPtr->GetHelpFilePath(contextText, contextObjectPtr);
	}

	return "";
}


// protected methods

istd::CString CComponentHelpFileProviderComp::GetInfoFilePath(const icomp::CComponentAddress& componentAddress) const
{
	if (m_metaInfoManagerCompPtr.IsValid()){
		QString infoPath = iqt::GetQString(m_metaInfoManagerCompPtr->GetComponentInfoPath(componentAddress));
		if (!infoPath.isEmpty()){
			QDir packageDir(infoPath);
			if (packageDir.exists()){
				return iqt::GetCString(packageDir.absoluteFilePath("Description.html"));
			}
		}
	}

	return "";
}


istd::CString CComponentHelpFileProviderComp::GetSlaveFilePath(const icomp::CComponentAddress& componentAddress) const
{
	if (m_packagesLoaderInfoCompPtr.IsValid() && m_classHelpProviderCompPtr.IsValid()){
		const icomp::IComponentStaticInfo* packageInfoPtr = m_packagesLoaderInfoCompPtr->GetSubcomponentInfo(componentAddress.GetPackageId());
		if (packageInfoPtr != NULL){
			const icomp::IComponentStaticInfo* componentInfoPtr = packageInfoPtr->GetSubcomponentInfo(componentAddress.GetComponentId());
			if (componentInfoPtr != NULL){
				istd::CClassInfo classInfo(*componentInfoPtr);

				while (classInfo.IsTemplateClass()){
					classInfo = classInfo.GetTemplateParam();
				}

				return m_classHelpProviderCompPtr->GetHelpFilePath("", &classInfo);
			}
		}
	}

	return "";
}


bool CComponentHelpFileProviderComp::ExtractComponentAddress(
			const istd::CString& /*contextText*/,
			const istd::IPolymorphic* contextObjectPtr,
			icomp::CComponentAddress& result) const
{
	const icomp::CComponentAddress* addressPtr = dynamic_cast<const icomp::CComponentAddress*>(contextObjectPtr);
	if (addressPtr != NULL){
		result = *addressPtr;

		return true;
	}

	return false;
}


} // namespace icmpstr


