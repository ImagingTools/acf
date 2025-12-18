#ifndef iview_CPolypointShape_included
#define iview_CPolypointShape_included


#include <iview/CShapeBase.h>


namespace iview
{


class CPolypointShape: public CShapeBase
{
public:
	typedef CShapeBase BaseClass;

	CPolypointShape();

	bool IsSmallTickersMode() const;
	void SetSmallTickersMode(bool state = true);

	// reimplemented (iview::IShape)
	virtual void Draw(QPainter& drawContext) const override;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;

protected:
	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;

	bool m_isSmallTickersMode;
};


} // namespace iview


#endif // !iview_CPolypointShape_included


