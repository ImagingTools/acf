#ifndef iqtdoc_TQtDocumentManagerWrap_included
#define iqtdoc_TQtDocumentManagerWrap_included


// Qt includes
#include <QtCore/QSettings>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QFileDialog>
#else
#include <QtGui/QFileDialog>
#endif


// ACF includes
#include "idoc/IDocumentManager.h"
#include "ifilegui/CFileDialogLoaderComp.h"


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
	typedef typename BaseClass::Ids Ids;

	// pseudo-reimplemented (iqtgui::TRestorableGuiWrap)
	virtual void OnRestoreSettings(const QSettings& settings);
	virtual void OnSaveSettings(QSettings& settings) const;

	// pseudo-reimplemented (idoc::CSingleDocumentManagerBase)
	virtual QString GetSaveFilePath(const QByteArray& documentTypeId, const QString& currentFilePath) const;

protected:
	/**
		Creates the filter for the file selection dialog.
		\param	documentTypeIdPtr	optional ID of document type if only filter for single document type should be created.
	*/
	QStringList CreateFileDialogFilters(const QByteArray* documentTypeIdPtr = NULL, int flags = 0) const;

	void UpdateLastDirectory(const QString& filePath) const;
	virtual QStringList GetOpenFilePathesFromDialog(const QByteArray* documentTypeIdPtr) const;

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
QString TQtDocumentManagerWrap<Base, Gui>::GetSaveFilePath(const QByteArray& documentTypeId, const QString& currentFilePath) const
{
	QStringList filters = CreateFileDialogFilters(&documentTypeId, ifile::IFilePersistence::QF_FILE | ifile::IFilePersistence::QF_SAVE);

	QString defaultFilter = "*." + QFileInfo(currentFilePath).completeSuffix();
	QString filePath = QFileDialog::getSaveFileName(
				Gui::GetWidget(),
				Gui::tr("Save..."),
				currentFilePath.isEmpty()? m_lastDirectory: currentFilePath, filters.join(";;"),
				&defaultFilter);

	if (!filePath.isEmpty()){
		UpdateLastDirectory(filePath);
	}

	return filePath;
}


// protected methods

template <class Base, class Gui>
QStringList TQtDocumentManagerWrap<Base, Gui>::CreateFileDialogFilters(const QByteArray* documentTypeIdPtr, int flags) const
{
	QStringList filters;

	Ids docTypeIds = BaseClass::GetDocumentTypeIds();

	QStringList allExt;

	if (documentTypeIdPtr != NULL){
		ifile::IFileTypeInfo* typeInfoPtr = BaseClass::GetDocumentFileTypeInfo(*documentTypeIdPtr);
		if (typeInfoPtr != NULL){
			ifilegui::CFileDialogLoaderComp::AppendLoaderFilterList(*typeInfoPtr, flags, allExt, filters);
		}
	}
	else{
		for (		typename Ids::const_iterator docTypeIter = docTypeIds.begin();
					docTypeIter != docTypeIds.end();
					++docTypeIter){
			ifile::IFileTypeInfo* typeInfoPtr = BaseClass::GetDocumentFileTypeInfo(*docTypeIter);
			if (typeInfoPtr != NULL){
				ifilegui::CFileDialogLoaderComp::AppendLoaderFilterList(*typeInfoPtr, flags, allExt, filters);
			}
		}
	}

	if ((allExt.size() > 1) && ((flags & ifile::IFilePersistence::QF_SAVE) == 0)){
		filters.prepend(Gui::tr("All known documents (%1)").arg("*." + allExt.join(" *.")));
	}

	return filters;
}


template <class Base, class Gui>
void TQtDocumentManagerWrap<Base, Gui>::UpdateLastDirectory(const QString& filePath) const
{
	QFileInfo fileInfo(filePath);

	m_lastDirectory = fileInfo.dir().absolutePath();
}


template <class Base, class Gui>
QStringList TQtDocumentManagerWrap<Base, Gui>::GetOpenFilePathesFromDialog(const QByteArray* documentTypeIdPtr) const
{
	QStringList filters = CreateFileDialogFilters(documentTypeIdPtr, ifile::IFilePersistence::QF_FILE | ifile::IFilePersistence::QF_LOAD);

	return QFileDialog::getOpenFileNames(Gui::GetWidget(), Gui::tr("Open Files..."), m_lastDirectory, filters.join(";;"));
}


} // namespace iqtdoc


#endif // !iqtdoc_TQtDocumentManagerWrap_included


