#ifndef iqtprm_CFileNameParamGuiComp_included
#define iqtprm_CFileNameParamGuiComp_included


// Qt includes
#include <QtGui/QDirModel>

// ACF includes
#include "ifile/IFileTypeInfo.h"

#include "ifile/IFileNameParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtprm/Generated/ui_CFileNameParamGuiComp.h"


namespace iqtprm
{


class CFileNameParamGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CFileNameParamGuiComp,
			ifile::IFileNameParam>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CFileNameParamGuiComp,
				ifile::IFileNameParam> BaseClass;

	I_BEGIN_COMPONENT(CFileNameParamGuiComp);
		I_ASSIGN(m_pathLabelAttrPtr, "PathLabel", "Label for the file editor", false, "Directory");
		I_ASSIGN(m_startHintAttrPtr, "StartHint", "Start hint for the editor", false, "<Enter path>");
		I_ASSIGN(m_fileTypeInfoPtr, "FileTypeInfo", "Allows to retrieve optional file type information for file selection dialog", false, "FileTypeInfo");
		I_ASSIGN(m_readOnlyAttrPtr, "ReadOnly", "True if read only", true, false);
		I_ASSIGN(m_filesComboAttrPtr, "FilesComboBox", "Show combobox with files", true, true);
		I_ASSIGN(m_labelWidthAttrPtr, "LabelWidth", "Fixed label width (in pixels)", false, 100);
		I_ASSIGN(m_defaultDirPtr, "DefaultPath", "Default path to search", false, "C:/");
	I_END_COMPONENT;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiRetranslate();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void UpdateGui(int updateFlags = 0);

protected Q_SLOTS:
	void on_BrowseButton_clicked();
	void on_DirEdit_editTextChanged(const QString& text);
	void OnDirectoryUp();

private:
	void SetPathToEditor(const QString& path) const;
	void MakeSelectionHint(const QString& text) const;
	QIcon GetFileIcon(const QString& filePath) const;
	void OnPathEdited(const QString& path) const;
	QString GetPathFromEditor() const;
	bool IsLabelNeeded() const;

private:
	I_ATTR(QString, m_pathLabelAttrPtr);
	I_ATTR(QString, m_startHintAttrPtr);
	I_ATTR(bool, m_readOnlyAttrPtr);
	I_ATTR(bool, m_filesComboAttrPtr);
	I_ATTR(int, m_labelWidthAttrPtr);
	I_REF(ifile::IFileTypeInfo, m_fileTypeInfoPtr);
	I_REF(ifile::IFileNameParam, m_defaultDirPtr);

	QDirModel m_directoryModel;
};


} // namespace iqtprm


#endif // !iqtprm_CFileNameParamGuiComp_included


