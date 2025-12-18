#ifndef iwidgets_CExtLineEdit_included
#define iwidgets_CExtLineEdit_included


// Qt includes
#include <QtCore/QList>
#include <QtGui/QIcon>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#else
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#endif

// ACF includes
#include <istd/TDelPtr.h>

#include <iwidgets/iwidgets.h>


namespace iwidgets
{


/**
	Input editor with extended functionality.
	It's possible to add an icon to the edit field
	or insert additional widgets (e.g control buttons) into the view.
*/
class CExtLineEdit: public QLineEdit
{
	Q_OBJECT

public:
	typedef QLineEdit BaseClass;

	explicit CExtLineEdit(QWidget* parent = NULL);

	/**
		Construct a line edit with the given properties.
	*/
	CExtLineEdit(
				const QString& startupText,
				int margins,
				QWidget* parent = NULL);

	/**
		Set the text, that will be shown at the first time the editor becomes visible.
	*/
	virtual void SetStartupText(const QString& startupText);

	/**
		Get the startup text.
	*/
	virtual QString GetStartupText() const;

	/**
		Get editor text.
	*/
	virtual QString GetText() const;

	/**
		Set the icon, that will appeared on the left side of the line edit.
	*/
	void SetIcon(const QIcon& icon);

	/**
		Add a widget to the line edit area according to \c alginmentFlags.
		Only Qt::AlignLeft and Qt::AlignRight are possible.
		\note CExtLineEdit get the control over the \c widget object.
	*/
	void AddWidget(QWidget* widgetPtr, int alignmentFlags);

protected:
	// reimplemented (QWidget)
	virtual void resizeEvent(QResizeEvent* resizeEvent) override;
	virtual void focusInEvent(QFocusEvent* focusEvent) override;

private Q_SLOTS:
	void OnEditingFinished();
	void OnTextChanged(const QString& text);

private:
	void UpdateLayout();
	void UpdateStartupText();
	void InitWidget();

private:
	QString m_startupText;
	int m_margins;

	struct SubWidgetInfo
	{
		QWidget* widget;
		int alignmentFlags;
		int margin;
	};

	typedef QList<SubWidgetInfo> SubWidgetsList;

	QLabel* m_iconPtr;
	SubWidgetsList m_subWidgets;
};


} // namespace iwidgets


#endif // !iwidgets_CExtLineEdit_included


