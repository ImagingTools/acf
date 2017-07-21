#ifndef iqt2d_TShapeParamsGuiCompBase_included
#define iqt2d_TShapeParamsGuiCompBase_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMenu>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QToolBar>
#else
#include <QtGui/QMenu>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QToolBar>
#endif

// ACF includes
#include <i2d/ICalibration2d.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <iview/IColorSchema.h>
#include <iview/IShapeFactory.h>
#include <iview/CInteractiveShapeBase.h>
#include <iqt2d/TViewExtenderCompBase.h>
#include <iprm/IEnableableParam.h>


namespace iqt2d
{


template <class Ui, class Shape, class ShapeModel>
class TShapeParamsGuiCompBase:
			public iqt2d::TViewExtenderCompBase<
						iqtgui::TDesignerGuiObserverCompBase<Ui, ShapeModel> >,
			virtual public iview::IShapeFactory
{
public:
	typedef iqt2d::TViewExtenderCompBase<iqtgui::TDesignerGuiObserverCompBase<Ui, ShapeModel> > BaseClass;

	I_BEGIN_COMPONENT(TShapeParamsGuiCompBase);
		I_REGISTER_INTERFACE(iview::IShapeFactory);
		I_ASSIGN(m_defaultUnitInfoCompPtr, "DefaultUnitInfo", "Provide default information about the logical value units e.g. mm, this will be used if no unit information found in model", false, "DefaultUnitInfo");
		I_ASSIGN(m_colorSchemaCompPtr, "ShapeColorSchema", "Color schema used by displayed shape", false, "ShapeColorSchema");
		I_ASSIGN(m_fixedPositionAttrPtr, "FixedPosition", "If enabled, the shape position will be not editable", true, false);
		I_ASSIGN(m_fixedPositionParamPtr, "FixedPositionParam", "If set, will be used instead of FixedPosition attribute", false, "FixedPositionParam");
		I_ASSIGN(m_allowToolsMenuAttrPtr, "ShowTools", "Show extended shape edit tools", true, true);
		I_ASSIGN(m_toolBarGuiCompPtr, "ToolBarGui", "Toolbar GUI object to fill with actions", false, "ToolBarGui");
		I_ASSIGN(m_shapeToolTipAttrPtr, "ShapeToolTip", "Tool tip will be show on console", false, "");
	I_END_COMPONENT;

	TShapeParamsGuiCompBase();

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask);
	virtual bool OnModelDetached(imod::IModel* modelPtr);

	// reimplemented (iview::IShapeFactory)
	virtual iview::IShape* CreateShape(const istd::IChangeable* objectPtr, bool connectToModel = false) const;

protected:
	typedef typename BaseClass::Shapes Shapes;
	typedef typename BaseClass::ShapesMap ShapesMap;

	QString GetUnitName() const;
	bool IsPositionFixed() const;
	bool IsToolsMenuAllowed() const;
	QToolBar* GetToolBar() const;

	/**
		Maintenance of the tools actions.
	*/
	virtual void CreateToolsMenu(QAbstractButton* buttonPtr);
	virtual bool PopulateActions(QWidget& host, imod::IModel* modelPtr);
	virtual void OnModelAttachedAndGuiShown(imod::IModel* modelPtr);
	virtual void OnModelDetachedOrGuiHidden(imod::IModel* modelPtr);
	virtual void OnActionTriggered(QAction* actionPtr);

	/**
		Simple creation of shape instance.
	*/
	virtual iview::CInteractiveShapeBase* CreateShapeInstance() const;

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiDestroyed();
	virtual void OnGuiShown();
	virtual void OnGuiHidden();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

private:
	QMenu* m_menuPtr;
	QAbstractButton* m_menuButtonPtr;

	typedef QSet<QAction*> Actions;
	Actions m_toolBarActions;

	I_REF(imath::IUnitInfo, m_defaultUnitInfoCompPtr);
	I_REF(iview::IColorSchema, m_colorSchemaCompPtr);
	I_REF(iprm::IEnableableParam, m_fixedPositionParamPtr);
	I_ATTR(bool, m_fixedPositionAttrPtr);
	I_ATTR(bool, m_allowToolsMenuAttrPtr);
	I_REF(iqtgui::IGuiObject, m_toolBarGuiCompPtr);
	I_TEXTATTR(m_shapeToolTipAttrPtr);
};


// public methods

template <class Ui, class Shape, class ShapeModel>
TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::TShapeParamsGuiCompBase()
:	m_menuPtr(NULL),
	m_menuButtonPtr(NULL)
{

}


// reimplemented (imod::IObserver)

