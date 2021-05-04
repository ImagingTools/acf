#ifndef iview_CCalibratedViewBase_included
#define iview_CCalibratedViewBase_included


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <istd/TOptDelPtr.h>
#include <i2d/CRect.h>
#include <i2d/ICalibration2d.h>
#include <iview/IVisualCalibrationInfo.h>
#include <iview/IToolsLayerInfo.h>
#include <iview/CViewBase.h>
#include <iview/CDrawBuffer.h>
#include <iview/CColorSchema.h>
#include <iview/CToolsViewLayer.h>


namespace iview
{


/**
	Abstract base class for all calibrated view.
	Calibrated view implements i2d::ICalibrationProvider interface,
	and can automatically show calibration grid if supported.
*/
class CCalibratedViewBase:
			public CViewBase,
			virtual public i2d::ICalibrationProvider,
			virtual public IVisualCalibrationInfo,
			virtual public IToolsLayerInfo
{
public:
	typedef CViewBase BaseClass;

	CCalibratedViewBase();

	/**
		Set calibration using to display logical coordinates.
	*/
	void SetDisplayCalibration(const i2d::ICalibration2d* calibrationPtr);

	/**
		Set grid visibility state.
	*/
	virtual void SetGridVisible(bool state = true);

	/**
		Connect visualisation shape for calibration object.
	*/
	virtual void ConnectCalibrationShape(iview::IShape* shapePtr);

	/**
		Set distance measure tool active.
	*/
	virtual void SetDistanceMeasureToolActive(bool state = true);

	/**
		Set point measure tool active.
	*/
	virtual void SetPointMeasureToolActive(bool state = true);


	/**
		Connect visualisation shape for ruler object.
	*/
	virtual void ConnectToolShape(iview::IShape* shapePtr);

	/**
		Set minimal grid size.
		It determines minimal distance between grid lines.
	*/
	void SetMinGridDistance(double gridDistance);

	/**
		Set if grid should be shown in milimeter.
	*/
	void SetGridInMm(bool state = true);

	/**
		Get default calibration layer.
	*/
	const iview::IViewLayer& GetCalibrationLayer() const;

	/**
		Get default tools layer.
	*/
	const iview::IViewLayer& GetToolsLayer() const;

	/**
		Set default color schema object.
		\param	colorSchemaPtr	pointer to color schema object.
		\param	releaseFlag		if its true, object will be automatically removed.
	*/
	void SetDefaultColorSchema(const IColorSchema* colorSchemaPtr, bool releaseFlag = false);

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
	virtual const iview::IColorSchema& GetDefaultColorSchema() const;
	virtual void UpdateAllShapes(const istd::IChangeable::ChangeSet& changeSet);
	virtual void InsertDefaultLayers();

	// reimplemented (iview::IShapeView)
	virtual int InsertLayer(iview::IViewLayer* layerPtr, int index = -1, int layerType = IViewLayer::LT_NONE);
	virtual void RemoveLayer(int index);

	// reimplemented (iview::IVisualCalibrationInfo)
	virtual bool IsGridVisible() const;
	virtual double GetMinGridDistance() const;
	virtual bool IsGridInMm() const;

	// reimplemented (iview::IToolsLayerInfo)
	virtual bool IsDistanceMeasureToolActive() const;
	virtual bool IsPointMeasureToolActive() const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

	using BaseClass::InvalidateBackground;

protected:
	virtual void DrawBuffers(QPaintDevice& nativeContext, const i2d::CRect& clipRect);
	virtual void DrawToContext(QPaintDevice& nativeContext, QPainter& context, const i2d::CRect& invalidatedBox);

private:
	mutable istd::TOptDelPtr<const IColorSchema> m_defaultColorSchemaPtr;

	iview::CDrawBuffer m_backgroundBuffer;
	iview::CDrawBuffer m_doubleBuffer;

	bool m_isBackgroundBufferActive;
	bool m_isDoubleBufferActive;

	i2d::CRect m_lastClientArea;

	const i2d::ICalibration2d* m_calibrationPtr;
	bool m_isGridVisible;
	bool m_isGridInMm;
	double m_minGridDistance;
	bool m_isDistanceMeasureToolActive;
	bool m_isPointMeasureToolActive;

	int m_calibrationLayerIndex;
	int m_toolsLayerIndex;

	// default layers
	iview::CViewLayer m_calibrationLayer;
	iview::CToolsViewLayer m_toolsLayer;
};


// inline methods

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


inline const iview::IViewLayer& CCalibratedViewBase::GetCalibrationLayer() const
{
	return m_calibrationLayer;
}


inline const iview::IViewLayer& CCalibratedViewBase::GetToolsLayer() const
{
	return m_toolsLayer;
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


inline bool CCalibratedViewBase::IsDistanceMeasureToolActive() const
{
	return m_isDistanceMeasureToolActive;
}


inline void CCalibratedViewBase::SetDistanceMeasureToolActive(bool state)
{
	if (state){
		m_isPointMeasureToolActive = false;
	}

	if (m_isDistanceMeasureToolActive != state){
		m_isDistanceMeasureToolActive = state;

		InvalidateBackground();
	}
}


inline bool CCalibratedViewBase::IsPointMeasureToolActive() const
{
	return m_isPointMeasureToolActive;
}


inline void CCalibratedViewBase::SetPointMeasureToolActive(bool state)
{
	if (state){
		m_isDistanceMeasureToolActive = false;
	}

	if (m_isPointMeasureToolActive != state){
		m_isPointMeasureToolActive = state;

		InvalidateBackground();
	}
}


// reimplemented (i2d::ICalibrationProvider)

inline const i2d::ICalibration2d* CCalibratedViewBase::GetCalibration() const
{
	return m_calibrationPtr;
}


} // namespace iview


#endif // !iview_CCalibratedViewBase_included




