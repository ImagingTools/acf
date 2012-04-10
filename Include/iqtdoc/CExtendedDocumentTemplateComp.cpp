#include "iqtdoc/CExtendedDocumentTemplateComp.h"


// Qt includes
#include <QtCore/QFileInfo>


namespace iqtdoc
{


// reimplemented (idoc::IDocumentTemplate)

idoc::IDocumentTemplate::Ids CExtendedDocumentTemplateComp::GetDocumentTypeIdsForFile(const QString& filePath) const
{
	idoc::IDocumentTemplate::Ids retVal;

	idoc::IDocumentTemplate::Ids allIds = GetDocumentTypeIds();

	for (		IDocumentTemplate::Ids::const_iterator documentIdIter = allIds.begin();
				documentIdIter != allIds.end();
				++documentIdIter){
		const QByteArray& id = *documentIdIter;

		QStringList extensions;
		iser::IFileLoader* loaderPtr = GetFileLoader(id);
		if ((loaderPtr != NULL) && loaderPtr->GetFileExtensions(extensions)){
			for (		QStringList::const_iterator extensionIter = extensions.begin();
						extensionIter != extensions.end();
						++extensionIter){
				QString extension = *extensionIter;
				if (filePath.endsWith(extension, Qt::CaseInsensitive)){
					retVal.push_back(id);

					break;
				}
			}
		}
	}

	return retVal;
}


// protected methods

// reimplemented (idoc::CSingleDocumentTemplateComp)

istd::IPolymorphic* CExtendedDocumentTemplateComp::ExtractViewInterface(icomp::IComponent* componentPtr) const
{
	return m_viewGuiCompFact.ExtractInterface(componentPtr);
}


} // namespace iqtdoc


