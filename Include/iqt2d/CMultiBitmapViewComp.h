#pragma once


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
#include <istd/IInformationProvider.h>
#include <imod/CMultiModelDispatcherBase.h>
#include <imod/TSingleModelObserverBase.h>
#include <iprm/CSelectionParam.h>
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
			public iqtgui::TGuiObserverWrap<
						iqtgui::TGuiComponentBase<QWidget>,
						imod::TSingleModelObserverBase<iimg::IMultiBitmapProvider> >,
			protected imod::CMultiModelDispatcherBase
{
public:
	typedef	iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<QWidget>,
				imod::TSingleModelObserverBase<iimg::IMultiBitmapProvider> > BaseClass;

	typedef imod::CMultiModelDispatcherBase BaseClass2;

	I_BEGIN_COMPONENT(CMultiBitmapViewComp);
		I_REGISTER_SUBELEMENT(ActiveViewIndex);
		I_REGISTER_SUBELEMENT_INTERFACE(ActiveViewIndex, iprm::ISelectionParam, ExtractActiveViewIndex);
		I_ASSIGN(m_horizontalViewsAttrPtr, "HorizontalViewsCount", "Number of horizontal views", false, 1);
		I_ASSIGN(m_verticalViewsAttrPtr, "VerticalViewsCount", "Number of vertical views", false, 1);
		I_ASSIGN(m_verticalViewLayoutAttrPtr, "UseVerticalLayout", "Layout of the single view", true, true);
		I_ASSIGN(m_viewExtenderCompPtr, "ViewExtender", "View extender", false, "ViewExtender");
		I_ASSIGN(m_generalInformationProviderCompPtr, "GeneralInformationProvider", "General information provider", false, "GeneralInformationProvider");
		I_ASSIGN_TO(m_generalInformationModelCompPtr, m_generalInformationProviderCompPtr, true);
		I_ASSIGN(m_viewBackgroundColorAttrPtr, "BackgroundColor", "Background color of the console", false, "black");
	I_END_COMPONENT;

protected:
	static const int GeneralStatusModelId = -1;

	static QIcon GetCategoryIcon(istd::IInformationProvider::InformationCategory category);

	class CSingleView: public QGroupBox, public iqt2d::IViewProvider
	{
	public:
		typedef QGroupBox BaseClass;

		CSingleView(QWidget* parentPtr, int id, const QString& title, const QByteArray& bitmapId);

		void UpdateImage(const iimg::IBitmap* bitmapPtr);

		void SetBackgroundColor(const QColor& color);

		// called once at initialization stage, at this moment the model should be connected already.
		virtual void Init(bool hasStatusLabel, bool hasStatusBackground, bool useVerticalLayout);
		virtual void SetInspectionResult(int result);

		// reimplemented (iqt2d::IViewProvider)
		virtual int GetViewId() const override;
		virtual iview::IShapeView* GetView() const override;

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

		QByteArray m_bitmapId;
	};

	istd::TPointerVector<CSingleView> m_views;

	virtual CSingleView* CreateView(QWidget* parentPtr, int id, const QString& title, const QByteArray& bitmapId);
	virtual void EnsureViewsCreated();
	virtual QString GetTitleByIndex(int index) const;
	virtual void UpdateInspectionCategory(int index);
	virtual void ResetAllViews();
	virtual void ConnectModels();
	virtual void OnViewCreated(int index, CSingleView* viewPtr);

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;
	virtual void OnGuiModelAttached() override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentDestroyed() override;

private:
	// static template methods for subelement access
	template <class InterfaceType>
	static InterfaceType* ExtractActiveViewIndex(CMultiBitmapViewComp& component)
	{
		return &component.m_activeViewIndex;
	}

protected:
	I_ATTR(int, m_horizontalViewsAttrPtr);
	I_ATTR(int, m_verticalViewsAttrPtr);
	I_ATTR(bool, m_verticalViewLayoutAttrPtr);
	I_REF(istd::IInformationProvider, m_generalInformationProviderCompPtr);
	I_REF(imod::IModel, m_generalInformationModelCompPtr);
	I_REF(iqt2d::IViewExtender, m_viewExtenderCompPtr);
	I_ATTR(QByteArray, m_viewBackgroundColorAttrPtr);
	
	int m_rowCount;
	int m_columnCount;
	int m_viewCount;

	iprm::CSelectionParam m_activeViewIndex;
};


} // namespace iqt2d