template <class Ui, class Shape, class ShapeModel>
bool TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	if (BaseClass::OnModelAttached(modelPtr, changeMask)){
		const ShapesMap& shapesMap = BaseClass::GetShapesMap();
		for (		typename ShapesMap::const_iterator iter = shapesMap.begin();
					iter != shapesMap.end();
					++iter){
			const Shapes& shapes = iter.value();
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				Shape* shapePtr = dynamic_cast<Shape*>(shapes.GetAt(shapeIndex));
				if (shapePtr != NULL){
					if (modelPtr->AttachObserver(shapePtr)){
						shapePtr->SetVisible(true);
					}
				}
			}
		}

		return true;
	}
	else{
		return false;
	}
}


template <class Ui, class Shape, class ShapeModel>
bool TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::OnModelDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnModelDetached(modelPtr)){
		const ShapesMap& shapesMap = BaseClass::GetShapesMap();
		for (		typename ShapesMap::const_iterator iter = shapesMap.begin();
					iter != shapesMap.end();
					++iter){
			const Shapes& shapes = iter.value();
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				Shape* shapePtr = dynamic_cast<Shape*>(shapes.GetAt(shapeIndex));
				if (shapePtr != NULL){
					if (modelPtr->IsAttached(shapePtr)){
						modelPtr->DetachObserver(shapePtr);
					}

					shapePtr->SetVisible(false);
				}
			}
		}

		return true;
	}
	else{
		return false;
	}
}


// reimplemented (iview::IShapeFactory)

template <class Ui, class Shape, class ShapeModel>
iview::IShape* TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::CreateShape(const istd::IChangeable* objectPtr, bool connectToModel) const
{
	iview::CInteractiveShapeBase* shapePtr = CreateShapeInstance();
	if (shapePtr != NULL){	
		shapePtr->SetEditablePosition(!IsPositionFixed());

		if (m_colorSchemaCompPtr.IsValid()){
			shapePtr->SetUserColorSchema(m_colorSchemaCompPtr.GetPtr());
		}

		if (m_shapeToolTipAttrPtr.IsValid()){
			shapePtr->SetDefaultDescription(*m_shapeToolTipAttrPtr);
		}

		if (BaseClass::IsReadOnly()){
			shapePtr->AssignToLayer(iview::IViewLayer::LT_INACTIVE);
		}

		if (connectToModel){
			imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(const_cast<istd::IChangeable*>(objectPtr));
			if (modelPtr != NULL){
				if (modelPtr->AttachObserver(shapePtr)){
					shapePtr->SetVisible(true);
				}
			}
		}
	}

	return shapePtr;
}


// protected methods

template <class Ui, class Shape, class ShapeModel>
bool TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::IsPositionFixed() const
{
	if (m_fixedPositionParamPtr.IsValid()){
		return m_fixedPositionParamPtr->IsEnabled();
	}

	return *m_fixedPositionAttrPtr;
}


template <class Ui, class Shape, class ShapeModel>
bool TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::IsToolsMenuAllowed() const
{
	return *m_allowToolsMenuAttrPtr;
}


template <class Ui, class Shape, class ShapeModel>
QToolBar* TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::GetToolBar() const
{
	return m_toolBarGuiCompPtr.IsValid() ? dynamic_cast<QToolBar*>(m_toolBarGuiCompPtr->GetWidget()) : NULL;
}


template <class Ui, class Shape, class ShapeModel>
QString TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::GetUnitName() const
{
	const imath::IUnitInfo* unitInfoPtr = NULL;

	const ShapeModel* objectPtr = BaseClass::GetObservedObject();
	if (objectPtr != NULL){
		const i2d::ICalibrationProvider* calibrationProviderPtr = dynamic_cast<const i2d::ICalibrationProvider*>(objectPtr);
		if (calibrationProviderPtr != NULL){
			const i2d::ICalibration2d* calibrationPtr = calibrationProviderPtr->GetCalibration();
			if (calibrationPtr != NULL){
				unitInfoPtr = calibrationPtr->GetArgumentUnitInfo();
			}
		}
	}

	if (unitInfoPtr != NULL){
		return unitInfoPtr->GetUnitName();
	}

	if (m_defaultUnitInfoCompPtr.IsValid()){
		return m_defaultUnitInfoCompPtr->GetUnitName();
	}

	return QString();
}


template <class Ui, class Shape, class ShapeModel>
iview::CInteractiveShapeBase* TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::CreateShapeInstance() const
{
	return new Shape();
}


