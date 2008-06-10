#include "iqt/CCompositePackageStaticInfo.h"


#include "icomp/CComponentAddress.h"


namespace iqt
{


CCompositePackageStaticInfo::CCompositePackageStaticInfo(const QDir& packageDir, const icomp::IRegistriesManager* registriesManagerPtr)
:	m_registriesManager(*registriesManagerPtr)
{
	I_ASSERT(registriesManagerPtr != NULL);

	m_packageId = QFileInfo(packageDir.absolutePath()).baseName().toStdString();

	QStringList filters;
	filters.append("*.arx");
	QStringList componentsInfo = packageDir.entryList(filters, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
	for (		QStringList::iterator iter = componentsInfo.begin();
				iter != componentsInfo.end();
				++iter){
		QFileInfo componentFileInfo(*iter);
		m_subcomponentInfos.InsertLocal(componentFileInfo.baseName().toStdString(), ComponentInfo());
	}
}


//	reimplemented (icomp::IComponentStaticInfo)

icomp::IComponent* CCompositePackageStaticInfo::CreateComponent(const icomp::IComponentContext* /*contextPtr*/) const
{
	return NULL;
}


const icomp::IComponentStaticInfo::InterfaceExtractors& CCompositePackageStaticInfo::GetInterfaceExtractors() const
{
	static InterfaceExtractors empty;

	return empty;
}


const icomp::IComponentStaticInfo::AttributeInfos& CCompositePackageStaticInfo::GetAttributeInfos() const
{
	static AttributeInfos empty;

	return empty;
}


icomp::IComponentStaticInfo::Ids CCompositePackageStaticInfo::GetSubcomponentIds() const
{
	Ids retVal;

	m_subcomponentInfos.GetKeys(retVal);

	return retVal;
}


const icomp::IComponentStaticInfo* CCompositePackageStaticInfo::GetSubcomponent(const std::string& subcomponentId) const
{
	ComponentInfo* infoPtr = m_subcomponentInfos.FindLocalElement(subcomponentId);
	if (infoPtr != NULL){
		if (!infoPtr->isInitialized){
			icomp::CComponentAddress address(m_packageId, subcomponentId);

			const icomp::IRegistry* registryPtr = m_registriesManager.GetRegistry(address, NULL);
			if (registryPtr != NULL){
				infoPtr->componentInfoPtr.SetPtr(new icomp::CCompositeComponentStaticInfo(registryPtr));
			}

			infoPtr->isInitialized = true;
		}

		return infoPtr->componentInfoPtr.GetPtr();
	}

	return NULL;

}


bool CCompositePackageStaticInfo::RegisterInterfaceExtractor(const std::string& /*interfaceId*/, InterfaceExtractorPtr /*extractorPtr*/)
{
	return false;
}


bool CCompositePackageStaticInfo::RegisterAttributeInfo(const std::string& /*attributeId*/, const icomp::IAttributeStaticInfo* /*attributeInfoPtr*/)
{
	return false;
}


bool CCompositePackageStaticInfo::RegisterSubcomponentInfo(const std::string& /*subcomponentId*/, const IComponentStaticInfo* /*componentInfoPtr*/)
{
	return false;
}


istd::CString CCompositePackageStaticInfo::GetDescription() const
{
	return "";
}


istd::CString CCompositePackageStaticInfo::GetKeywords() const
{
	return "";
}


}//namespace iqt


