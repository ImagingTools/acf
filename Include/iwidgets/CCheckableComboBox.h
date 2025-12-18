#ifndef iwidgets_CCheckableComboBox_included
#define iwidgets_CCheckableComboBox_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QComboBox>
#include <QtWidgets/QItemDelegate>
#else
#include <QtGui/QComboBox>
#include <QtGui/QItemDelegate>
#endif


// ACF includes
#include <istd/TDelPtr.h>
#include <iwidgets/iwidgets.h>


namespace iwidgets
{


class CCheckableComboBox: public QComboBox
{
	Q_OBJECT

public:
	typedef QComboBox BaseClass;

	explicit CCheckableComboBox(QWidget* parentPtr = NULL);

	void SetItemsChecked(const QStringList& items, bool isChecked = true);

protected:
	virtual void UpdateDisplayText();

	// reimplemented (QWidget)
	virtual void paintEvent(QPaintEvent* eventPtr) override;

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* object, QEvent* eventPtr) override;

Q_SIGNALS:
	/**
		Emit the list of items, they have checked state.
	*/
	void EmitActivatedItems(const QStringList& items);

private:
	class CCheckBoxDelegate: public QItemDelegate
	{
	public:
		typedef QItemDelegate BaseClass;

		explicit CCheckBoxDelegate(CCheckableComboBox& parent);

	protected:
		// reimplemented (QItemDelegate)
		virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
		virtual QWidget *createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
		virtual void setEditorData(QWidget* editor, const QModelIndex &index) const override;
		virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

	private:
		CCheckableComboBox& m_parent;
	};

	QString m_displayText;
};


} // namespace iwidgets


#endif // !iwidgets_CCheckableComboBox_included


