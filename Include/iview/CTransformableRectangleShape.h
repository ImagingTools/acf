#ifndef iview_CTransformableRectangleShape_included
#define iview_CTransformableRectangleShape_included


// ACF includes
#include <iview/CRectControlledShapeBase.h>


namespace iview
{


class CTransformableRectangleShape: public CRectControlledShapeBase
{
public:
	typedef CRectControlledShapeBase BaseClass;
	typedef CRectControlledShapeBase ShapeBaseClass;
	
	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseMove(istd::CIndex2d position);

protected:
	// reimplemented (iview::CRectControlledShapeBase)
	virtual void EnsureValidNodes() const;
	virtual bool IsCurveTouched(istd::CIndex2d position) const;
};


} // namespace iview


#endif // !iview_CTransformableRectangleShape_included



