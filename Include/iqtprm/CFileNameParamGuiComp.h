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
	I_END_COMPONENT

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();

protected slots:
	void on_BrowseButton_clicked();
	void on_DirEdit_editTextChanged(const QString& text);
	void on_DirEdit_currentIndexChanged(const QString& text);
private:
	void SetPathToEditor(const QString& path) const;
	void MakeSelectionHint(const QString& text) const;
	void OnPathEdited(const QString& path) const;

private:
	mutable QDirModel m_directoryModel;
};


} // namespace iqtprm


#endif // !iqtprm_CFileNameParamGuiComp_included


