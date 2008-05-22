#ifndef iqt_CFileAcquisitionParamsGuiComp_included
#define iqt_CFileAcquisitionParamsGuiComp_included


#include "icam/IFileBitmapAcquisitionParams.h"

#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqt/Generated/ui_CFileAcquisitionParamsGuiComp.h"


namespace iqt
{


class CFileAcquisitionParamsGuiComp: public TDesignerGuiObserverCompBase<
			Ui::CFileAcquisitionParamsGuiComp,
			icam::IFileBitmapAcquisitionParams>
{
	Q_OBJECT

public:
	typedef TDesignerGuiObserverCompBase<
				Ui::CFileAcquisitionParamsGuiComp,
				icam::IFileBitmapAcquisitionParams> BaseClass;

	I_BEGIN_COMPONENT(CFileAcquisitionParamsGuiComp)
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

protected slots:
	void on_BrowseButton_clicked();
	void on_DirEdit_textChanged(const QString& text);
};


} // namespace iqt


#endif // !iqt_CFileAcquisitionParamsGuiComp_included


