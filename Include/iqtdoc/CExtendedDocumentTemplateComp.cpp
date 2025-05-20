#include <iqtdoc/CExtendedDocumentTemplateComp.h>


// Qt includes
#include <QtCore/QFileInfo>


namespace iqtdoc
{


// reimplemented (idoc::IDocumentTemplate)

idoc::IDocumentTemplate::Ids CExtendedDocumentTemplateComp::GetDocumentTypeIdsForFile(const QString& filePath) const
{
	idoc::IDocumentTemplate::Ids retVal;

	idoc::IDocumentTemplate::Ids allIds = GetDocumentTypeIds();

	QFileInfo fileInfo(filePath);

	for (		IDocumentTemplate::Ids::const_iterator documentIdIter = allIds.begin();
				documentIdIter != allIds.end();
				++documentIdIter){
		const QByteArray& id = *documentIdIter;

		QStringList extensions;
		ifile::IFilePersistence* loaderPtr = GetFileLoader(id);
		if ((loaderPtr != NULL) && loaderPtr->GetFileExtensions(extensions)){
			for (		QStringList::const_iterator extensionIter = extensions.begin();
						extensionIter != extensions.end();
						++extensionIter){
				QString extension = *extensionIter;

				if (fileInfo.suffix().compare(extension, Qt::CaseInsensitive) == 0){
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

idoc::IDocumentTemplate::ViewUniquePtr CExtendedDocumentTemplateComp::ExtractViewInterface(std::unique_ptr<icomp::IComponent>& componentPtr) const
{
	ViewUniquePtr viewPtr(componentPtr.get(), [&componentPtr, this]()
	{
		iqtgui::IGuiObject* guiPtr = m_viewGuiCompFact.ExtractInterface(componentPtr.get());

		return guiPtr;
	});

	componentPtr.release();

	return viewPtr;
}


} // namespace iqtdoc


