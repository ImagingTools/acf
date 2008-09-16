#ifndef iqtprm_CFileNameParamGuiComp_included
#define iqtprm_CFileNameParamGuiComp_included


#include "iprm/IFileNameParam.h"

#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqtprm/Generated/ui_CFileNameParamGuiComp.h"


namespace iqtprm
{


class CFileNameParamGuiComp: public iqt::TDesignerGuiObserverCompBase<
			Ui::CFileNameParamGuiComp,
			iprm::IFileNameParam>
{
	Q_OBJECT

public:
	typedef iqt::TDesignerGuiObserverCompBase<
				Ui::CFileNameParamGuiComp,
				iprm::IFileNameParam> BaseClass;

	I_BEGIN_COMPONENT(CFileNameParamGuiComp)
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

protected slots:
	void on_BrowseButton_clicked();
	void on_DirEdit_textChanged(const QString& text);
};


} // namespace iqtprm


#endif // !iqtprm_CFileNameParamGuiComp_included


