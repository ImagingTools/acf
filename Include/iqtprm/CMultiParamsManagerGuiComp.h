#ifndef iqtprm_CMultiParamsManagerGuiComp_included
#define iqtprm_CMultiParamsManagerGuiComp_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QStackedWidget>
#else
#include <QtGui/QStackedWidget>
#endif

// ACF includes
#include <iqt2d/IViewExtender.h>

#include <iqtprm/CParamsManagerGuiCompBase.h>


namespace iqtprm
{


class CMultiParamsManagerGuiComp: public CParamsManagerGuiCompBase
{
public:
	typedef CParamsManagerGuiCompBase BaseClass;

	I_BEGIN_COMPONENT(CMultiParamsManagerGuiComp);
		I_ASSIGN_MULTI_0(m_paramsGuiCompPtr, "ParamsGui", "GUI object used to display parameters", false);
		I_ASSIGN_TO(m_paramsObserverCompPtr, m_paramsGuiCompPtr, false);
		I_ASSIGN_TO(m_viewExtendersCompPtr, m_paramsGuiCompPtr, false);
		I_ASSIGN_MULTI_0(m_paramsSetTypeIdsAttrPtr, "ParamTypeIds", "The list of type IDs for supported parameter sets", true);
	I_END_COMPONENT;

	CMultiParamsManagerGuiComp();

protected:
	// reimplemented (CParamsManagerGuiCompBase)
	virtual imod::IObserver* GetObserverPtr(const iprm::IParamsSet* paramsSetPtr) const;
	virtual iqtgui::IGuiObject* GetEditorGuiPtr(const iprm::IParamsSet* paramsSetPtr) const;
	virtual void UpdateParamsView(int selectedIndex);
	virtual iqt2d::IViewExtender* GetCurrentViewExtenderPtr() const;

	// reimplemented (iqtgui::CComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_MULTIREF(iqtgui::IGuiObject, m_paramsGuiCompPtr);
	I_MULTIREF(imod::IObserver, m_paramsObserverCompPtr);
	I_MULTIREF(iqt2d::IViewExtender, m_viewExtendersCompPtr);
	I_MULTIATTR(QByteArray, m_paramsSetTypeIdsAttrPtr);

	QStackedWidget* m_parameterEditorStackPtr;

	typedef QMap<QWidget*, iqtgui::IGuiObject*> WidgetGuiMap;
	typedef QMap<QByteArray /*parameter type ID*/, int /*Position in the page stack*/> TypeToStackPositionMap;
	typedef QMap<QByteArray /*parameter type ID*/, iqt2d::IViewExtender* /*Corresponding view extender*/> TypeToExtenderMap;
	typedef QMap<iqt2d::IViewProvider*, int> ConnectedSceneFlags; // maps connected scene provider to connection flags

	WidgetGuiMap m_widgetToGuiMap;
	TypeToStackPositionMap m_typeToStackPositionMap;
	TypeToExtenderMap m_typeToExtenderMap;
};


} // namespace iqtprm


#endif // !iqtprm_CMultiParamsManagerGuiComp_included


