#ifndef iqtdoc_TQtDocumentManagerWrap_included
#define iqtdoc_TQtDocumentManagerWrap_included


// Qt includes
#include <QFileDialog>
#include <QSettings>


// ACF includes
#include "idoc/IDocumentManager.h"

#include "iqtgui/CFileDialogLoaderComp.h"


namespace iqtdoc
{


/**	
	Wrapper for general functionality of a Qt based document workspace.
*/
template <class Base, class Gui>
class TQtDocumentManagerWrap: public Base, public Gui
{
public:
	typedef Base BaseClass;

	// pseudo-reimplemented (iqtgui::TRestorableGuiWrap)
	virtual void OnRestoreSettings(const QSettings& settings);
	virtual void OnSaveSettings(QSettings& settings) const;

	// pseudo-reimplemented (idoc::CSingleDocumentManagerBase)
	virtual QString GetSaveFilePath(const std::string& documentTypeId) const;

protected:
	/**
		Creates the filter for the file selection dialog.
		\param	documentTypeIdPtr	optional ID of document type if only filter for single document type should be created.
	*/
	QString CreateFileDialogFilter(const std::string* documentTypeIdPtr = NULL, int flags = 0) const;

	void UpdateLastDirectory(const QString& filePath) const;
	virtual QStringList GetOpenFilePathesFromDialog(const std::string* documentTypeIdPtr) const;

private:
	mutable QString m_lastDirectory;
};


// public methods

// pseudo-reimplemented (TRestorableGuiWrap)

template <class Base, class Gui>
void TQtDocumentManagerWrap<Base, Gui>::OnRestoreSettings(const QSettings& settings)
{
	QVariant valueNotSet = QVariant(-1);

	QVariant lastDirectory = settings.value("Workspace/LastOpenDocumentDirectory", valueNotSet);

	if (lastDirectory != valueNotSet){
		m_lastDirectory = lastDirectory.toString();
	}
}


template <class Base, class Gui>
void TQtDocumentManagerWrap<Base, Gui>::OnSaveSettings(QSettings& settings) const
{
	settings.setValue("Workspace/LastOpenDocumentDirectory", m_lastDirectory);
}


// pseudo-reimplemented (idoc::CSingleDocumentManagerBase)

template <class Base, class Gui>
QString TQtDocumentManagerWrap<Base, Gui>::GetSaveFilePath(const std::string& documentTypeId) const
{
	QString filter = CreateFileDialogFilter(&documentTypeId, iser::IFileLoader::QF_FILE | iser::IFileLoader::QF_SAVE);

	QString filePath = QFileDialog::getSaveFileName(NULL, Gui::tr("Save..."), m_lastDirectory, filter);

	UpdateLastDirectory(filePath);

	return filePath;
}


// protected methods

template <class Base, class Gui>
QString TQtDocumentManagerWrap<Base, Gui>::CreateFileDialogFilter(const std::string* documentTypeIdPtr, int flags) const
{
	QString retVal;

	Ids docTypeIds = BaseClass::GetDocumentTypeIds();

	QString allExt;
	int filtersCount = 0;

	if (documentTypeIdPtr != NULL){
		iser::IFileTypeInfo* typeInfoPtr = BaseClass::GetDocumentFileTypeInfo(*documentTypeIdPtr);
		if (typeInfoPtr != NULL){
			filtersCount += iqtgui::CFileDialogLoaderComp::AppendLoaderFilterList(*typeInfoPtr, flags, allExt, retVal);
		}
	}
	else{
		for (		Ids::const_iterator docTypeIter = docTypeIds.begin();
					docTypeIter != docTypeIds.end();
					++docTypeIter){
			iser::IFileTypeInfo* typeInfoPtr = BaseClass::GetDocumentFileTypeInfo(*docTypeIter);
			if (typeInfoPtr != NULL){
				filtersCount += iqtgui::CFileDialogLoaderComp::AppendLoaderFilterList(*typeInfoPtr, flags, allExt, retVal);
			}
		}
	}

	if ((filtersCount > 1) && ((flags & iser::IFileLoader::QF_SAVE) == 0)){
		retVal = Gui::tr("All known documents (%1)\n").arg(allExt) + retVal;
	}

	return retVal;
}

template <class Base, class Gui>
void TQtDocumentManagerWrap<Base, Gui>::UpdateLastDirectory(const QString& filePath) const
{
	QFileInfo fileInfo(filePath);

	m_lastDirectory = fileInfo.dir().absolutePath();
}


template <class Base, class Gui>
QStringList TQtDocumentManagerWrap<Base, Gui>::GetOpenFilePathesFromDialog(const std::string* documentTypeIdPtr) const
{
	QString filter = CreateFileDialogFilter(documentTypeIdPtr, iser::IFileLoader::QF_FILE | iser::IFileLoader::QF_LOAD);

	return QFileDialog::getOpenFileNames(Gui::GetWidget(), Gui::tr("Open Files..."), m_lastDirectory, filter);
}


} // namespace iqtdoc


#endif // !iqtdoc_TQtDocumentManagerWrap_included


