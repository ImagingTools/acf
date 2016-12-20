#ifndef iqt2d_CPosition2dParamsGuiComp_included
#define iqt2d_CPosition2dParamsGuiComp_included


// ACF includes
#include <i2d/CPosition2d.h>

#include <iview/CPinShape.h>

#include <iqt2d/TShapeParamsGuiCompBase.h>
#include <GeneratedFiles/iqt2d/ui_CPosition2dParamsGuiComp.h>


namespace iqt2d
{


class CPosition2dParamsGuiComp:
			public iqt2d::TShapeParamsGuiCompBase<
						Ui::CPosition2dParamsGuiComp,
						iview::CPinShape,
						i2d::CPosition2d>
{
	Q_OBJECT

public:
	typedef iqt2d::TShapeParamsGuiCompBase<
				Ui::CPosition2dParamsGuiComp,
				iview::CPinShape,
				i2d::CPosition2d> BaseClass;

	I_BEGIN_COMPONENT(CPosition2dParamsGuiComp);
		I_ASSIGN(m_altrnativeStrAttrPtr, "AlternativeLabel", "String that repleace default label", false, "Position");
		I_ASSIGN(m_xLabelStrAttrPtr, "XLabel", "X value label", false, "X:");
		I_ASSIGN(m_yLabelStrAttrPtr, "YLabel", "Y value label", false, "Y:");
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiRetranslate();

protected Q_SLOTS:
	void OnParamsChanged(double value);
	void OnActionTriggered(QAction* actionPtr);

private:
	I_TEXTATTR(m_altrnativeStrAttrPtr);
	I_TEXTATTR(m_xLabelStrAttrPtr);
	I_TEXTATTR(m_yLabelStrAttrPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CPosition2dParamsGuiComp_included


