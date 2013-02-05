#ifndef iview_CCalibrationShapeBase_included
#define iview_CCalibrationShapeBase_included


#include "iview/CInteractiveShapeBase.h"
#include "i2d/ICalibration2d.h"


namespace iview
{


// TODO: Redesign it to ACF transformation concept.

/**
	Base class for all shapes using i2d::ICalibration2d to transform between logical and physical positions.
*/
class CCalibrationShapeBase: public CInteractiveShapeBase
{
public:
	typedef iview::CInteractiveShapeBase BaseClass;

	CCalibrationShapeBase();

	// reimplemented (iview::IInteractiveShape)
	virtual void BeginDrag(const i2d::CVector2d& position);
	virtual void SetDragPosition(const i2d::CVector2d& position);

	// reimplemented (iview::IVisualizable)
	virtual bool IsDisplayAccepted(const iview::IDisplay& display) const;
	virtual void OnConnectDisplay(iview::IDisplay* displayPtr);
	virtual void OnDisconnectDisplay(iview::IDisplay* displayPtr);

protected:
	static const i2d::ICalibration2d* FindCalibration(const iview::IDisplay& display);

	/**
		Get access to calibration object.
	*/
	virtual const i2d::ICalibration2d& GetIsomorphCalib() const;

	// reimplemented (iview::CShapeBase)
	virtual int GetDisplayChangesMask();

private:
	const i2d::ICalibration2d* m_calibPtr;
};


// protected inline methods

inline const i2d::ICalibration2d& CCalibrationShapeBase::GetIsomorphCalib() const
{
	Q_ASSERT(IsDisplayConnected());
	Q_ASSERT(m_calibPtr != NULL);

	return *m_calibPtr;
}


// reimplemented (iview::CShapeBase)

inline int CCalibrationShapeBase::GetDisplayChangesMask()
{
	return BaseClass::GetDisplayChangesMask() | CF_CALIB;
}


} // namespace iview


#endif // !iview_CCalibrationShapeBase_included


