#include <iwidgets/CCheckableComboBox.h>


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QApplication>
#include <QtWidgets/QStylePainter>
#include <QtWidgets/QAbstractItemView>
#else
#include <QtGui/QCheckBox>
#include <QtGui/QApplication>
#include <QtGui/QStylePainter>
#include <QtGui/QAbstractItemView>
#endif

// ACF includes
#include <iqt/CSignalBlocker.h>


namespace iwidgets
{


// public methods

CCheckableComboBox::CCheckableComboBox(QWidget* parent)
	:BaseClass(parent)
{
	view()->setItemDelegate(new CCheckBoxDelegate(*this));
	view()->setEditTriggers(QAbstractItemView::CurrentChanged);
	view()->viewport()->installEventFilter(this);
}


void CCheckableComboBox::SetItemsChecked(const QStringList& items, bool isChecked)
{
	for (int itemIndex = 0; itemIndex < count(); itemIndex++){
		QString currentItemText = itemText(itemIndex);
		setItemData(itemIndex, false, Qt::UserRole);

		if (qFind(items, currentItemText) != items.end()){
			bool currentState = itemData(itemIndex, Qt::UserRole).toBool();
			if (currentState != isChecked){
				setItemData(itemIndex, isChecked, Qt::UserRole);
			}
		}
	}

	UpdateDisplayText();
}


// protected methods

void CCheckableComboBox::UpdateDisplayText()
{
	m_displayText.clear();
	QStringList activatedItems;

	for (int itemIndex = 0; itemIndex < count(); itemIndex++){
		bool isChecked = itemData(itemIndex, Qt::UserRole).toBool();
		if (isChecked){
			m_displayText += itemText(itemIndex);
			m_displayText += ";";

			activatedItems.push_back(itemText(itemIndex));
		}
	}

	update();

	Q_EMIT EmitActivatedItems(activatedItems);
}


// reimplemented (QWidget)

void CCheckableComboBox::paintEvent(QPaintEvent* /*eventPtr*/)
{
	QStylePainter painter(this);

	painter.setPen(palette().color(QPalette::Text));

	QStyleOptionComboBox opt;
	initStyleOption(&opt);

	if (m_displayText.isNull()){
		opt.currentText = tr("None");
	}
	else{
		opt.currentText = m_displayText;
	}

	painter.drawComplexControl(QStyle::CC_ComboBox, opt);
	painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}


// reimplemented (QObject)

bool CCheckableComboBox::eventFilter(QObject* object, QEvent* eventPtr)
{
	if (eventPtr->type() == QEvent::MouseButtonRelease && object == view()->viewport()){
		return true;
	}

	return QComboBox::eventFilter(object, eventPtr);
}


// public methods of embedded class CCheckBoxDelegate

CCheckableComboBox::CCheckBoxDelegate::CCheckBoxDelegate(CCheckableComboBox& parent)
	:BaseClass(&parent),
	m_parent(parent)
{
}


// protected methods of embedded class CCheckBoxDelegate

// reimplemented (QItemDelegate)

void CCheckableComboBox::CCheckBoxDelegate::paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	bool value = index.data(Qt::UserRole).toBool();
	QString text = index.data(Qt::DisplayRole).toString();

	const QStyle *style = QApplication::style();
	QStyleOptionButton opt;
	opt.state |= value ? QStyle::State_On : QStyle::State_Off;
	opt.state |= QStyle::State_Enabled;
	opt.text = text;
	opt.rect = option.rect;

	style->drawControl(QStyle::CE_CheckBox, &opt, painter);
}


QWidget* CCheckableComboBox::CCheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem&/* option*/, const QModelIndex& /*index*/) const
{
	return new QCheckBox(parent);
}


void CCheckableComboBox::CCheckBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QCheckBox* editorPtr = dynamic_cast<QCheckBox*>(editor);
	if (editorPtr != NULL){
		editorPtr->setText(index.data(Qt::DisplayRole).toString());

		editorPtr->setChecked(index.data(Qt::UserRole).toBool());
	}
	else{
		BaseClass::setEditorData(editor, index);
	}
}


void CCheckableComboBox::CCheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QCheckBox* myEditor = dynamic_cast<QCheckBox*>(editor);
	if (myEditor != NULL){
		bool value = myEditor->isChecked();

		QMap<int,QVariant> data;
		data.insert(Qt::DisplayRole, myEditor->text());
		data.insert(Qt::UserRole, value);
		model->setItemData(index, data);
	}
	else{
		BaseClass::setModelData(editor, model, index);
	}

	m_parent.UpdateDisplayText();
}


} // namespace iwidgets


