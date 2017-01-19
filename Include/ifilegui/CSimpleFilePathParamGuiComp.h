#ifndef ifilegui_CSimpleFilePathParamGuiComp_included
#define ifilegui_CSimpleFilePathParamGuiComp_included


// ACF includes
#include <ifile/IFileTypeInfo.h>
#include <ifile/IFileNameParam.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <GeneratedFiles/ifilegui/ui_CSimpleFilePathParamGuiComp.h>


namespace ifilegui
{


class CSimpleFilePathParamGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CSimpleFilePathParamGuiComp,
			ifile::IFileNameParam>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CSimpleFilePathParamGuiComp,
				ifile::IFileNameParam> BaseClass;

	I_BEGIN_COMPONENT(CSimpleFilePathParamGuiComp);
		I_ASSIGN(m_pathLabelAttrPtr, "PathLabel", "Label for the file editor", false, "Directory");
		I_ASSIGN(m_startHintAttrPtr, "StartHint", "Start hint for the editor", false, "<Enter path>");
		I_ASSIGN(m_fileTypeInfoPtr, "FileTypeInfo", "Allows to retrieve optional file type information for file selection dialog", false, "FileTypeInfo");
		I_ASSIGN(m_readOnlyAttrPtr, "ReadOnly", "True if read only", true, false);
		I_ASSIGN(m_filesComboAttrPtr, "FilesComboBox", "Show combobox with files", true, true);
		I_ASSIGN(m_labelWidthAttrPtr, "LabelWidth", "Fixed label width (in pixels)", false, 100);
		I_ASSIGN(m_defaultDirPtr, "DefaultPath", "Default path to search", false, "C:/");
		I_ASSIGN(m_showBrowseButtonAttrPtr, "ShowBrowseButton", "If enabled, the browse button will be shown", true, true);
		I_ASSIGN(m_filePathValidationExpressionAttrPtr, "ValidationExpression", "Regular expression used for path validation", false, "^[/,\\\\,\\w,\\s-]+");
		I_ASSIGN(m_useSaveDialogAttrPtr, "UseSaveDialog", "Use save dialog instead of open one", true, false);
	I_END_COMPONENT;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiRetranslate();

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void on_BrowseButton_clicked();
	void on_PathEdit_textChanged(const QString& text);

private:
	void SetPathToEditor(const QString& path) const;
	void OnPathEdited(const QString& path);
	QString GetPathFromEditor() const;
	bool IsLabelNeeded() const;

private:
	I_TEXTATTR(m_pathLabelAttrPtr);
	I_TEXTATTR(m_startHintAttrPtr);
	I_ATTR(bool, m_readOnlyAttrPtr);
	I_ATTR(bool, m_filesComboAttrPtr);
	I_ATTR(int, m_labelWidthAttrPtr);
	I_ATTR(bool, m_showBrowseButtonAttrPtr);
	I_ATTR(bool, m_useSaveDialogAttrPtr);
	I_ATTR(QString, m_filePathValidationExpressionAttrPtr);
	I_REF(ifile::IFileTypeInfo, m_fileTypeInfoPtr);
	I_REF(ifile::IFileNameParam, m_defaultDirPtr);
};


} // namespace ifilegui


#endif // !ifilegui_CSimpleFilePathParamGuiComp_included


