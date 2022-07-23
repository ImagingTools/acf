#pragma once

// Qt includes
#include <QtCore/QObject>
#include <QtCore/QSet>
#include <QtWidgets/QWidget>


namespace iwidgets
{


/**
	This class created to avoid an accident changing values in comboBoxes or spinBoxes.
	It blocks wheel events while comboBoxes or spinBoxes are have not focus.
 */
class CWidgetWheelEventBlocker: public QObject
{

public:
	enum AffectedWidgets
	{
		/**
			If set, the wheel events will be blocked for spinBoxes (QAbstractSpinBox class)
		*/
		AW_SPIN_BOXES = 1 << 0,

		/**
			If set, the wheel events will be blocked for comboBoxes (QComboBox class)
		*/
		AW_COMBO_BOXES = 1 << 1
	};

	CWidgetWheelEventBlocker(QWidget* parentPtr, int flags = AW_SPIN_BOXES | AW_COMBO_BOXES);

protected:
	typedef QList<QObject*> ObjectPtrList;

	/**
		\brief Adds all supported widgets ftom the provided list depends on current \enum ProcessingFlags.
		\param widgetPtrsList - the widget list to add for ignore events.
		\return true - if at least one widget has been found and added otherwise false.
		\sa SetProcessingFlags.
	 */
	void FilterWidgets(ObjectPtrList& widgetPtrsList);

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* objectPtr, QEvent* eventPtr) override;

private:
	/**
		\brief Finds and adds all supported widgets depends on current \enum ProcessingFlags.
		\param parent - the widget where searching is required to add its for ignore events.
		\return true - if at least one widget has been found and added otherwise false.
		\sa SetProcessingFlags.
	 */
	void AddSupportedChildWidgets(QWidget& parentObject);

	/**
		Sets the event filter of the widget to this class and adds it to the watching list.
	*/
	void AcquireWidget(QWidget& widget);

private:
	int m_processingFlags;
	QSet<QWidget*> m_filteredWidgets;

};


} // namespace iwidgets

