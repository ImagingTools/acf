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


namespace iqt2d
{


class CImageViewComp: 
			public iqt::TGuiObserverWrap<iqt::TGuiComponentBase<QGraphicsView>, imod::TSingleModelObserverBase<iimg::IBitmap> >
{
	Q_OBJECT

public:
	typedef iqt::TGuiObserverWrap<
				iqt::TGuiComponentBase<QGraphicsView>,
				imod::TSingleModelObserverBase<iimg::IBitmap> > BaseClass;

	I_BEGIN_COMPONENT(CImageViewComp)
		I_REGISTER_INTERFACE(imod::IObserver)
	I_END_COMPONENT

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

	enum FitMode{
		NoFit,
		ScaleToFit
	};

	CImageViewComp();
	virtual ~CImageViewComp();

	void SetFitMode(FitMode fitMode);
	void SetFullScreenMode(bool isFullScreen);

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();

public slots:
	void SetZoom(qreal zoomFactor);
	void SetZoom(const QString &);
	void onIncrementZoom();
	void onDecrementZoom();
	void onFitInView();
	void onFitToImage();
	void switchFullScreen();
	
signals:
	void zoomChanged(double);
	
protected:
	void ScaleView(qreal);
	void CreateContextMenu();
	int GetImageItemWidth(); 
	int GetImageItemHeight();

/*	virtual void resizeEvent(QResizeEvent*);
	virtual void wheelEvent(QWheelEvent*);
	virtual void keyReleaseEvent(QKeyEvent*);
	virtual void contextMenuEvent(QContextMenuEvent*);
	virtual void mouseDoubleClickEvent(QMouseEvent*);
	virtual void mouseMoveEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);
	virtual void drawBackground(QPainter*, const QRectF&);
	virtual QSize sizeHint() const;
*/
private:
	void InvalidateScene();

private:
	QGraphicsScene m_scene;
	CImageItem m_imageItem;
	QMenu* m_contextMenu;
	QMatrix m_currentTransorm;
	
	QPixmap m_backgroundPixmap;
	FitMode m_fitMode;

	bool m_isFullScreenMode;
	bool m_isZoomIgnored;
};


} // namespace iqt2d


#endif // !iqt2d_CImageViewComp_included