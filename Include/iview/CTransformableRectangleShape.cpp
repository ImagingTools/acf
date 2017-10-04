#include <iview/CTransformableRectangleShape.h>


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <i2d/CParallelogram.h>
#include <imod/IModel.h>
#include <iqt/iqt.h>


namespace iview
{


// reimplemented (imod::IObserver)

bool CTransformableRectangleShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(dynamic_cast<i2d::CParallelogram*>(modelPtr) != NULL);

	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


// reimplemented (iview::IMouseActionObserver)

bool CTransformableRectangleShape::OnMouseMove(istd::CIndex2d position)
{
	imod::IModel* modelPtr = GetObservedModel();
	i2d::CParallelogram* parallelogramPtr = dynamic_cast<i2d::CParallelogram*>(modelPtr);
	if (parallelogramPtr != NULL){
		const i2d::CAffine2d& parallTransform = parallelogramPtr->GetTransform();

		i2d::CVector2d cp = GetLogPosition(position);

		parallelogramPtr->SetTransform(CalcMoveTransform(cp, parallTransform).GetApply(parallTransform));

		UpdateModelChanges();

		return true;
	}

	return true;
}


// protected methods

void CTransformableRectangleShape::EnsureValidNodes() const
{
	if (!AreNodesValid()){
		const imod::IModel* modelPtr = GetObservedModel();
		const i2d::CParallelogram* parallelogramPtr = dynamic_cast<const i2d::CParallelogram*>(modelPtr);
		if (parallelogramPtr != NULL){
			const i2d::CAffine2d& parallTransform = parallelogramPtr->GetTransform();

			CalcNodes(parallTransform);
		}
		else{
			ResetNodes();
		}
	}
}


bool CTransformableRectangleShape::IsCurveTouched(istd::CIndex2d position) const
{
	const i2d::CParallelogram* parallelogramPtr = dynamic_cast<const i2d::CParallelogram*>(GetObservedModel());
	if (parallelogramPtr != NULL){
		return IsParallTouched(parallelogramPtr->GetTransform(), position);
	}

	return false;
}


} // namespace iview

