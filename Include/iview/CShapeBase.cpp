#include "iview/CShapeBase.h"


// ACF includes
#include "iview/IViewLayer.h"


namespace iview
{


CShapeBase::CShapeBase()
{
	m_isVisible = true;
	m_userColorSchemaPtr = NULL;
	m_displayPtr = NULL;
	m_isBoundingBoxValid = false;
	m_layerType = iview::IViewLayer::LT_INACTIVE;
}


CShapeBase::CShapeBase(const CShapeBase& shape)
:	BaseClass2(shape)
{
	I_ASSERT(shape.m_displayPtr == NULL);	// Copy contructor not allowed for shapes connected to view

	m_displayPtr = NULL;

	m_userColorSchemaPtr = shape.m_userColorSchemaPtr;
	m_isVisible = shape.m_isVisible;
	m_isBoundingBoxValid = false;
	m_layerType = shape.m_layerType;
}


CShapeBase::~CShapeBase()
{
	DisconnectDisplay();

	SetVisible(false);
}


void CShapeBase::SetVisible(bool state)
{
	if (m_isVisible != state){
		m_isVisible = state;

		Invalidate(CS_CONSOLE);
	}
}


bool CShapeBase::AssignToLayer(int layerType)
{
	if (m_displayPtr != NULL){
		return false;
	}

	m_layerType = layerType;

	return true;
}


// reimplemented (iview::CShapeControl)

void CShapeBase::Invalidate(int /*changeFlags*/)
{
	InvalidateBoundingBox();

	if (m_displayPtr != NULL){
		m_displayPtr->OnChangeShape(this);
	}
}


IDisplay* CShapeBase::GetDisplayPtr() const
{
	return m_displayPtr;
}


// reimplemented (iview::IShape)

int CShapeBase::GetLayerType() const
{
	return m_layerType;
}


i2d::CRect CShapeBase::GetBoundingBox() const
{
	if (!m_isBoundingBoxValid){
		if (IsVisible()){
			m_boundingBox = CalcBoundingBox();
		}
		else{
			m_boundingBox.Reset();
		}

		m_isBoundingBoxValid = true;
	}

	return m_boundingBox;
}


const IColorSchema* CShapeBase::GetUserColorSchema() const
{
	return m_userColorSchemaPtr;
}


void CShapeBase::SetUserColorSchema(const IColorSchema* schemaPtr)
{
	m_userColorSchemaPtr = schemaPtr;
	if (m_displayPtr != NULL){
		m_displayPtr->OnChangeShape(this);
	}
}


// reimplemented (iview::IVisualizable)

bool CShapeBase::IsVisible() const
{
	return m_isVisible;
}


bool CShapeBase::IsDisplayAccepted(const IDisplay& /*display*/) const
{
	return true;
}


void CShapeBase::OnConnectDisplay(IDisplay* displayPtr)
{
	I_ASSERT(displayPtr != NULL);
	I_ASSERT(m_displayPtr == NULL);
	I_ASSERT(IsDisplayAccepted(*displayPtr));

	m_displayPtr = displayPtr;
}


void CShapeBase::OnDisconnectDisplay(IDisplay* I_IF_DEBUG(displayPtr))
{
	I_ASSERT(displayPtr != NULL);
	I_ASSERT(displayPtr == m_displayPtr);
	m_displayPtr = NULL;
}


bool CShapeBase::OnDisplayChange(int flags)
{
	if (flags & GetDisplayChangesMask()){
		InvalidateBoundingBox();
		if ((flags & CF_TRANSFORM) != 0){
			InvalidateTransforms();
		}
		return true;
	}
	return false;
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CShapeBase::IsTouched(istd::CIndex2d /*position*/) const
{
	return TS_NONE;
}


QString CShapeBase::GetShapeDescriptionAt(istd::CIndex2d /*position*/) const
{
	return "";
}


// reimplemented (imod::IObserver)

bool CShapeBase::OnAttached(imod::IModel* modelPtr)
{
	bool retVal = BaseClass3::OnAttached(modelPtr);

	Invalidate(CS_CONSOLE);

	return retVal;
}


bool CShapeBase::OnDetached(imod::IModel* modelPtr)
{
	bool retVal = BaseClass3::OnDetached(modelPtr);

	Invalidate(CS_CONSOLE);

	return retVal;
}


void CShapeBase::OnUpdate(int changeFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	Invalidate(changeFlags);
}


// protected methods

i2d::CVector2d CShapeBase::GetScreenPosition(const i2d::CVector2d& logPosition, const i2d::ITransformation2d* calibrationPtr) const
{
	I_ASSERT(m_displayPtr != NULL);

	if (calibrationPtr != NULL){
		i2d::CVector2d calibratedPosition;
		if (!calibrationPtr->GetValueAt(logPosition, calibratedPosition)){
			calibratedPosition = logPosition;
		}

		return m_displayPtr->GetTransform().GetApply(calibratedPosition);
	}
	else{
		return m_displayPtr->GetTransform().GetApply(logPosition);
	}
}


i2d::CVector2d CShapeBase::GetLogPosition(const i2d::CVector2d& screenPosition, const i2d::ITransformation2d* calibrationPtr) const
{
	I_ASSERT(m_displayPtr != NULL);

	i2d::CVector2d logPosition = m_displayPtr->GetTransform().GetInvertedApply(screenPosition);
	if (calibrationPtr != NULL){
		logPosition = calibrationPtr->GetInvValueAt(logPosition);
	}

	return logPosition;
}


int CShapeBase::GetDisplayChangesMask()
{
	return CF_TRANSFORM | CF_COLORS;
}


void CShapeBase::InvalidateBoundingBox()
{
	InvalidateTransforms();
	m_isBoundingBoxValid = false;
}


bool CShapeBase::IsDisplayConnected() const
{
	I_ASSERT((m_displayPtr == NULL) || IsDisplayAccepted(*m_displayPtr));

	return (m_displayPtr != NULL);
}


const IColorSchema& CShapeBase::GetColorSchema() const
{
	if (m_userColorSchemaPtr != NULL){
		return *m_userColorSchemaPtr;
	}
	else{
		I_ASSERT(IsDisplayConnected());
	
		return m_displayPtr->GetColorSchema();
	}
}


i2d::CRect CShapeBase::GetClientRect() const
{
	I_ASSERT(IsDisplayConnected());

	return m_displayPtr->GetClientRect();
}


bool CShapeBase::IsBoundingBoxValid() const
{
	return m_isBoundingBoxValid;
}


// protected methods

void CShapeBase::DisconnectDisplay()
{
	if (m_displayPtr != NULL){
		m_displayPtr->DisconnectShape(this);
		m_displayPtr = NULL;
	}
}


// reimplemented (iview::CShapeControl)

const iview::CScreenTransform& CShapeBase::GetViewToScreenTransform() const
{
	I_ASSERT(IsDisplayConnected());

	return m_displayPtr->GetTransform();
}


} // namespace iview


