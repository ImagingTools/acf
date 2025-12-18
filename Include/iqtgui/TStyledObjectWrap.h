#pragma once


// ACF includes
#include <iqtgui/iqtgui.h>
#include <iqtgui/CIconProvider.h>
#include <iqt/CDesignThemeEvent.h>

// Qt includes
#include <QtWidgets/QWidget>
#include <QtCore/QCoreApplication>


namespace iqtgui
{

/**
	Wrapper for QWidget (or any QObject in general) which reacts to global style (DesignSchema) change event
	and provides helper methods for icons
 */
template <class Base>
class TStyledObjectWrap: public Base, public CIconProvider
{
	static_assert(std::is_base_of_v<QObject, Base>, "TStyledObjectWrap must wrap a QObject derived class");

	typedef Base BaseClass;

protected:

	template<typename... Args>
	TStyledObjectWrap(Args&&... args)
		: Base(std::forward<Args>(args)...)
	{
		EnableDesignHandler(true);
	}

	virtual void OnDesignChanged() = 0;
	
	void EnableDesignHandler(bool enable)
	{
		auto* app = QCoreApplication::instance();
		if (app != nullptr){
			if (enable){
				app->installEventFilter(this);
			}
			else{
				app->removeEventFilter(this);
			}
		}
	}

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr) override
	{
		if (sourcePtr == QCoreApplication::instance()){
			if (eventPtr->type() == iqt::CDesignThemeEvent::s_eventType){
				Q_ASSERT(dynamic_cast<const iqt::CDesignThemeEvent*>(eventPtr) != nullptr);
				auto* designEventPtr = static_cast<const iqt::CDesignThemeEvent*>(eventPtr);
				
				SetThemeId(designEventPtr->GetThemeId());
				OnDesignChanged();
			}
		}

		return BaseClass::eventFilter(sourcePtr, eventPtr);
	}

};


using StyledWidget = TStyledObjectWrap<QWidget>;


} // namespace iqtgui


