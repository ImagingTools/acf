#ifndef iwidgets_CHierarchicalHeaderView_included
#define iwidgets_CHierarchicalHeaderView_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QHeaderView>
#else
#include <QtGui/QHeaderView>
#endif


namespace iwidgets
{


/**
	Hierarchical header view for QTableView.
*/
class CHierarchicalHeaderView: public QHeaderView
{
	Q_OBJECT
public:
	typedef QHeaderView BaseClass;

	enum HeaderDataModelRoles
	{
		HorizontalHeaderDataRole=Qt::UserRole,
		VerticalHeaderDataRole=Qt::UserRole+1
	};

	CHierarchicalHeaderView(Qt::Orientation orientation, QWidget* parent = 0);

	// reimplemented (QHeaderView)
	virtual void setModel(QAbstractItemModel* model) override;

protected:
	// reimplemented (QHeaderView)
	virtual void paintSection(
				QPainter* painter,
				const QRect& rect,
				int logicalIndex) const override;
	virtual QSize sectionSizeFromContents(int logicalIndex) const override;

private Q_SLOTS:
	void on_sectionResized(int logicalIndex);

private:
	QStyleOptionHeader GetStyleOptionForCell(int logicalIndex) const;
	QModelIndex GetChildIndex(int sectionIndex) const;
	QSize GetCellSize(
				const QModelIndex& childIndex,
				const QHeaderView* headerViewPtr,
				QStyleOptionHeader styleOptions) const;
	void PaintHorizontalSection(
				QPainter *painter,
				const QRect& sectionRect,
				int logicalChildIndex,
				const QHeaderView* headerViewPtr,
				const QStyleOptionHeader& styleOptions,
				const QModelIndex& childIndex) const;
	void PaintVerticalSection(
				QPainter *painter,
				const QRect& sectionRect,
				int logicalChildIndex,
				const QHeaderView* headerViewPtr,
				const QStyleOptionHeader& styleOptions,
				const QModelIndex& childIndex) const;
	QModelIndexList GetChilds(const QModelIndex& searchedIndex) const;
	QModelIndexList SearchChilds(const QModelIndex& curentIndex) const;
	QModelIndex FindRootIndex(QModelIndex index) const;
	QModelIndexList GetParentIndexes(QModelIndex index) const;
	QModelIndex FindChild(const QModelIndex& curentIndex, int sectionIndex, int& curentChildIndex) const;
	void SetForegroundBrush(QStyleOptionHeader& opt, const QModelIndex& index) const;
	void SetBackgroundBrush(QStyleOptionHeader& opt, const QModelIndex& index) const;
	int	GetCurrentCellWidth(
				const QModelIndex& searchedIndex,
				const QModelIndex& childIndex,
				int sectionIndex,
				const QHeaderView* headerViewPtr) const;
	int	GetCurrentCellLeft(
				const QModelIndex& searchedIndex,
				const QModelIndex& childIndex,
				int sectionIndex,
				int left,
				const QHeaderView* headerViewPtr) const;
	int PaintHorizontalCell(
				QPainter *painter,
				const QHeaderView* headerViewPtr,
				const QModelIndex& cellIndex,
				const QModelIndex& childIndex,
				int logicalChildIndex,
				const QStyleOptionHeader& styleOptions,
				const QRect& sectionRect,
				int top) const;
	int PaintVerticalCell(
				QPainter *painter,
				const QHeaderView* headerViewPtr,
				const QModelIndex& cellIndex,
				const QModelIndex& childIndex,
				int logicalChildIndex,
				const QStyleOptionHeader& styleOptions,
				const QRect& sectionRect,
				int left) const;
};


} // namespace iwidgets


#endif // !iwidgets_CHierarchicalHeaderView_included

