#ifndef iqt2d_CImageItem_included
#define iqt2d_CImageItem_included


#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>


namespace iqt2d
{


class CImageItem: public QObject, public QGraphicsItemGroup
{
public:
	CImageItem();
	virtual ~CImageItem();

	void SetImage(const QImage& image);
	int GetWidth() const;
	int GetHeight() const;

protected:
	class ImageItem: public QGraphicsRectItem
	{
	public:
		void SetImage(const QImage& image);
		int GetWidth() const;
		int GetHeight() const;

	protected:
		void CreateBackgroundPixmap();

		// reimplemented (QGraphicsRectItem)
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

	private:
		QPixmap m_pixmap;
		QPixmap m_backgroundPixmap;
	};

	class ImageFrame: public QGraphicsRectItem
	{
	protected:
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
	};

private:

private:
	ImageItem m_imageItem;
	ImageFrame m_frameItem;
};


} // namespace iqt2d


#endif // !iqt2d_CImageItem_included

