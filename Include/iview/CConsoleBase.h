#ifndef iview_CConsoleBase_included
#define iview_CConsoleBase_included


// Qt includes
#include <QtCore/QString>
#include <QtGui/QWidget>


// ACF includes
#include "i2d/CRectangle.h"

#include "iview/IViewEventObserver.h"
#include "iview/CScreenTransform.h"


namespace iview
{


class CViewport;


class CConsoleBase: public QWidget
{
public:
	/**
		Control fitting mode, to fit area of interest (AOI) to display.
	*/
	enum FitMode
	{
		/**
			No fitting, frame use always identity transformation.
		*/
		FM_RESET,
		
		/**
			Scale both axes proportional to display biggest, but full visible AOI.
		*/
		FM_BOTH,
		
		/**
			Only horizontal size of AOI will be consider.
		*/
		FM_HORIZONTAL,
		
		/**
			Only vertical size of AOI will be consider.
		*/
		FM_VERTICAL,
		
		/**
			Both axes will be scaled separately.
		*/
		FM_UNPROP,
		
		/**
			Scale both axes proportional to display smallest AOI, which fully covers display.
		*/
		FM_COVER
	};

	CConsoleBase(QWidget* parent);

	// console visibility
	bool IsZoomToFit() const;
	void SetZoomToFit(bool state = true);
	FitMode GetFitMode() const;
	void SetFitMode(FitMode mode);
	bool IsGridVisible() const;
	void SetGridVisible(bool state = true);
	bool IsRulerVisible() const;
	void SetRulerVisible(bool state = true);
	bool IsGridInMm() const;
	void SetGridInMm(bool state = true);

	// gui visibility
	bool IsButtonsPanelVisible() const;
	void SetButtonsPanelVisible(bool state = true);
	bool IsButtonsPanelVertical() const;
	void SetButtonsPanelVertical(bool state = true);
	bool AreScrollbarsVisible() const;
	void SetScrollbarsVisible(bool state = true);

	// buttons visibility
	bool AreZoomsVisible() const;
	void SetZoomsVisible(bool state = true);
	bool IsZoomToFitVisible() const;
	void SetZoomToFitVisible(bool state = true);
	bool ArePolylineButtonsVisible() const;
	void SetPolylineButtonsVisible(bool state = true);
	bool AreUserModeButtonsVisible() const;
	void SetUserModeButtonsVisible(bool state = true);
	bool IsScrollbarsButtonVisible() const;
	void SetScrollbarsButtonVisible(bool state = true);
	bool IsGridButtonVisible() const;
	void SetGridButtonVisible(bool state = true);
	bool IsRulerButtonVisible() const;
	void SetRulerButtonVisible(bool state = true);
	bool IsMmButtonVisible() const;
	void SetMmButtonVisible(bool state = true);

	/**
		Check if background object is present and active.
	*/
	bool IsBackgroundActive() const;

	void UpdateView();

	// abstract methods

	/**
		Get access to internal view implementation.
	*/
	virtual const CViewport& GetView() const = 0;

	/**
		Get access to internal view implementation.
	*/
	virtual CViewport& GetViewRef() = 0;

	/**
		Update state of edit mode buttons.
		Edit mode buttons controls the mode of interaction with objects.
		For example for polyline nodes: select, move, insert, remove.
		\sa iview::ISelectable::EditMode
	*/
	virtual void UpdateEditModeButtons() = 0;
	
	/**
		Recalculates enabling flags for buttons.
	*/
	virtual void UpdateButtonsState() = 0;
	
	/**
		Recalculates position of all components.
	*/
	virtual void UpdateComponentsPosition() = 0;

	/**
		Recalculate menu commands.
	*/
	virtual void UpdateCommands() = 0;

	// abstract methods
	virtual void UpdateCursorInfo(const QString& infoText) = 0;

protected:
	// events
	virtual bool OnSelectChange(const iview::IShapeView& view, const istd::CIndex2d& position, const iview::IInteractiveShape& shape, bool state) = 0;
	virtual bool OnMouseButton(const iview::IShapeView& view, const istd::CIndex2d& position, Qt::MouseButton buttonType, bool state, const iview::IInteractiveShape* shapePtr) = 0;
	virtual void OnBoundingBoxChanged() = 0;

private:
	void Init();

	FitMode m_fitMode;
	bool m_isRulerVisible;

	bool m_areScollbarsVisible;
	bool m_isButtonsPanelVisible;
	bool m_isButtonsPanelVertical;

	bool m_areZoomsVisible;
	bool m_isZoomToFitVisible;
	bool m_arePolylineButtonsVisible;
	bool m_areUserModeButtonsVisible;
	bool m_isScrollbarsButtonVisible;
	bool m_isGridButtonVisible;
	bool m_isRulerButtonVisible;
	bool m_isMmButtonVisible;

	iview::CScreenTransform m_storedTransform;

	bool m_isZoomToFit;

	bool m_isBkActive;

	friend class CViewport;
};


} // namespace iview


#endif // !iview_CConsoleBase_included

