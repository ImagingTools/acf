#include <iview/CInteractiveShapeBase.h>


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include <istd/IChangeable.h>
#include <iqt/iqt.h>

#include <iview/ISelectable.h>
#include <iview/IViewLayer.h>


namespace iview
{


const istd::IChangeable::ChangeSet s_moveObjectChangeSet(IDisplay::CS_CONSOLE, i2d::IObject2d::CF_OBJECT_POSITION, QObject::tr("Move object"));


CInteractiveShapeBase::CInteractiveShapeBase()
:	m_isSelected(false),
	m_isEditablePosition(true)
{
	AssignToLayer(IViewLayer::LT_ACTIVE);
}


CInteractiveShapeBase::CInteractiveShapeBase(const CInteractiveShapeBase& shape)
:	BaseClass(shape),
	m_isSelected(false),
	m_isEditablePosition(true),
	m_dragNotifierPtr()
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

		Invalidate();
	
		ISelectable* observerPtr = dynamic_cast<ISelectable*>(GetDisplayPtr());
		if (observerPtr != NULL){
			observerPtr->OnShapeSelected(*this, selectFlag);
		}
	}
}


// reimplemented (iview::IMouseActionObserver)

bool CInteractiveShapeBase::OnMouseButton(istd::CIndex2d /*position*/, Qt::MouseButton /*buttonType*/, bool /*downFlag*/)
{
	return false;
}


bool CInteractiveShapeBase::OnMouseMove(istd::CIndex2d)
{
	return false;
}


// reimplemented (imod::IObserver)

bool CInteractiveShapeBase::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	if (BaseClass::OnModelAttached(modelPtr, changeMask)){
		Invalidate();

		return true;
	}

	return false;
}


bool CInteractiveShapeBase::OnModelDetached(imod::IModel* modelPtr)
{
	bool retVal = BaseClass::OnModelDetached(modelPtr);
	if (retVal){
		Invalidate();
	}

	return retVal;
}


// reimplemented (iview::IDraggable)

bool CInteractiveShapeBase::IsDraggable() const
{
	return m_isEditablePosition;
}


void CInteractiveShapeBase::BeginDrag(const istd::CIndex2d& position)
{
	BeginLogDrag(GetLogPosition(position));
}


void CInteractiveShapeBase::SetDragPosition(const istd::CIndex2d& position)
{
	if (!m_dragNotifierPtr.IsValid()){
		istd::IChangeable* objectPtr = dynamic_cast<istd::IChangeable*>(GetObservedModel());

		m_dragNotifierPtr.SetPtr(new istd::CChangeGroup(objectPtr, &s_moveObjectChangeSet));
	}

	SetLogDragPosition(GetLogPosition(position));

	Invalidate();
}


void CInteractiveShapeBase::EndDrag()
{
	m_dragNotifierPtr.Reset();
}


// protected methods

void CInteractiveShapeBase::BeginTickerDrag()
{
	istd::IChangeable* objectPtr = dynamic_cast<istd::IChangeable*>(GetObservedModel());
	m_dragNotifierPtr.SetPtr(new istd::CChangeGroup(objectPtr, &s_moveObjectChangeSet));

	ISelectable* controllerPtr = dynamic_cast<ISelectable*>(GetDisplayPtr());
	if (controllerPtr != NULL){
		controllerPtr->OnShapeFocused(this);
	}
}


void CInteractiveShapeBase::EndTickerDrag()
{
	UpdateModelChanges();

	ISelectable* controllerPtr = dynamic_cast<ISelectable*>(GetDisplayPtr());
	if (controllerPtr != NULL){
		controllerPtr->OnShapeDefocused(this);
	}

	m_dragNotifierPtr.Reset();
}


void CInteractiveShapeBase::UpdateModelChanges()
{
	Invalidate();
}


void CInteractiveShapeBase::DrawText(QPainter& drawContext, istd::CIndex2d point, const QString& text) const
{
	QFontMetrics metrics(drawContext.font());

	QRect textRect(iqt::GetQPoint(point), metrics.size(0, text));

	drawContext.drawText(textRect, Qt::AlignLeft | Qt::AlignBottom, text);
}


} // namespace iview


