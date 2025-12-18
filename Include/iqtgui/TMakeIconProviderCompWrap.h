#pragma once


// ACF includes
#include <iqtgui/iqtgui.h>
#include <iqtgui/TDesignSchemaHandlerWrap.h>
#include <iqtgui/CIconProvider.h>


namespace iqtgui
{


template <class Base>
class TMakeIconProviderCompWrap: public TDesignSchemaHandlerWrap<Base>, public CIconProvider
{
public:
	typedef Base BaseClass;

	// reimplemented (ibase::TDesignSchemaHandlerWrap)
	virtual void OnDesignSchemaChanged(const QByteArray& themeId) override;
};


// reimplemented (ibase::TDesignSchemaHandlerWrap)

template <class Base>
void TMakeIconProviderCompWrap<Base>::OnDesignSchemaChanged(const QByteArray& themeId)
{
	SetThemeId(themeId);
}


} // namespace iqtgui


