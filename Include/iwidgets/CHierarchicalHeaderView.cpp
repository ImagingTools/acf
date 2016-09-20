#include <iwidgets/CHierarchicalHeaderView.h>


// Qt includes
#include <QtCore/QAbstractItemModel>
#include <QtGui/QPainter>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QStyleOptionHeader>
#else
#include <QtGui/QStyleOptionHeader>
#endif


namespace iwidgets
{


// public methods

CHierarchicalHeaderView::CHierarchicalHeaderView(Qt::Orientation orientation, QWidget *parent)
	:BaseClass(orientation, parent)
{
	connect(this, SIGNAL(sectionResized(int, int, int)), this, SLOT(on_sectionResized(int)));
}


// reimplemented (QHeaderView)

void CHierarchicalHeaderView::setModel(QAbstractItemModel* modelPtr)
{
	BaseClass::setModel(modelPtr);

	if (modelPtr != NULL){
		int itemsCount = (orientation() == Qt::Horizontal ? modelPtr->columnCount() : modelPtr->rowCount());
		if (itemsCount > 0){
			BaseClass::initializeSections(0, itemsCount - 1);
		}
	}
}


// protected methods

// reimplemented (QHeaderView)

void CHierarchicalHeaderView::paintSection(
			QPainter *painter,
			const QRect &rect,
			int logicalIndex) const
{
	if (rect.isValid()){
		QModelIndex childIndex(GetChildIndex(logicalIndex));
		if (childIndex.isValid()){
			if (orientation() == Qt::Horizontal){
				PaintHorizontalSection(painter, rect, logicalIndex, this, GetStyleOptionForCell(logicalIndex), childIndex);
			}
			else{
				PaintVerticalSection(painter, rect, logicalIndex, this, GetStyleOptionForCell(logicalIndex), childIndex);
			}

			return;
		}
	}

	BaseClass::paintSection(painter, rect, logicalIndex);
}


QSize CHierarchicalHeaderView::sectionSizeFromContents(int logicalIndex) const
{
	QAbstractItemModel* modelPtr = model();
	if (modelPtr != NULL){
		QModelIndex currentChildIndex(GetChildIndex(logicalIndex));

		if (currentChildIndex.isValid()){
			QStyleOptionHeader styleOption(GetStyleOptionForCell(logicalIndex));

			QSize cellSize(GetCellSize(currentChildIndex, this, styleOption));

			currentChildIndex = currentChildIndex.parent();

			while(currentChildIndex.isValid()){
				if (orientation() == Qt::Horizontal){
					cellSize.rheight() += GetCellSize(currentChildIndex, this, styleOption).height();
				}
				else{
					cellSize.rwidth() += GetCellSize(currentChildIndex, this, styleOption).width();
				}

				currentChildIndex = currentChildIndex.parent();
			}

			return cellSize;
		}
	}

	return BaseClass::sectionSizeFromContents(logicalIndex);
}


// private slots

void CHierarchicalHeaderView::on_sectionResized(int logicalIndex)
{
	if (isSectionHidden(logicalIndex)){
		return;
	}

	QModelIndex childIndex(GetChildIndex(logicalIndex));
	if (childIndex.isValid()){

		QModelIndexList childsList(GetChilds(FindRootIndex(childIndex)));

		for (int n = childsList.indexOf(childIndex); n > 0; --n){
			--logicalIndex;

			int w = viewport()->width();
			int h = viewport()->height();
			int pos = sectionViewportPosition(logicalIndex);
			QRect r(pos, 0, w - pos, h);

			if (orientation() == Qt::Horizontal){
				if (isRightToLeft()){
					r.setRect(0, 0, pos + sectionSize(logicalIndex), h);
				}
			}
			else{
				r.setRect(0, pos, w, h - pos);
			}

			viewport()->update(r.normalized());
		}
	}
}


// private methods

QStyleOptionHeader CHierarchicalHeaderView::GetStyleOptionForCell(int sectionIndex) const
{
	QStyleOptionHeader opt;
	initStyleOption(&opt);

	if (window()->isActiveWindow()){
		opt.state |= QStyle::State_Active;
	}

	opt.textAlignment = Qt::AlignCenter;
	opt.iconAlignment = Qt::AlignVCenter;
	opt.section = sectionIndex;

	int visualIndex = BaseClass::visualIndex(sectionIndex);

	if (count() == 1){
		opt.position = QStyleOptionHeader::OnlyOneSection;
	}
	else{
		if (visualIndex == 0){
			opt.position = QStyleOptionHeader::Beginning;
		}
		else{
			opt.position = (visualIndex == (count() - 1) ? QStyleOptionHeader::End : QStyleOptionHeader::Middle);
		}
	}

	QItemSelectionModel* selectionModelPtr = selectionModel();

#if QT_VERSION >= 0x050000	// TODO: find out what is equivalent of QHeaderView::isClickable for Qt 5?
	if (sectionsClickable()){
#else
	if (isClickable()){
#endif
		if (highlightSections() && (selectionModelPtr != NULL)){
			if (orientation() == Qt::Horizontal){
				if (selectionModelPtr->columnIntersectsSelection(sectionIndex, rootIndex())){
					opt.state |= QStyle::State_On;
				}

				if (selectionModelPtr->isColumnSelected(sectionIndex, rootIndex())){
					opt.state |= QStyle::State_Sunken;
				}
			}
			else{
				if (selectionModelPtr->rowIntersectsSelection(sectionIndex, rootIndex())){
					opt.state |= QStyle::State_On;
				}
				if (selectionModelPtr->isRowSelected(sectionIndex, rootIndex())){
					opt.state |= QStyle::State_Sunken;
				}
			}
		}
	}

	if (selectionModelPtr != NULL){
		bool previousSelected = false;
		if (orientation() == Qt::Horizontal){
			previousSelected = selectionModelPtr->isColumnSelected(logicalIndex(visualIndex - 1), rootIndex());
		}
		else{
			previousSelected = selectionModelPtr->isRowSelected(logicalIndex(visualIndex - 1), rootIndex());
		}

		bool nextSelected = false;
		if (orientation() == Qt::Horizontal){
			nextSelected = selectionModelPtr->isColumnSelected(logicalIndex(visualIndex + 1), rootIndex());
		}
		else{
			nextSelected = selectionModelPtr->isRowSelected(logicalIndex(visualIndex + 1), rootIndex());
		}

		if (previousSelected && nextSelected){
			opt.selectedPosition = QStyleOptionHeader::NextAndPreviousAreSelected;
		}
		else{
			if (previousSelected){
				opt.selectedPosition = QStyleOptionHeader::PreviousIsSelected;
			}
			else{
				opt.selectedPosition = nextSelected ? QStyleOptionHeader::NextIsSelected : QStyleOptionHeader::NotAdjacent;
			}
		}
	}

	return opt;
}


QModelIndex CHierarchicalHeaderView::FindRootIndex(QModelIndex index) const
{
	while (index.parent().isValid()){
		index=index.parent();
	}

	return index;
}


QModelIndexList CHierarchicalHeaderView::GetParentIndexes(QModelIndex index) const
{
	QModelIndexList indexes;
	while(index.isValid()){
		indexes.push_front(index);

		index = index.parent();
	}

	return indexes;
}


QModelIndex CHierarchicalHeaderView::FindChild(const QModelIndex& curentIndex, int sectionIndex, int& curentChildIndex) const
{
	if (curentIndex.isValid()){
		int childCount = curentIndex.model()->columnCount(curentIndex);
		if (childCount){
			for (int i = 0; i < childCount; ++i){
				QModelIndex res(FindChild(curentIndex.child(0, i), sectionIndex, curentChildIndex));
				if (res.isValid()){
					return res;
				}
			}
		}
		else
		{
			++curentChildIndex;
			if (curentChildIndex == sectionIndex){
				return curentIndex;
			}
		}
	}

	return QModelIndex();
}


QModelIndex CHierarchicalHeaderView::GetChildIndex(int sectionIndex) const
{
	QAbstractItemModel* modelPtr = model();
	if (modelPtr != NULL){
		int curentChildIndex = -1;

		for (int i = 0; i < modelPtr->columnCount(); ++i)
		{
			QModelIndex res(FindChild(modelPtr->index(0, i), sectionIndex, curentChildIndex));
			if (res.isValid()){
				return res;
			}
		}
	}

	return QModelIndex();
}


QModelIndexList CHierarchicalHeaderView::SearchChilds(const QModelIndex& curentIndex) const
{
	QModelIndexList res;
	if (curentIndex.isValid()){
		int childCount = curentIndex.model()->columnCount(curentIndex);
		if (childCount){
			for (int childIndex = 0; childIndex < childCount; ++childIndex){
				res += SearchChilds(curentIndex.child(0, childIndex));
			}
		}
		else{
			res.push_back(curentIndex);
		}
	}

	return res;
}


QModelIndexList CHierarchicalHeaderView::GetChilds(const QModelIndex& searchedIndex) const
{
	QModelIndexList childs;
	if (searchedIndex.isValid()){
		int childCount = searchedIndex.model()->columnCount(searchedIndex);
		for (int i = 0; i < childCount; ++i){
			childs += SearchChilds(searchedIndex.child(0, i));
		}
	}

	return childs;
}


void CHierarchicalHeaderView::SetForegroundBrush(QStyleOptionHeader& opt, const QModelIndex& index) const
{
	QVariant foregroundBrush = index.data(Qt::ForegroundRole);
#if QT_VERSION >= 0x050000	// TODO: find out what is equivalent of QHeaderView::isClickable for Qt 5?
	if (foregroundBrush.canConvert(QMetaType::QBrush)){
#else
	if (qVariantCanConvert<QBrush>(foregroundBrush)){
#endif
		opt.palette.setBrush(QPalette::ButtonText, qvariant_cast<QBrush>(foregroundBrush));
	}
}


void CHierarchicalHeaderView::SetBackgroundBrush(QStyleOptionHeader& opt, const QModelIndex& index) const
{
	QVariant backgroundBrush = index.data(Qt::BackgroundRole);
#if QT_VERSION >= 0x050000	// TODO: find out what is equivalent of QHeaderView::isClickable for Qt 5?
	if (backgroundBrush.canConvert(QMetaType::QBrush)){
#else
	if (qVariantCanConvert<QBrush>(backgroundBrush)){
#endif
		opt.palette.setBrush(QPalette::Button, qvariant_cast<QBrush>(backgroundBrush));
		opt.palette.setBrush(QPalette::Window, qvariant_cast<QBrush>(backgroundBrush));
	}
}


QSize CHierarchicalHeaderView::GetCellSize(
			const QModelIndex& childIndex,
			const QHeaderView* headerViewPtr,
			QStyleOptionHeader styleOptions) const
{
	QSize res;
	QVariant variant(childIndex.data(Qt::SizeHintRole));
	if (variant.isValid())
		res=qvariant_cast<QSize>(variant);
	QFont fnt(headerViewPtr->font());
	QVariant var(childIndex.data(Qt::FontRole));
#if QT_VERSION >= 0x050000	// TODO: find out what is equivalent of QHeaderView::isClickable for Qt 5?
	if (var.isValid() && var.canConvert(QMetaType::QFont)){
#else
	if (var.isValid() && qVariantCanConvert<QFont>(var)){
#endif
		fnt = qvariant_cast<QFont>(var);
	}

	fnt.setBold(true);

	QFontMetrics fm(fnt);
	QSize size(fm.size(0, childIndex.data(Qt::DisplayRole).toString()));
	if (childIndex.data(Qt::UserRole).isValid()){
		size.transpose();
	}

	QSize decorationsSize(headerViewPtr->style()->sizeFromContents(QStyle::CT_HeaderSection, &styleOptions, QSize(), headerViewPtr));

	QSize emptyTextSize(fm.size(0, ""));

	return res.expandedTo(size + decorationsSize - emptyTextSize);
}


int	CHierarchicalHeaderView::GetCurrentCellWidth(
			const QModelIndex& searchedIndex,
			const QModelIndex& childIndex,
			int sectionIndex,
			const QHeaderView* headerViewPtr) const
{
	QModelIndexList childsList(GetChilds(searchedIndex));
	if (childsList.empty()){
		return headerViewPtr->sectionSize(sectionIndex);
	}

	int firstChildSectionIndex = sectionIndex-childsList.indexOf(childIndex);

	int currentCellWidth = 0;
	for (int i = 0; i < childsList.size(); ++i){
		currentCellWidth += headerViewPtr->sectionSize(firstChildSectionIndex + i);
	}

	return currentCellWidth;
}


int	CHierarchicalHeaderView::GetCurrentCellLeft(
			const QModelIndex& searchedIndex,
			const QModelIndex& childIndex,
			int sectionIndex,
			int left,
			const QHeaderView* headerViewPtr) const
{
	QModelIndexList childsList(GetChilds(searchedIndex));
	if (!childsList.empty()){
		int n = childsList.indexOf(childIndex);

		int firstChildSectionIndex = sectionIndex - n;
		--n;

		for (; n>=0; --n){
			left -= headerViewPtr->sectionSize(firstChildSectionIndex+n);
		}
	}

	return left;
}


int CHierarchicalHeaderView::PaintHorizontalCell(
			QPainter *painter,
			const QHeaderView* headerViewPtr,
			const QModelIndex& cellIndex,
			const QModelIndex& childIndex,
			int logicalChildIndex,
			const QStyleOptionHeader& styleOptions,
			const QRect& sectionRect,
			int top) const
{
	QStyleOptionHeader uniopt(styleOptions);
	SetForegroundBrush(uniopt, cellIndex);
	SetBackgroundBrush(uniopt, cellIndex);

	int height = GetCellSize(cellIndex, headerViewPtr, uniopt).height();
	if (cellIndex == childIndex){
		height = sectionRect.height() - top;
	}

	int left = GetCurrentCellLeft(cellIndex, childIndex, logicalChildIndex, sectionRect.left(), headerViewPtr);
	int width = GetCurrentCellWidth(cellIndex, childIndex, logicalChildIndex, headerViewPtr);

	QRect r(left, top, width, height);

	uniopt.text = cellIndex.data(Qt::DisplayRole).toString();
	painter->save();
	uniopt.rect = r;
	if (cellIndex.data(Qt::UserRole).isValid())
	{
		headerViewPtr->style()->drawControl(QStyle::CE_HeaderSection, &uniopt, painter, headerViewPtr);
		QMatrix m;
		m.rotate(-90);
		painter->setWorldMatrix(m, true);
		QRect new_r(0, 0,  r.height(), r.width());
		new_r.moveCenter(QPoint(-r.center().y(), r.center().x()));
		uniopt.rect = new_r;
		headerViewPtr->style()->drawControl(QStyle::CE_HeaderLabel, &uniopt, painter, headerViewPtr);
	}
	else
	{
		headerViewPtr->style()->drawControl(QStyle::CE_Header, &uniopt, painter, headerViewPtr);
	}
	painter->restore();
	return top+height;
}


void CHierarchicalHeaderView::PaintHorizontalSection(
			QPainter *painter,
			const QRect& sectionRect,
			int logicalChildIndex,
			const QHeaderView* headerViewPtr,
			const QStyleOptionHeader& styleOptions,
			const QModelIndex& childIndex) const
{
	QPointF currentBrushOrigin(painter->brushOrigin());
	int top = sectionRect.y();

	QModelIndexList indexes(GetParentIndexes(childIndex));

	for (int i = 0; i < indexes.size(); ++i){
		QStyleOptionHeader realStyleOptions(styleOptions);

		if (		i < indexes.size() - 1 &&
					(realStyleOptions.state.testFlag(QStyle::State_Sunken) || realStyleOptions.state.testFlag(QStyle::State_On))){
			QStyle::State t(QStyle::State_Sunken | QStyle::State_On);

			realStyleOptions.state &= (~t);
		}

		top = PaintHorizontalCell(painter,
			headerViewPtr,
			indexes[i],
			childIndex,
			logicalChildIndex,
			realStyleOptions,
			sectionRect,
			top);
	}

	painter->setBrushOrigin(currentBrushOrigin);
}


int CHierarchicalHeaderView::PaintVerticalCell(
			QPainter *painter,
			const QHeaderView* headerViewPtr,
			const QModelIndex& cellIndex,
			const QModelIndex& childIndex,
			int logicalChildIndex,
			const QStyleOptionHeader& styleOptions,
			const QRect& sectionRect,
			int left) const
{
	QStyleOptionHeader uniopt(styleOptions);
	SetForegroundBrush(uniopt, cellIndex);
	SetBackgroundBrush(uniopt, cellIndex);

	int width = GetCellSize(cellIndex, headerViewPtr, uniopt).width();
	if (cellIndex == childIndex){
		width = sectionRect.width()-left;
	}

	int top = GetCurrentCellLeft(cellIndex, childIndex, logicalChildIndex, sectionRect.top(), headerViewPtr);
	int height = GetCurrentCellWidth(cellIndex, childIndex, logicalChildIndex, headerViewPtr);

	QRect r(left, top, width, height);

	uniopt.text = cellIndex.data(Qt::DisplayRole).toString();
	painter->save();

	uniopt.rect = r;

	if (cellIndex.data(Qt::UserRole).isValid()){
		headerViewPtr->style()->drawControl(QStyle::CE_HeaderSection, &uniopt, painter, headerViewPtr);
		QMatrix m;
		m.rotate(-90);
		painter->setWorldMatrix(m, true);
		QRect new_r(0, 0,  r.height(), r.width());
		new_r.moveCenter(QPoint(-r.center().y(), r.center().x()));
		uniopt.rect = new_r;
		headerViewPtr->style()->drawControl(QStyle::CE_HeaderLabel, &uniopt, painter, headerViewPtr);
	}
	else{
		headerViewPtr->style()->drawControl(QStyle::CE_Header, &uniopt, painter, headerViewPtr);
	}

	painter->restore();

	return left + width;
}


void CHierarchicalHeaderView::PaintVerticalSection(
			QPainter *painter,
			const QRect& sectionRect,
			int logicalChildIndex,
			const QHeaderView* headerViewPtr,
			const QStyleOptionHeader& styleOptions,
			const QModelIndex& childIndex) const
{
	QPointF currentBrushOrigin(painter->brushOrigin());
	int left=sectionRect.x();
	QModelIndexList indexes(GetParentIndexes(childIndex));
	for (int i = 0; i < indexes.size(); ++i)
	{
		QStyleOptionHeader realStyleOptions(styleOptions);
		if (   i < indexes.size()-1
			&&
			(   realStyleOptions.state.testFlag(QStyle::State_Sunken)
			|| realStyleOptions.state.testFlag(QStyle::State_On)))
		{
			QStyle::State t(QStyle::State_Sunken | QStyle::State_On);
			realStyleOptions.state&=(~t);
		}
		left = PaintVerticalCell(painter,
			headerViewPtr,
			indexes[i],
			childIndex,
			logicalChildIndex,
			realStyleOptions,
			sectionRect,
			left);
	}

	painter->setBrushOrigin(currentBrushOrigin);
}


} // namespace iwidgets


