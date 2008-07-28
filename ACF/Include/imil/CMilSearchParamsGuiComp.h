#ifndef imil_CMilSearchParamsGuiComp_h
#define imil_CMilSearchParamsGuiComp_h


#include "iqt/TDesignerGuiObserverCompBase.h"

#include "imil/CMilSearchParams.h"
#include "imil/Generated/ui_CMilSearchParamsGuiComp.h"

#include "imod/TModelWrap.h"

#include "iqt/CBitmap.h"


namespace imil
{


class CMilSearchParamsGuiComp: public iqt::TDesignerGuiObserverCompBase<Ui::CMilSearchParamsGuiComp, imil::CMilSearchParams>
{
    Q_OBJECT
public:
	typedef  iqt::TDesignerGuiObserverCompBase<Ui::CMilSearchParamsGuiComp, imil::CMilSearchParams> BaseClass;

	I_BEGIN_COMPONENT(CMilSearchParamsGuiComp)
	I_END_COMPONENT

	CMilSearchParamsGuiComp();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor();
	virtual void UpdateModel() const;

protected slots:
	void OnParameterChanged();
	void OnFilterTypeChanged(int filterType);

protected:
	// reimplemented (iqt::CGuiObjectBase)
	virtual void OnGuiCreated();

private:
	imod::TModelWrap<iqt::CBitmap> m_modelBitmap;
	QMap<int,int> m_speedMap;
	QMap<int,int> m_accuracyMap;
	QMap<int,int> m_detailLevelMap;
	QMap<int,int> m_polarityMap;
};


} // namespace imil


#endif //!imil_CMilSearchParamsGuiComp_h


