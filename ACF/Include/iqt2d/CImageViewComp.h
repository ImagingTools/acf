#ifndef iqt2d_CImageViewComp_included
#define iqt2d_CImageViewComp_included


#include <QGraphicsView>
#include <QGraphicsScene>


#include "imod/TSingleModelObserverBase.h"
#include "imod/IModel.h"

#include "iimg/IBitmap.h"

#include "iqt/TGuiComponentBase.h"
#include "iqt/TGuiObserverWrap.h"

#include "iqt2d/CImageItem.h"

#include "idoc/ICommandsProvider.h"

#include "iqt/CHierarchicalCommand.h"


namespace iqt2d
{


class CImageViewComp: 
			public iqt::TGuiObserverWrap<iqt::TGuiComponentBase<QGraphicsView>, imod::TSingleModelObserverBase<iimg::IBitmap> >,
			public idoc::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqt::TGuiObserverWrap<
				iqt::TGuiComponentBase<QGraphicsView>,
				imod::TSingleModelObserverBase<iimg::IBitmap> > BaseClass;

	I_BEGIN_COMPONENT(CImageViewComp)
		I_REGISTER_INTERFACE(imod::IObserver)
		I_ASSIGN(m_allowWidgetResizeAttrPtr, "AllowWidgetResize", "Allow resize of QWidet object (should be disabled if this GUI size is managed by layout)", true, false)
	I_END_COMPONENT

	enum FitMode{
		NoFit,
		ScaleToFit
	};

	CImageViewComp();
	virtual ~CImageViewComp();

	void SetFitMode(FitMode fitMode);
	void SetFullScreenMode(bool isFullScreen);

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

public slots:
	void SetZoom(double scaleFactor);
	void SetZoom(const QString& zoomString);
	void OnZoomIncrement();
	void OnZoomDecrement();
	void SwitchFullScreen();
	void OnFitToView();
	void OnFitToImage();
	void OnResetScale();
	void OnAutoFit(bool isAutoScale);
	
protected:
	virtual void OnResize(QResizeEvent* event);
	virtual void OnWheelEvent(QGraphicsSceneWheelEvent* event);
	virtual void OnKeyReleaseEvent(QKeyEvent* event);
	virtual void OnContextMenuEvent(QContextMenuEvent* event);
	virtual void OnMouseDoubleClickEvent(QMouseEvent* event);
	virtual void OnMouseMoveEvent(QMouseEvent* event);

	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* obj, QEvent* event);

private:
	void InvalidateScene();
	void CreateContextMenu();
	void ScaleView(double scaleFactor);

signals:
	void zoomChanged(double);

private:
	QGraphicsScene* m_scenePtr;
	CImageItem m_imageItem;
	
	FitMode m_fitMode;

	bool m_isFullScreenMode;
	bool m_isZoomIgnored;

	iqt::CHierarchicalCommand m_editorCommand;
	iqt::CHierarchicalCommand m_fitToViewCommand;
	iqt::CHierarchicalCommand m_resetZoomCommand;

	I_ATTR(bool, m_allowWidgetResizeAttrPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CImageViewComp_included


