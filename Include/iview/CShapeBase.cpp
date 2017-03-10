#include <iview/CShapeBase.h>


// ACF includes
#include <istd/IInformationProvider.h>
#include <iview/IViewLayer.h>
#include <iview/IColorSchema.h>


namespace iview
{


CShapeBase::CShapeBase()
:	m_displayPtr(NULL),
	m_calibrationObserver(this)
{
	m_isVisible = true;
	m_userColorSchemaPtr = NULL;
	m_isBoundingBoxValid = false;
	m_layerType = iview::IViewLayer::LT_INACTIVE;
	m_shapeTransformMode = STM_SHAPE;
}


CShapeBase::CShapeBase(const CShapeBase& shape)
:	imod::CSingleModelObserverBase(),
	m_displayPtr(NULL),
	m_calibrationObserver(this)
{
	Q_ASSERT(shape.m_displayPtr == NULL);	// Copy contructor not allowed for shapes connected to view

	m_displayPtr = NULL;

	m_userColorSchemaPtr = shape.m_userColorSchemaPtr;
	m_isVisible = shape.m_isVisible;
	m_isBoundingBoxValid = false;
	m_layerType = shape.m_layerType;
	m_shapeTransformMode = shape.m_shapeTransformMode;
}


CShapeBase::~CShapeBase()
{
	DisconnectDisplay();

	SetVisible(false);
}


bool CShapeBase::AssignToLayer(int layerType)
{
	if (m_displayPtr != NULL){
		return false;
	}

	m_layerType = layerType;

	return true;
}


void CShapeBase::Invalidate()
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


CShapeBase::ShapeTransformMode CShapeBase::GetTransformMode() const
{
	return m_shapeTransformMode;
}


void CShapeBase::SetTransformMode(ShapeTransformMode mode)
{
	if (m_shapeTransformMode != mode){
		m_shapeTransformMode = mode;

		Invalidate();
	}
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


void CShapeBase::SetVisible(bool state)
{
	if (m_isVisible != state){
		m_isVisible = state;

		Invalidate();
	}
}


const IColorSchema* CShapeBase::GetUserColorSchema() const
{
	return m_userColorSchemaPtr;
}


void CShapeBase::SetUserColorSchema(const IColorSchema* schemaPtr)
{
	if (schemaPtr != m_userColorSchemaPtr){
		m_userColorSchemaPtr = schemaPtr;

		Invalidate();
	}
}


void CShapeBase::SetDefaultDescription(const QString& description)
{
	m_defaultDescription = description;
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
	Q_ASSERT(displayPtr != NULL);
	Q_ASSERT(m_displayPtr == NULL);
	Q_ASSERT(IsDisplayAccepted(*displayPtr));

	m_displayPtr = displayPtr;
}


void CShapeBase::OnDisconnectDisplay(IDisplay* I_IF_DEBUG(displayPtr))
{
	I_IF_DEBUG(Q_ASSERT(displayPtr != NULL));
	I_IF_DEBUG(Q_ASSERT(displayPtr == m_displayPtr));
	m_displayPtr = NULL;
}


bool CShapeBase::OnDisplayChange(const istd::IChangeable::ChangeSet& changeSet)
{
	if (IsDisplayChangeImportant(changeSet)){
		InvalidateBoundingBox();

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
	const istd::IInformationProvider* infoPtr = dynamic_cast<const istd::IInformationProvider*>(GetObservedModel());
	if (infoPtr != NULL){
		return infoPtr->GetInformationDescription();
	}

	return m_defaultDescription;
}


// reimplemented (imod::IObserver)

bool CShapeBase::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	bool retVal = BaseClass::OnModelAttached(modelPtr, changeMask);

	const i2d::IObject2d* object2dPtr = dynamic_cast<const i2d::IObject2d*>(modelPtr);
	if (object2dPtr != NULL){
		i2d::ICalibration2d* calibrationPtr = const_cast<i2d::ICalibration2d*>(object2dPtr->GetCalibration());
		if (calibrationPtr != NULL){
			m_calibrationObserver.AttachOrSetObject(calibrationPtr);
		}
	}

	Invalidate();

	return retVal;
}


bool CShapeBase::OnModelDetached(imod::IModel* modelPtr)
{
	m_calibrationObserver.EnsureModelDetached();

	bool retVal = BaseClass::OnModelDetached(modelPtr);

	Invalidate();

	return retVal;
}


void CShapeBase::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	const i2d::IObject2d* object2dPtr = dynamic_cast<const i2d::IObject2d*>(GetObservedModel());
	if (object2dPtr != NULL){
		i2d::ICalibration2d* calibrationPtr = const_cast<i2d::ICalibration2d*>(object2dPtr->GetCalibration());
		if ((calibrationPtr != NULL) && (calibrationPtr != m_calibrationObserver.GetObservedObject())){
			m_calibrationObserver.AttachOrSetObject(calibrationPtr);
		}
	}

	Invalidate();
}


// protected methods

i2d::CVector2d CShapeBase::GetScreenPosition(const i2d::CVector2d& logPosition) const
{
	const iview::CScreenTransform& transform = GetViewToScreenTransform();

	switch (m_shapeTransformMode){
	case STM_SHAPE:
		{
			const i2d::ITransformation2d* calibrationPtr = m_calibrationObserver.GetObservedObject();

			if (calibrationPtr != NULL){
				i2d::CVector2d calibratedPosition;
				if (!calibrationPtr->GetValueAt(logPosition, calibratedPosition)){
					calibratedPosition = logPosition;
				}

				return transform.GetApply(calibratedPosition);
			}
			else{
				return transform.GetApply(logPosition);
			}
		}

	default:
		return transform.GetApply(logPosition);
	}
}


i2d::CVector2d CShapeBase::GetLogPosition(const i2d::CVector2d& screenPosition) const
{
	const iview::CScreenTransform& transform = GetViewToScreenTransform();

	i2d::CVector2d logPosition = transform.GetInvertedApply(screenPosition);

	switch (m_shapeTransformMode){
	case STM_SHAPE:
		{
			const i2d::ITransformation2d* calibrationPtr = m_calibrationObserver.GetObservedObject();

			if (calibrationPtr != NULL){
				i2d::CVector2d calibratedPosition;
				if (calibrationPtr->GetInvValueAt(logPosition, calibratedPosition)){
					return calibratedPosition;
				}
			}
		}
		break;

	default:
		break;
	}

	return logPosition;
}


double CShapeBase::GetLocalLineWidth(const i2d::CVector2d& screenPosition) const
{
	const iview::CScreenTransform& transform = GetViewToScreenTransform();

	const IColorSchema& schema = GetColorSchema();
	double logWidth = schema.GetLogicalLineWidth() / transform.GetDeformMatrix().GetApproxScale();

	switch (m_shapeTransformMode){
	case STM_SHAPE:
		{
			const i2d::ITransformation2d* calibrationPtr = m_calibrationObserver.GetObservedObject();

			if (calibrationPtr != NULL){
				i2d::CAffine2d transform;
				if (calibrationPtr->GetLocalInvTransform(screenPosition, transform)){
					logWidth *= transform.GetDeformMatrix().GetApproxScale();
				}
				else{
					return 0;
				}
			}
		}
		break;

	default:
		break;
	}

	return logWidth;
}


bool CShapeBase::IsDisplayChangeImportant(const istd::IChangeable::ChangeSet& changeSet)
{
	return changeSet.Contains(IDisplay::CF_TRANSFORM) || changeSet.Contains(IDisplay::CF_COLORS);
}


void CShapeBase::InvalidateBoundingBox()
{
	m_isBoundingBoxValid = false;
}


bool CShapeBase::IsDisplayConnected() const
{
	Q_ASSERT((m_displayPtr == NULL) || IsDisplayAccepted(*m_displayPtr));

	return (m_displayPtr != NULL);
}


const IColorSchema& CShapeBase::GetColorSchema() const
{
	if (m_userColorSchemaPtr != NULL){
		return *m_userColorSchemaPtr;
	}
	else{
		Q_ASSERT(IsDisplayConnected());
	
		return m_displayPtr->GetColorSchema();
	}
}


i2d::CRect CShapeBase::GetClientRect() const
{
	Q_ASSERT(IsDisplayConnected());

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


const iview::CScreenTransform& CShapeBase::GetViewToScreenTransform() const
{
	Q_ASSERT(IsDisplayConnected());

	return m_displayPtr->GetTransform();
}


// public methods of embedded class CalibrationObserver

CShapeBase::CalibrationObserver::CalibrationObserver(CShapeBase* parentPtr)
:	m_parentPtr(parentPtr)
{
	Q_ASSERT(m_parentPtr != NULL);
}


// reimplemented (imod::CSingleModelObserverBase)

void CShapeBase::CalibrationObserver::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(m_parentPtr != NULL);

	m_parentPtr->Invalidate();
}


} // namespace iview


