#include "iqt2d/CSceneControllerGuiComp.h"


// Qt includes
#include <QGraphicsView>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"

#include "iqt2d/ISceneProvider.h"


namespace iqt2d
{


// public methods

CSceneControllerGuiComp::CSceneControllerGuiComp()
	:m_sceneProviderPtr(NULL)
{
}
	
// reimplemented (iqt2d::ISceneController)

void CSceneControllerGuiComp::RegisterSceneProvider(const ISceneProvider* sceneProviderPtr)
{
	m_sceneProviderPtr = sceneProviderPtr;
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CSceneControllerGuiComp::OnGuiCreated()
{
	connect(ZoomInButton, SIGNAL(clicked()), this, SLOT(OnZoomIncrement()));
	connect(ZoomOutButton, SIGNAL(clicked()), this, SLOT(OnZoomDecrement()));
	connect(ResetViewButton, SIGNAL(clicked()), this, SLOT(OnResetView()));
	connect(RotateRightButton, SIGNAL(clicked()), this, SLOT(OnRotateRight()));
	connect(RotateLeftButton, SIGNAL(clicked()), this, SLOT(OnRotateLeft()));

	BaseClass::OnGuiCreated();
}


void CSceneControllerGuiComp::OnGuiDestroyed()
{
	BaseClass::OnGuiDestroyed();
}


// protected slots

void CSceneControllerGuiComp::OnZoomIncrement()
{
	if (		m_sceneRestrictionsCompPtr.IsValid() && 
				m_sceneRestrictionsCompPtr->GetSceneRestrictionsFlags() & iqt2d::ISceneRestrictions::SR_SCALE){
			return;
	}

	if (m_sceneProviderPtr != NULL){
		QGraphicsScene* scenePtr = m_sceneProviderPtr->GetScene();
		if (scenePtr != NULL){
			QGraphicsView* viewPtr = scenePtr->views().at(0);

			QMatrix sceneMatrix = viewPtr->matrix();
			double scaleFactor = pow((double)2, 0.5);
			sceneMatrix.scale(scaleFactor, scaleFactor);

			viewPtr->setMatrix(sceneMatrix);
		}
	}
}


void CSceneControllerGuiComp::OnZoomDecrement()
{
	if (		m_sceneRestrictionsCompPtr.IsValid() && 
				m_sceneRestrictionsCompPtr->GetSceneRestrictionsFlags() & iqt2d::ISceneRestrictions::SR_SCALE){
			return;
	}

	if (m_sceneProviderPtr != NULL){
		QGraphicsScene* scenePtr = m_sceneProviderPtr->GetScene();
		if (scenePtr != NULL){
			QGraphicsView* viewPtr = scenePtr->views().at(0);

			QMatrix sceneMatrix = viewPtr->matrix();
			double scaleFactor = pow((double)2, -0.5);
			sceneMatrix.scale(scaleFactor, scaleFactor);

			viewPtr->setMatrix(sceneMatrix);
		}
	}
}


void CSceneControllerGuiComp::OnResetView()
{
	if (m_sceneProviderPtr != NULL){
		QGraphicsScene* scenePtr = m_sceneProviderPtr->GetScene();
		if (scenePtr != NULL){
			QGraphicsView* viewPtr = scenePtr->views().at(0);
			viewPtr->resetMatrix();
		}
	}
}


void CSceneControllerGuiComp::OnRotateRight()
{
	if (		m_sceneRestrictionsCompPtr.IsValid() && 
				m_sceneRestrictionsCompPtr->GetSceneRestrictionsFlags() & iqt2d::ISceneRestrictions::SR_ROTATE){
			return;
	}

	if (m_sceneProviderPtr != NULL){
		QGraphicsScene* scenePtr = m_sceneProviderPtr->GetScene();
		if (scenePtr != NULL){
			QGraphicsView* viewPtr = scenePtr->views().at(0);

			QMatrix sceneMatrix = viewPtr->matrix();
			sceneMatrix.rotate(2);

			viewPtr->setMatrix(sceneMatrix);
		}
	}
}


void CSceneControllerGuiComp::OnRotateLeft()
{
	if (		m_sceneRestrictionsCompPtr.IsValid() && 
				m_sceneRestrictionsCompPtr->GetSceneRestrictionsFlags() & iqt2d::ISceneRestrictions::SR_ROTATE){
			return;
	}

	if (m_sceneProviderPtr != NULL){
		QGraphicsScene* scenePtr = m_sceneProviderPtr->GetScene();
		if (scenePtr != NULL){
			QGraphicsView* viewPtr = scenePtr->views().at(0);

			QMatrix sceneMatrix = viewPtr->matrix();
			sceneMatrix.rotate(-2);

			viewPtr->setMatrix(sceneMatrix);
		}
	}
}


} // namespace iqt2d


