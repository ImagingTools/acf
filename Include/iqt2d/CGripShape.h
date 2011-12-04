#ifndef iqt2d_CGripShape_included
#define iqt2d_CGripShape_included


#include "iqt2d/TShapeBase.h"


namespace iqt2d
{


class CGripShape: public TShapeBase<QGraphicsEllipseItem>
{
	Q_OBJECT

public:
	enum LabelPosition
	{
		LabelLeft,
		LabelRight,
		LabelTop,
		LabelBottom
	};


	typedef TShapeBase<QGraphicsEllipseItem> BaseClass;

	CGripShape(QGraphicsItem* parentPtr = NULL, const ISceneProvider* providerPtr = NULL);

	i2d::CVector2d GetPosition() const;
	void SetPosition(const i2d::CVector2d& position);

	/**
		Set label position relative to the grip point.
		\sa LabelPosition
	*/
	void SetLabelPosition(int labelArea);

Q_SIGNALS:
	void PositionChanged(const i2d::CVector2d& position);

protected:
	// reimplemented (TShapeBase<QGraphicsEllipseItem>)
	virtual void OnPositionChanged(const QPointF& position);
	virtual void OnSelectionChanged(bool isSelected);

private:
	QGraphicsSimpleTextItem m_labelItem;
};


} // namespace iqt2d


#endif // !iqt2d_CGripShape_included


