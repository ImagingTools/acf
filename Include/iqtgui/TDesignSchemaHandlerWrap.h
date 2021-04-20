#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QCoreApplication>


namespace iqtgui
{


template <class Base>
class TDesignSchemaHandlerWrap: public Base
{
public:
	typedef Base BaseClass;

	TDesignSchemaHandlerWrap();

	/**
		If enabled, listen to the design schema change events from the application context.
	*/
	virtual void EnableDesignHandler(bool enable = true);

protected:
	virtual void OnDesignSchemaChanged() = 0;

private:
	class DesignSchemaEventsFilter: public QObject
	{
	public:
		typedef QObject BaseClass;

		explicit DesignSchemaEventsFilter(TDesignSchemaHandlerWrap& parent);

	protected:
		// reimplemented (QObject)
		bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

	private:
		TDesignSchemaHandlerWrap& m_parent;
	};

	DesignSchemaEventsFilter m_designSchemaEventsFilter;
};


// public methods of class TDesignSchemaHandlerWrap

template <class Base>
TDesignSchemaHandlerWrap<Base>::TDesignSchemaHandlerWrap()
	:m_designSchemaEventsFilter(*this)
{
}


template <class Base>
void TDesignSchemaHandlerWrap<Base>::EnableDesignHandler(bool enable)
{
	QCoreApplication* applicationPtr = QCoreApplication::instance();
	if (applicationPtr != NULL){
		if (enable){
			applicationPtr->installEventFilter(&m_designSchemaEventsFilter);
		}
		else{
			applicationPtr->removeEventFilter(&m_designSchemaEventsFilter);
		}
	}
}


// public methods of the embedded class DesignSchemaEventsFilter

template <class Base>
TDesignSchemaHandlerWrap<Base>::DesignSchemaEventsFilter::DesignSchemaEventsFilter(TDesignSchemaHandlerWrap& parent)
	:m_parent(parent)
{
}


// protected methods of the embedded class DesignSchemaEventsFilter

// reimplemented (QObject)

template <class Base>
bool TDesignSchemaHandlerWrap<Base>::DesignSchemaEventsFilter::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	if (sourcePtr == QCoreApplication::instance()){
		switch (eventPtr->type()){
		case QEvent::QEvent::ApplicationPaletteChange:
			m_parent.OnDesignSchemaChanged();
			break;
		}
	}

	return BaseClass::eventFilter(sourcePtr, eventPtr);
}


} // namespace iqtgui


