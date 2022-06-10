#pragma once

// Qt includes
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtWidgets/QWidget>

// Acf includes
#include <istd/IChangeable.h>


namespace iwidgets
{

/**
	This class created to avoid an accident changing values in comboBoxes or spinBoxes.
	It blocks wheel events while comboBoxes or spinBoxes are have not focus.
 */
class CWidgetWheelEventBlocker: public QObject, public istd::IChangeable
{

public:
	enum ProcessingFlags
	{
		/**
			Processing disabled
		*/
		PF_NONE = 0,

		/**
			If set, the wheel events will be blocked for spinBoxes (QAbstractSpinBox class)
		*/
		PF_SPIN_BOXES = 1 << 0,

		/**
			If set, the wheel events will be blocked for comboBoxes (QComboBox class)
		*/
		PF_COMBO_BOXES = 1 << 1,

		/**
			If set, the processor will search and acquire all child widgets created dynamically
		*/
		PF_COLLECT_CHILDREN = 1 << 2,
	};
	I_DECLARE_FLAGS(ProcessingFlags,
				PF_NONE,
				PF_SPIN_BOXES,
				PF_COMBO_BOXES,
				PF_COLLECT_CHILDREN);

public:
	typedef QObject BaseClass;
	typedef istd::IChangeable BaseClass2;
	typedef QList<QObject*> ObjectsPtrsList;

	CWidgetWheelEventBlocker(int flags = PF_NONE, QObject* parentPtr = nullptr);

	int GetProcessingFlags() const;
	const ObjectsPtrsList GetWatchedWidgets() const;

	void SetProcessingFlags(int flags);
	void AddProcessingFlag(ProcessingFlags flag);

	void AcquireChildScrollArea(QObject& parentObject);

	/**
		\brief Finds and adds all supported widgets depends on current \enum ProcessingFlags.
		\param parent - the widget where searching is required to add its for ignore events.
		\return true - if at least one widget has been found and added otherwise false.
		\sa SetProcessingFlags.
	 */
	bool AddChildrenWidgetsFromParent(QObject& parentObject);

	/**
		\brief Adds all supported widgets ftom the provided list depends on current \enum ProcessingFlags.
		\param widgetPtrsList - the widget list to add for ignore events.
		\return true - if at least one widget has been found and added otherwise false.
		\sa SetProcessingFlags.
	 */
	bool AddWidgets(ObjectsPtrsList& widgetPtrsList);

	// Reimplemented (QObject)
	virtual bool eventFilter(QObject* objectPtr, QEvent* eventPtr) override;

	// Reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool ResetData(CompatibilityMode mode) override;

private:
	/**
		\brief Finds and adds all supported widgets depends on current \enum ProcessingFlags.
		\param parent - the widget where searching is required to add its for ignore events.
		\return true - if at least one widget has been found and added otherwise false.
		\sa SetProcessingFlags.
	 */
	bool AddSupportedChildWidgets(QObject& parentObject);

	/**
		Sets the event filter of the widget to this class and adds it to the watching list.
	*/
	void AcquireWidget(QObject& widget);

private:
	int m_processingFlags;
	ObjectsPtrsList m_watchingWidgetPtrsList;

};


} // namespace iwidgets

