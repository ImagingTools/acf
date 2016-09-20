#include <iwidgets/CItemDelegate.h>


// Qt includes
#include <QtGui/QPainter>


namespace iwidgets
{


// public methods

CItemDelegate::CItemDelegate(int itemHeight, QObject* parent)
	:BaseClass(parent),
	m_itemHeight(itemHeight)
{
}


void CItemDelegate::SetItemHeight(int itemHeight)
{
	m_itemHeight = itemHeight;
}


int CItemDelegate::GetItemHeight() const
{
	return m_itemHeight;
}


// reimplemented (QItemDelegate)

QSize CItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	QSize size = QItemDelegate::sizeHint(option, index);

	QFontMetrics fontMetrics(option.font);

	int height = qMax(fontMetrics.height(), m_itemHeight);

	size.setHeight(height);

	return size;
}


void CItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QItemDelegate::paint(painter, option, index);

	QRect rect = option.rect;
	painter->setPen(QPen(Qt::darkGray, 0, Qt::SolidLine));
	if (index.column() != 3){
		painter->drawLine(rect.topRight(), rect.bottomRight());
	}

	painter->drawLine(rect.bottomLeft(), rect.bottomRight());
}


} // namespace iwidgets


