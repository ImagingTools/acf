#ifndef iview_CPinShape_included
#define iview_CPinShape_included


#include <i2d/CVector2d.h>
#include <i2d/CPosition2d.h>

#include <iview/CInteractiveShapeBase.h>


namespace iview
{


class CPinShape: public CInteractiveShapeBase
{
public:
	typedef CInteractiveShapeBase BaseClass;

	CPinShape();

	// reimplemented (iview::IInteractiveShape)
	virtual TouchState IsTouched(istd::CIndex2d position) const override;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const override;

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag) override;
	virtual bool OnMouseMove(istd::CIndex2d position) override;

protected:
	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void BeginLogDrag(const i2d::CVector2d& reference) override;
	virtual void SetLogDragPosition(const i2d::CVector2d& position) override;

	i2d::CVector2d m_referencePosition;
};


} // namespace iview


#endif // !iview_CPinShape_included


