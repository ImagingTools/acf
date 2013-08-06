#ifndef iqtgui_CItemDelegate_included
#define iqtgui_CItemDelegate_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QItemDelegate>
#else
#include <QtGui/QItemDelegate>
#endif


// ACF includes
#include "iqtgui/iqtgui.h"


namespace iqtgui
{


/**
	Standard ACF item view delegator.
*/
class CItemDelegate: public QItemDelegate
{
public:
	typedef QItemDelegate BaseClass;

	CItemDelegate(int itemHeight = 20, QObject* parent = NULL);

	/**
		Set the item height.
	*/
	void SetItemHeight(int itemHeight);

	/**
		Get the item height.
	*/
	int GetItemHeight() const;

	// reimplemented (QItemDelegate)
	virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
	int m_itemHeight;
};


} // namespace iqtgui


#endif // !iqtgui_CItemDelegate_included


