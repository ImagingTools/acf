#ifndef iqt2d_CMultiBitmapViewComp_included
#define iqt2d_CMultiBitmapViewComp_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#else
#include <QtGui/QGroupBox>
#include <QtGui/QBoxLayout>
#include <QtGui/QLabel>
#endif

// ACF includes
#include <istd/TPointerVector.h>
#include <ibase/TModelObserverCompWrap.h>
#include <imod/CMultiModelDispatcherBase.h>
#include <imod/TSingleModelObserverBase.h>
#include <istd/IInformationProvider.h>
#include <iqtgui/TGuiComponentBase.h>
#include <iqtgui/TGuiObserverWrap.h>
#include <iqtgui/TGuiComponentBase.h>
#include <iqt2d/IViewExtender.h>
#include <iqt2d/IViewProvider.h>
#include <iview/CConsoleGui.h>
#include <iimg/IMultiBitmapProvider.h>
#include <iimg/CBitmap.h>


namespace iqt2d
{


class CMultiBitmapViewComp:
			public ibase::TModelObserverCompWrap<
						iqtgui::TGuiObserverWrap<
									iqtgui::TGuiComponentBase<QWidget>, imod::TSingleModelObserverBase<iimg::IMultiBitmapProvider> > >,
			protected imod::CMultiModelDispatcherBase
{
public:
	typedef ibase::TModelObserverCompWrap<
				iqtgui::TGuiObserverWrap<
							iqtgui::TGuiComponentBase<QWidget>,
							imod::TSingleModelObserverBase<iimg::IMultiBitmapProvider> > > BaseClass;

	typedef imod::CMultiModelDispatcherBase BaseClass2;

	I_BEGIN_COMPONENT(CMultiBitmapViewComp);
		I_ASSIGN(m_horizontalViewsAttrPtr, "HorizontalViewsCount", "Number of horizontal views", false, 1);
		I_ASSIGN(m_verticalViewsAttrPtr, "VerticalViewsCount", "Number of vertical views", false, 1);

		I_ASSIGN_MULTI_0(m_viewExtendersCompPtr, "ViewExtenders", "View extenders", false);
		
		I_ASSIGN_MULTI_0(m_informationProvidersCompPtr, "InformationProviders", "Information providers", false);
		I_ASSIGN_TO(m_informationModelsCompPtr, m_informationProvidersCompPtr, true);
		I_ASSIGN(m_generalInformationProviderCompPtr, "GeneralInformationProvider", "General information provider", false, "GeneralInformationProvider");
		I_ASSIGN_TO(m_generalInformationModelCompPtr, m_generalInformationProviderCompPtr, true);
		I_ASSIGN(m_optionsListCompPtr, "OptionsList", "List to provide dynamic descriptions", false, "OptionsList");

		I_ASSIGN_MULTI_0(m_viewLabelPrefixesAttrPtr, "ViewLabelPrefixes", "Prefixes used to title the single bitmap view. If none present, information from InformationProviders will be used.", false);
		I_ASSIGN(m_showStatusLabelAttrPtr, "ShowStatusLabel", "If active then status will be shown in the view's header", true, false);
		I_ASSIGN(m_showStatusBackgroundAttrPtr, "ShowStatusBackground", "If active then status will be shown as the view's background color", true, false);
		I_ASSIGN(m_viewBackgroundColorAttrPtr, "BackgroundColor", "Background color of the console", false, "black");
	I_END_COMPONENT;

protected:
	static const int GeneralStatusModelId = -1;

	static QIcon GetCategoryIcon(istd::IInformationProvider::InformationCategory category);

	class CSingleView: public QGroupBox, public iqt2d::IViewProvider
	{
	public:
		typedef QGroupBox BaseClass;

		CSingleView(QWidget* parentPtr, int id = -1, const QString& title = "");

		void UpdateImage(const iimg::IBitmap* bitmapPtr);

		void SetBackgroundColor(const QColor& color);

		// called once at initialization stage, at this moment the model should be connected already.
		virtual void Init(bool hasStatusLabel, bool hasStatusBackground);
		virtual void SetInspectionResult(int result);

		// reimplemented (iqt2d::IViewProvider)
		virtual int GetViewId() const;
		virtual iview::IShapeView* GetView() const;

	protected:
		// called from Init() to add status labels etc. to the main widget layout (layoutPtr).
		virtual void AddStatusItems(QBoxLayout* layoutPtr);

	protected:
		iview::CImageShape m_backgroundShape;
		imod::TModelWrap<iimg::CBitmap> m_backgroundModel;

		mutable iview::CConsoleGui m_console;
		int m_id;
		bool m_showStatusLabel;
		bool m_showStatusBackground;
	};

	istd::TPointerVector<CSingleView> m_views;

	virtual CSingleView* CreateView(QWidget* parentPtr, int id = -1, const QString& title = "");
	virtual void EnsureViewsCreated();
	virtual QString GetTitleByIndex(int index) const;
	virtual void UpdateInspectionCategory(int index);
	virtual void ResetAllViews();
	virtual void ConnectModels();
	virtual void OnViewCreated(int index, CSingleView* viewPtr);

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags);
	virtual void OnGuiModelAttached();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentDestroyed();

private:
	I_ATTR(int, m_horizontalViewsAttrPtr);
	I_ATTR(int, m_verticalViewsAttrPtr);

	I_REF(istd::IInformationProvider, m_generalInformationProviderCompPtr);
	I_REF(imod::IModel, m_generalInformationModelCompPtr);
	I_MULTIREF(istd::IInformationProvider, m_informationProvidersCompPtr);
	I_MULTIREF(imod::IModel, m_informationModelsCompPtr);
	I_REF(iprm::IOptionsList, m_optionsListCompPtr);

	I_MULTIREF(iqt2d::IViewExtender, m_viewExtendersCompPtr);

	I_MULTIATTR(QString, m_viewLabelPrefixesAttrPtr);
	I_ATTR(bool, m_showStatusLabelAttrPtr);

	I_ATTR(bool, m_showStatusBackgroundAttrPtr);
	I_ATTR(QByteArray, m_viewBackgroundColorAttrPtr);
	
	int m_rowCount;
	int m_columnCount;
	int m_viewCount;
	bool m_dynamicTitles;
};


} // namespace iqt2d


#endif // !iqt2d_CMultiBitmapViewComp_included
