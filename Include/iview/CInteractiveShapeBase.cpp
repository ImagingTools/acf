#include "iview/CInteractiveShapeBase.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "iqt/iqt.h"

#include "iview/ISelectable.h"
#include "iview/IViewLayer.h"


namespace iview
{


CInteractiveShapeBase::CInteractiveShapeBase()
{
	m_isSelected = false;
	m_isEditablePosition = true;

	AssignToLayer(IViewLayer::LT_ACTIVE);
}


CInteractiveShapeBase::CInteractiveShapeBase(const CInteractiveShapeBase& shape)
	:BaseClass(shape)
{
}


CInteractiveShapeBase::~CInteractiveShapeBase()
{
	DisconnectDisplay();
}


// reimplemented (iview::IInteractiveShape)

bool CInteractiveShapeBase::IsSelected() const
{
	return m_isSelected;
}


void CInteractiveShapeBase::SetSelected(bool selectFlag)
{
	if (m_isSelected != selectFlag){
		m_isSelected = selectFlag;

		InvalidateBoundingBox();

		ISelectable* observerPtr = dynamic_cast<ISelectable*>(GetDisplayPtr());
		if (observerPtr != NULL){
			observerPtr->OnShapeSelected(*this, selectFlag);
		}
	}
}


// reimplemented (iview::IMouseActionObserver)

bool CInteractiveShapeBase::OnMouseButton(istd::CIndex2d /*position*/, Qt::MouseButton buttonType, bool downFlag)
{
	i2d::CObject2dBase* objectPtr = dynamic_cast<i2d::CObject2dBase*>(GetModelPtr());
	if (objectPtr != NULL && buttonType == Qt::LeftButton){
		if (downFlag){
			objectPtr->StartTransform();
		} 
		else {
			objectPtr->FinishTransform();
		}
	}

	return false;
}


bool CInteractiveShapeBase::OnMouseMove(istd::CIndex2d)
{
	return false;
}


// reimplemented (imod::IObserver)

bool CInteractiveShapeBase::OnAttached(imod::IModel* modelPtr)
{
	if (BaseClass::OnAttached(modelPtr)){
		Invalidate(CS_CONSOLE);

		return true;
	}

	return false;
}


bool CInteractiveShapeBase::OnDetached(imod::IModel* modelPtr)
{
	Invalidate(CS_CONSOLE);

	return BaseClass::OnDetached(modelPtr);
}


// protected methods

void CInteractiveShapeBase::BeginModelChanges()
{
	ISelectable* controllerPtr = dynamic_cast<ISelectable*>(GetDisplayPtr());
	if (controllerPtr != NULL){
		controllerPtr->OnShapeFocused(this);
	}
}


void CInteractiveShapeBase::EndModelChanges()
{
	UpdateModelChanges();

	ISelectable* controllerPtr = dynamic_cast<ISelectable*>(GetDisplayPtr());
	if (controllerPtr != NULL){
		controllerPtr->OnShapeDefocused(this);
	}
}


void CInteractiveShapeBase::UpdateModelChanges()
{
	i2d::CObject2dBase* objectPtr = dynamic_cast<i2d::CObject2dBase*>(GetModelPtr());
	int flags = istd::IChangeable::CF_MODEL;
	if (!objectPtr->IsUndoAllowed()){
		flags |= istd::IChangeable::CF_NO_UNDO;
	}

	istd::CChangeNotifier changePtr(objectPtr, flags);
}


void CInteractiveShapeBase::DrawText(QPainter& drawContext, istd::CIndex2d point, const QString& text) const
{
	QFontMetrics metrics(drawContext.font());

	QRect textRect(iqt::GetQPoint(point), metrics.size(0, text));

	drawContext.drawText(textRect, Qt::AlignLeft | Qt::AlignBottom, text);
}


} // namespace iview


