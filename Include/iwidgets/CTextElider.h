#ifndef iwidgets_CTextElider_included
#define iwidgets_CTextElider_included


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QTimer>


namespace iwidgets
{


/*
	Helper class to control visual text behavoir during resizing of widgets, which contain text representation
	(e.g. QAbstractButton, QLineEdit, etc.).
	If the widget size is smaller than the size of the text, the text will be croped and shown with a specified suffix.
	For example the text "blablabla" will be shown as "blabl...".
	You can register multiple widget objects by the text elider with the method RegisterElideObject().
	If you want specifiy your own elide suffix, use SetElideSuffix() method.
*/
class CTextElider: public QObject
{
	Q_OBJECT
public:
	explicit CTextElider(QObject* parentPtr = NULL);
	virtual ~CTextElider();

	bool RegisterElideObject(QObject* elideObject);
	void UnregisterElideObject(QObject* elideObject);
	void UnregisterAllElideObjects();
	void SetUpdateInterval(int updateInterval = 300);
	void SetElideSuffix(QString elideSuffix);
	QString GetElideSuffix() const;

protected:
	// reimplemented (QObject)
	virtual bool eventFilter(QObject* objectPtr, QEvent* eventPtr) override;

private:
	void UpdateElidedText(QObject& elideObject);

	void OnTimeout();

private Q_SLOTS:
	void OnObjectDestroyed(QObject* objectPtr);

private:
	struct ElideObjectInfo
	{
		QString baseString;
		QString elideString;
	};

	typedef QMap<QObject*, ElideObjectInfo> ElideObjectsMap;
	ElideObjectsMap m_objectPtrMap;

	QTimer m_textObservingTimer;
	int m_updateInterval;
	QString m_elideSuffix;
};


} // namespace iwidgets


#endif // !iwidgets_CTextElider_included


