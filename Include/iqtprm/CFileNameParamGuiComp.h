#ifndef iqtprm_CFileNameParamGuiComp_included
#define iqtprm_CFileNameParamGuiComp_included


// Qt includes
#include <QDirModel>


// ACF includes
#include "iprm/IFileNameParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtprm/Generated/ui_CFileNameParamGuiComp.h"


namespace iqtprm
{


class CFileNameParamGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CFileNameParamGuiComp,
			iprm::IFileNameParam>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CFileNameParamGuiComp,
				iprm::IFileNameParam> BaseClass;

	I_BEGIN_COMPONENT(CFileNameParamGuiComp)
		I_ASSIGN(m_pathLabelAttrPtr, "PathLabel", "Label for the file editor", false, "Directory");
	I_END_COMPONENT

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();

protected slots:
	void on_BrowseButton_clicked();
	void on_DirEdit_editTextChanged(const QString& text);

private:
	void SetPathToEditor(const QString& path) const;
	void MakeSelectionHint(const QString& text) const;
	QIcon GetFileIcon(const QString& filePath) const;
	void OnPathEdited(const QString& path) const;

private:
	I_ATTR(istd::CString, m_pathLabelAttrPtr);

	QDirModel m_directoryModel;
	istd::TDelPtr<QLabel> m_fileIconPtr;
};


} // namespace iqtprm


#endif // !iqtprm_CFileNameParamGuiComp_included


