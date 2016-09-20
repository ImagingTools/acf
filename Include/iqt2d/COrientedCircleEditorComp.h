#ifndef iqt2d_COrientedCircleEditorComp_included
#define iqt2d_COrientedCircleEditorComp_included


// ACF includes
#include <i2d/COrientedCircle.h>
#include <iview/COrientedCircleShape.h>
#include <iqt2d/TCircleBasedParamsGuiComp.h>


namespace iqt2d
{


class COrientedCircleEditorComp:
			public iqt2d::TCircleBasedParamsGuiComp<
						iview::COrientedCircleShape,
						i2d::COrientedCircle>
{
	Q_OBJECT

public:
	typedef iqt2d::TCircleBasedParamsGuiComp<
				iview::COrientedCircleShape,
				i2d::COrientedCircle> BaseClass;

	I_BEGIN_COMPONENT(COrientedCircleEditorComp);
	I_END_COMPONENT;

	COrientedCircleEditorComp();

protected:
	// reimplemented (iqt2d::TCircleBasedParamsGuiComp)
	virtual bool PopulateActions(CActionAdapter& host, imod::IModel* modelPtr);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiRetranslate();

protected Q_SLOTS:
	void OnParamsChanged(double value);
	void OnActionTriggered(QAction* actionPtr);

protected:
	QAction m_reversePolarityAction;
};


} // namespace iqt2d


#endif // !iqt2d_COrientedCircleEditorComp_included


