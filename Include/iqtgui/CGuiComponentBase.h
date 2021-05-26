#ifndef iqtgui_CGuiComponentBase_included
#define iqtgui_CGuiComponentBase_included


// Qt includes
#include <QtCore/QString>
#include <QtGui/QIcon>

// ACF includes
#include <imod/TModelWrap.h>
#include <icomp/CComponentBase.h>
#include <ibase/TLocalizableWrap.h>
#include <iqtgui/IGuiObject.h>
#include <iqtgui/IVisualStatus.h>
#include <iqtgui/TDesignSchemaHandlerWrap.h>
#include <iqtgui/TMakeIconProviderCompWrap.h>


namespace iqtgui
{


/**
	Base class for classes implementing interface iqtgui::IGuiObject.
*/
class CGuiComponentBase:
			public QObject,
			public TMakeIconProviderCompWrap<
						TDesignSchemaHandlerWrap<
									ibase::TLocalizableWrap<icomp::CComponentBase>>>,
			virtual public IGuiObject
{
public:
	typedef TMakeIconProviderCompWrap<
				TDesignSchemaHandlerWrap<
							ibase::TLocalizableWrap<icomp::CComponentBase>>> BaseClass;

	I_BEGIN_BASE_COMPONENT(CGuiComponentBase);
		I_REGISTER_INTERFACE(IGuiObject);
		I_REGISTER_SUBELEMENT(VisualStatus);
		I_REGISTER_SUBELEMENT_INTERFACE_T(VisualStatus, IVisualStatus, ExtractVisualStatus);
		I_REGISTER_SUBELEMENT_INTERFACE_T(VisualStatus, istd::IChangeable, ExtractVisualStatus);
		I_REGISTER_SUBELEMENT_INTERFACE_T(VisualStatus, imod::IModel, ExtractVisualStatus);
		I_ASSIGN(m_defaultStatusIconPathAttrPtr, "DefaultStatusIcon", "Path of status icon used by default", false, "");
		I_ASSIGN(m_defaultStatusTextAttrPtr, "DefaultStatusText", "Status text used by default", true, "");
		I_ASSIGN(m_styleSheetPathAttrPtr, "StyleSheet", "Path to the style sheet file used for this UI component.", false, "");
		I_ASSIGN(m_objectIdAttrPtr, "ObjectId", "ID of the widget", false, "");
		I_ASSIGN(m_fixedWidthAttrPtr, "FixedWidth", "Fixed width of the widget", false, 300);
		I_ASSIGN(m_fixedHeightAttrPtr, "FixedHeight", "Fixed height of the widget", false, 300);
	I_END_COMPONENT;

	CGuiComponentBase();

	bool IsGuiShown() const;

	// reimplemented (iqtgui::IGuiObject)
	virtual bool IsGuiCreated() const;
	virtual bool CreateGui(QWidget* parentPtr);
	virtual bool DestroyGui();
	virtual QWidget* GetWidget() const;
	virtual void OnTryClose(bool* ignoredPtr = NULL);

#if _MSC_VER < 1500
public:
#else
protected:
#endif
	class VisualStatus: virtual public IVisualStatus
	{
	public:
		// reimplemented (iqtgui::IVisualStatus)
		virtual QIcon GetStatusIcon() const;
		virtual QString GetStatusText() const;

		friend class CGuiComponentBase;

	private:
		QIcon m_statusIcon;
		QString m_statusText;
	};

protected:
	/**
		Called from widget event filter when slave widget is shown.
	*/
	virtual void OnGuiShown();

	/**
		Called from widget event filter when slave widget is hidden.
	*/
	virtual void OnGuiHidden();

	/**
		Called from widget event filter when key is pressed.
		\return	if event was consumed returns true, otherwise false.
	*/
	virtual bool OnKeyPressed(QKeyEvent* event);

	/**
		Called when non-GUI elements (like commands) should be retranslated.
	*/
	virtual void OnRetranslate();

	/**
		Called from widget event filter when GUI should be retranslated.
	*/
	virtual void OnGuiRetranslate();

	/**
		Called just after GUI is initialized.
	*/
	virtual void OnGuiCreated();

	/**
		Called just before GUI is released.
	*/
	virtual void OnGuiDestroyed();

	/**
		Set status icon;
	*/
	void SetStatusIcon(const QIcon& icon);
	/**
		Set status text;
	*/
	void SetStatusText(const QString& text);

	// reimplemented (ibase::TLocalizableWrap)
	virtual void OnLanguageChanged();

	// reimplemented (ibase::TDesignSchemaHandlerWrap)
	virtual void OnDesignSchemaChanged();

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// abstract methods
	/**
		Create slave widget object.
	*/
	virtual QWidget* CreateQtWidget(QWidget* parentPtr) = 0;

	// attributes
	imod::TModelWrap<VisualStatus> m_visualStatus;

private:
	void MakeAutoSlotConnection();

	I_ATTR(QString, m_defaultStatusIconPathAttrPtr);
	I_TEXTATTR(m_defaultStatusTextAttrPtr);
	I_ATTR(QString, m_styleSheetPathAttrPtr);
	I_ATTR(QByteArray, m_objectIdAttrPtr);
	I_ATTR(int, m_fixedWidthAttrPtr);
	I_ATTR(int, m_fixedHeightAttrPtr);

	QWidget* m_widgetPtr;
	bool m_isGuiShown;

	QIcon m_defaultStatusIcon;

	// static template methods for subelement access
	template <class InterfaceType>
	static InterfaceType* ExtractVisualStatus(CGuiComponentBase& component)
	{
		return &component.m_visualStatus;
	}
};


// inline methods

inline bool CGuiComponentBase::IsGuiShown() const
{
	return m_isGuiShown;
}


// reimplemented (iqtgui::IGuiObject)

inline QWidget* CGuiComponentBase::GetWidget() const
{
	return m_widgetPtr;
}


} // namespace iqtgui


#endif // !iqtgui_CGuiComponentBase_included


