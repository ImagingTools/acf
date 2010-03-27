#ifndef iqtgui_CCheckableComboBox_included
#define iqtgui_CCheckableComboBox_included


// Qt includes
#include <QComboBox>
#include <QItemDelegate>


// ACF includes 
#include "istd/TDelPtr.h"

#include "iqtgui/iqtgui.h"


namespace iqtgui
{


class CCheckableComboBox: public QComboBox
{
	Q_OBJECT

public:
	typedef QComboBox BaseClass;

	CCheckableComboBox(QWidget* parentPtr = NULL);

	void SetItemsChecked(const QStringList& items, bool isChecked = true);

protected:
	virtual void UpdateDisplayText();

	// reimplemented (QWidget)
	virtual void paintEvent(QPaintEvent* eventPtr);

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* object, QEvent* eventPtr);

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

		CCheckBoxDelegate(CCheckableComboBox& parent);
	protected:
		// reimplemented (QItemDelegate)
		virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		virtual QWidget *createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		virtual void setEditorData(QWidget* editor, const QModelIndex &index) const;
		virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
	
	private:
		CCheckableComboBox& m_parent;
	};

	QString m_displayText;
};


} // namespace iqtgui


#endif // !iqtgui_CCheckableComboBox_included


