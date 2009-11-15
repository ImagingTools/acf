#ifndef iqtgui_CExtLineEdit_included
#define iqtgui_CExtLineEdit_included


// Qt includes
#include <QLineEdit>
#include <QIcon>
#include <QLabel>
#include <QList>


// ACF includes 
#include "istd/TDelPtr.h"

#include "iqtgui/iqtgui.h"


namespace iqtgui
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

	/**
		Construct a line edit with the given properties.
	*/
	CExtLineEdit(
				const QString& startupText = QString(),
				int margins = 2,
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
	virtual void resizeEvent(QResizeEvent* resizeEvent);
    virtual void focusInEvent(QFocusEvent* focusEvent);

private Q_SLOTS:
	void OnEditingFinished();
	void OnTextChanged(const QString& text);

private:
	void UpdateLayout();
	void UpdateStartupText();

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


} // namespace iqtgui


#endif // !iqtgui_CExtLineEdit_included