template <class Ui, class Shape, class ShapeModel>
void TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::CreateToolsMenu(QAbstractButton* buttonPtr)
{
	m_menuButtonPtr = buttonPtr;

	if (buttonPtr == NULL){
		return;
	}

	if (!IsToolsMenuAllowed()){
		buttonPtr->hide();
		return;
	}

	if (m_menuPtr == NULL){
		m_menuPtr = new QMenu(buttonPtr);
		QObject::connect(m_menuPtr, SIGNAL(triggered(QAction*)), this, SLOT(OnActionTriggered(QAction*)));

		QPushButton* pushButtonPtr = dynamic_cast<QPushButton*>(buttonPtr);
		if (pushButtonPtr != NULL){
			pushButtonPtr->setMenu(m_menuPtr);
		}
		else{
			QToolButton* toolButtonPtr = dynamic_cast<QToolButton*>(buttonPtr);
			if (toolButtonPtr != NULL){
				toolButtonPtr->setMenu(m_menuPtr);
			}
		}
	}
}


template <class Ui, class Shape, class ShapeModel>
bool TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::PopulateActions(QWidget& /*host*/, imod::IModel* /*modelPtr*/)
{
	return true;
}


template <class Ui, class Shape, class ShapeModel>
void TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::OnModelAttachedAndGuiShown(imod::IModel* modelPtr)
{
	if (m_menuPtr != NULL){
		m_menuPtr->clear();

		if (PopulateActions(*m_menuPtr, modelPtr)){
			m_menuButtonPtr->setEnabled(true);
		}
		else
			m_menuButtonPtr->setEnabled(false);
	}

	QToolBar* toolBarPtr = GetToolBar();
	if (toolBarPtr != NULL){
		// get actual toolbar actions
		QSet<QAction*> oldActions = toolBarPtr->actions().toSet();

		if (PopulateActions(*toolBarPtr, modelPtr)){
			// store added actions
			QSet<QAction*> newActions = toolBarPtr->actions().toSet();
			m_toolBarActions = newActions - oldActions;

			toolBarPtr->setVisible(true);

			// connect toolbar signals ONLY if there is no menu
			// because of a Qt issue, an action added to both of menu and toolbar will emit actionTriggered() signal twice :(
			if (m_menuPtr == NULL){
				QObject::connect(toolBarPtr, SIGNAL(actionTriggered(QAction*)), this, SLOT(OnActionTriggered(QAction*)));
			}
		}
		else{
			toolBarPtr->disconnect();
		}
	}
}


template <class Ui, class Shape, class ShapeModel>
void TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::OnModelDetachedOrGuiHidden(imod::IModel* /*modelPtr*/)
{
	if (m_menuButtonPtr != NULL){
		m_menuButtonPtr->setEnabled(false);
	}

	QToolBar* toolBarPtr = GetToolBar();
	if (toolBarPtr){
		toolBarPtr->disconnect();

		// depopulate stored actions
		for (Actions::ConstIterator iter = m_toolBarActions.constBegin(); iter != m_toolBarActions.constEnd(); ++iter){
			toolBarPtr->removeAction(*iter);
		}

		m_toolBarActions.clear();
	}
}


// reimplemented (iqt2d::TShapeParamsGuiCompBase)

template <class Ui, class Shape, class ShapeModel>
void TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::OnActionTriggered(QAction* /*actionPtr*/)
{
}


// reimplemented (iqt2d::TViewExtenderCompBase)

template <class Ui, class Shape, class ShapeModel>
void TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::CreateShapes(int /*sceneId*/, Shapes& result)
{
	iview::IShape* shapePtr = CreateShape(BaseClass::GetObservedObject(), true);
	if (shapePtr != NULL){
		result.PushBack(shapePtr);
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

template <class Ui, class Shape, class ShapeModel>
void TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::OnGuiDestroyed()
{
	m_menuButtonPtr = NULL;
	if (m_menuPtr != NULL){
		m_menuPtr->deleteLater();

		m_menuPtr = NULL;
	}

	BaseClass::OnGuiDestroyed();
}


template <class Ui, class Shape, class ShapeModel>
void TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::OnGuiShown()
{
	BaseClass::OnGuiShown();

	OnModelAttachedAndGuiShown(BaseClass::GetObservedModel());
}


template <class Ui, class Shape, class ShapeModel>
void TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::OnGuiHidden()
{
	OnModelDetachedOrGuiHidden(BaseClass::GetObservedModel());

	BaseClass::OnGuiHidden();
}


// reimplemented (iqtgui::TGuiObserverWrap)

template <class Ui, class Shape, class ShapeModel>
void TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	if (BaseClass::IsGuiShown()){
		OnModelAttachedAndGuiShown(BaseClass::GetObservedModel());
	}

	m_fixedPositionParamPtr.EnsureInitialized();
}


template <class Ui, class Shape, class ShapeModel>
void TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::OnGuiModelDetached()
{
	OnModelDetachedOrGuiHidden(BaseClass::GetObservedModel());

	BaseClass::OnGuiModelDetached();
}


} // namespace iview


#endif // !iview_TShapeParamsGuiCompBase_included


