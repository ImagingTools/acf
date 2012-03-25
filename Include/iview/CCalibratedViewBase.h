#ifndef iview_CCalibratedViewBase_included
#define iview_CCalibratedViewBase_included


// Qt includes
#include <QtGui/QPainter>
#include <QtGui/QWidget>


// ACF includes
#include "istd/TOptDelPtr.h"

#include "i2d/CRect.h"

#include "i2d/ITransformation2d.h"
#include "iview/IVisualCalibrationInfo.h"
#include "iview/CNoneCalibration.h"
#include "iview/CViewBase.h"
#include "iview/CDrawBuffer.h"
#include "iview/CColorShema.h"


namespace iview
{


/**
	Abstract base class for all calibrated view.
	Calibrated view implements i2d::ICalibrationProvider interface,
	and can automatically show calibration grid if supported.
*/
class CCalibratedViewBase:
			public QWidget,
			public iview::CViewBase,
			virtual public IVisualCalibrationInfo
{
public:
	typedef iview::CViewBase BaseClass;
	typedef QWidget BaseClass2;

	enum LayerType2
	{
		LT_CALIBRATION = 0x100
	};

	CCalibratedViewBase(QWidget* parentWidgetPtr = NULL);

	/**
		Set calibration object for this view.
	*/
	void SetCalibrationPtr(const i2d::ITransformation2d* calibrationPtr);

	/**
		Set grid visibility state.
	*/
	virtual void SetGridVisible(bool state = true);

	/**
		Connect visualisation shape for calibration object.
	*/
	virtual void ConnectCalibrationShape(iview::IShape* shapePtr);

	/**
		Set minimal grid size.
		It determines minimal distance between grid lines.
	*/
	void SetMinGridDistance(double gridDistance);

	/**
		Set if grid should be shown in milimeter.
	*/
	void SetGridInMm(bool state = true);

	const iview::CSingleLayer& GetCalibrationLayer() const;

	/**
		Set default color shema object.
		\param	colorShemaPtr	pointer to color shema object.
		\param	releaseFlag		if its true, object will be automatically removed.
	*/
	void SetDefaultColorShema(const IColorShema* colorShemaPtr, bool releaseFlag = false);

	bool IsBackgroundBufferActive() const;
	void SetBackgroundBufferActive(bool state = true);
	bool IsDoubleBufferActive() const;
	void SetDoubleBufferActive(bool state = true);

	const iview::CDrawBuffer& GetBackgroundBuffer() const;
	iview::CDrawBuffer& GetBackgroundBuffer();
	const iview::CDrawBuffer& GetDoubleBuffer() const;
	iview::CDrawBuffer& GetDoubleBuffer();

	/**
		Called if found that display area was resized.
	*/
	virtual void OnResize();
	
	/**
		Check if size size was changed and invalidation and OnResize() should be done.
	*/
	void CheckResize();

	// reimplemented (iview::CViewBase)
	virtual const iview::IColorShema& GetDefaultColorShema() const;
	virtual void UpdateAllShapes(int changeFlag);
	virtual void InsertDefaultLayers();

	// reimplemented (iview::IShapeView)
	virtual int InsertLayer(iview::ILayer* layerPtr, int index = -1, int layerType = LT_NONE);
	virtual void RemoveLayer(int index);

	// reimplemented (iview::IVisualCalibrationInfo)
	virtual bool IsGridVisible() const;
	virtual double GetMinGridDistance() const;
	virtual bool IsGridInMm() const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ITransformation2d* GetCalibration() const;

	using BaseClass::InvalidateBackground;

protected:
	virtual void DrawBuffers(QPaintDevice& nativeContext, const i2d::CRect& clipRect);
	virtual void DrawToContext(QPaintDevice& nativeContext, QPainter& context, const i2d::CRect& invalidatedBox);

private:
	mutable istd::TOptDelPtr<const IColorShema> m_defaultColorShemaPtr;

	iview::CDrawBuffer m_backgroundBuffer;
	iview::CDrawBuffer m_doubleBuffer;

	bool m_isBackgroundBufferActive;
	bool m_isDoubleBufferActive;

	i2d::CRect m_lastClientArea;

	const i2d::ITransformation2d* m_calibrationPtr;
	bool m_isGridVisible;
	bool m_isGridInMm;
	double m_minGridDistance;

	int m_calibrationLayerIndex;

	// default layers
	iview::CSingleLayer m_calibrationLayer;
};


// inline methods

inline 	void CCalibratedViewBase::SetCalibrationPtr(const i2d::ITransformation2d* calibrationPtr)
{
	if (calibrationPtr != m_calibrationPtr){
		if (calibrationPtr != NULL){
			m_calibrationPtr = calibrationPtr;
		}
		else{
			m_calibrationPtr = &CNoneCalibration::GetInstance();
		}

		InvalidateBackground();
	}

	UpdateAllShapes(iview::IShape::CF_CALIB);
}


inline void CCalibratedViewBase::SetGridVisible(bool state)
{
	if (m_isGridVisible != state){
		m_isGridVisible = state;
		InvalidateBackground();
	}
}


inline void CCalibratedViewBase::SetMinGridDistance(double gridDistance)
{
	if (m_minGridDistance != gridDistance){
		m_minGridDistance = gridDistance;
		if (m_isGridVisible){
			SetBackgroundBufferValid(false);
		}
	}
}


inline void CCalibratedViewBase::SetGridInMm(bool state)
{
	if (m_isGridInMm != state){
		m_isGridInMm = state;
		SetBackgroundBufferValid(false);
	}
}


inline const iview::CSingleLayer& CCalibratedViewBase::GetCalibrationLayer() const
{
	return m_calibrationLayer;
}


inline bool CCalibratedViewBase::IsGridVisible() const
{
	return m_isGridVisible;
}


inline double CCalibratedViewBase::GetMinGridDistance() const
{
	return m_minGridDistance;
}


inline bool CCalibratedViewBase::IsGridInMm() const
{
	return m_isGridInMm;
}


// reimplemented (i2d::ICalibrationProvider)

inline const i2d::ITransformation2d* CCalibratedViewBase::GetCalibration() const
{
	return m_calibrationPtr;
}



} // namespace iview



#endif // !iview_CCalibratedViewBase_included




