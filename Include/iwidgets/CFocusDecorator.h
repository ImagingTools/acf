#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QMap>
#include <QtCore/QEvent>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QGraphicsEffect>
#include <QtWidgets/QWidget>
#else
#include <QtGui/QGraphicsEffect>
#include <QtGui/QWidget>
#endif

// ACF includes
#include <istd/TIFactory.h>


namespace iwidgets
{


/*
	Helper class for creating widget decorations based on graphics effects, such drop shadow.
*/
class CFocusDecorator: public QObject
{
	Q_OBJECT
public:
	typedef istd::TIFactory<QGraphicsEffect> GraphicsEffectFactory;

	explicit CFocusDecorator(QObject* parentPtr = NULL);
	virtual ~CFocusDecorator();

	bool RegisterWidget(QWidget* widgetPtr, GraphicsEffectFactory* effectFactoryPtr);
	void UnregisterWidget(QWidget* widgetPtr);
	void UnregisterAllWidgets();

protected:
	// reimplemented (QObject)
	virtual bool eventFilter(QObject* objectPtr, QEvent* eventPtr) override;

private Q_SLOTS:
	void OnObjectDestroyed(QObject* objectPtr);

private:
	typedef QMap<QObject*, GraphicsEffectFactory*> WidgetEffectsMap;
	WidgetEffectsMap m_widgetEffectsMap;
};


} // namespace iwidgets


