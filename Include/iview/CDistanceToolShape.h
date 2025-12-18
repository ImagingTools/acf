#pragma once


// ACF includes
#include <iview/CShapeBase.h>
#include <iview/IInteractiveShape.h>


namespace iview
{


class CDistanceToolShape:
			public CShapeBase,
			virtual public iview::IInteractiveShape
{
public:
	typedef CShapeBase BaseClass;

	CDistanceToolShape();

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag) override;
	virtual bool OnMouseMove(istd::CIndex2d position) override;

	// reimplemented (iview::IInteractiveShape)
	virtual bool IsSelected() const override;
	virtual void SetSelected(bool selectFlag = true) override;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const override;

protected:
	const i2d::ICalibration2d* GetCalibration() const;

	virtual void DrawLabel(QPainter& drawContext) const;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const override;

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;

private:
	enum MeasureState
	{
		MS_NONE = 0,
		MS_START,
		MS_DONE
	};

	bool IsDistanceMeasureToolActive() const;
	void BeginDrag();
	void EndDrag();

private:
	istd::CIndex2d m_screenPos;
	MeasureState m_measureState;
	int m_dragNodeIndex;
	bool m_isSelected;
};


} // namespace iview

