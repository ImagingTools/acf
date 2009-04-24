#include "iqt2d/CImageViewComp.h"


#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>
#include <QMenu>


namespace iqt2d
{


CImageViewComp::CImageViewComp()
{
	SetBackgroundGridUsed();
}


// reimplemented (imod::IModelEditor)

void CImageViewComp::UpdateModel() const
{
}


void CImageViewComp::UpdateEditor(int /*updateFlags*/)
{
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CImageViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QGraphicsScene* scenePtr = GetScene();
	if (scenePtr != NULL){
		scenePtr->addItem(this);
	}
}


void CImageViewComp::OnGuiDestroyed()
{
	QGraphicsScene* scenePtr = GetScene();
	if (scenePtr != NULL){
		scenePtr->removeItem(this);
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqt2d

