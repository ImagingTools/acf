#ifndef iqt2d_CImageItem_included
#define iqt2d_CImageItem_included


#include <QGraphicsRectItem>

#include "istd/CIndex2d.h"

#include "iimg/IBitmap.h"

#include "iqt2d/iqt2d.h"


namespace iqt2d
{


class CImageItem: public QGraphicsRectItem
{
public:
	CImageItem();

	istd::CIndex2d GetSize() const;

	void SetBitmap(const iimg::IBitmap& bitmap);

	// reimplemented (QGraphicsRectItem)
	virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

private:
	QPixmap m_backgroundPixmap;

	QPixmap m_bitmap;
};


// inline methods

inline istd::CIndex2d CImageItem::GetSize() const
{
	return iqt::GetCIndex2d(m_bitmap.size());
}


} // namespace iqt2d


#endif // !iqt2d_CImageItem_included


