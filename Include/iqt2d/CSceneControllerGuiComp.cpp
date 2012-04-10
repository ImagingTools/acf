#include "iqt2d/CSceneControllerGuiComp.h"


// Qt includes
#include <QtCore/qmath.h>
#include <QtGui/QGraphicsView>

// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt2d/ISceneProvider.h"


namespace iqt2d
{


// public methods


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
	I_ASSERT(m_sceneProviderCompPtr.IsValid());
	if (!m_sceneProviderCompPtr.IsValid()){
		return;
	}

	if (		m_sceneRestrictionsCompPtr.IsValid() && 
				m_sceneRestrictionsCompPtr->GetSceneRestrictionFlags() & i2d::ISceneController::SR_SCALE){
			return;
	}

	QGraphicsScene* scenePtr = m_sceneProviderCompPtr->GetScene();
	if (scenePtr != NULL){
		QGraphicsView* viewPtr = scenePtr->views().at(0);

		QMatrix sceneMatrix = viewPtr->matrix();
		double scaleFactor = qPow((double)2, 0.5);
		sceneMatrix.scale(scaleFactor, scaleFactor);

		viewPtr->setMatrix(sceneMatrix);
	}
}


void CSceneControllerGuiComp::OnZoomDecrement()
{
	I_ASSERT(m_sceneProviderCompPtr.IsValid());
	if (!m_sceneProviderCompPtr.IsValid()){
		return;
	}

	if (		m_sceneRestrictionsCompPtr.IsValid() && 
				m_sceneRestrictionsCompPtr->GetSceneRestrictionFlags() & i2d::ISceneController::SR_SCALE){
			return;
	}

	QGraphicsScene* scenePtr = m_sceneProviderCompPtr->GetScene();
	if (scenePtr != NULL){
		QGraphicsView* viewPtr = scenePtr->views().at(0);

		QMatrix sceneMatrix = viewPtr->matrix();
		double scaleFactor = qPow((double)2, -0.5);
		sceneMatrix.scale(scaleFactor, scaleFactor);

		viewPtr->setMatrix(sceneMatrix);
	}
}


void CSceneControllerGuiComp::OnResetView()
{
	I_ASSERT(m_sceneProviderCompPtr.IsValid());
	if (!m_sceneProviderCompPtr.IsValid()){
		return;
	}
	
	if (		m_sceneRestrictionsCompPtr.IsValid() && 
				m_sceneRestrictionsCompPtr->GetSceneRestrictionFlags() != i2d::ISceneController::SR_NONE){
			return;
	}

	QGraphicsScene* scenePtr = m_sceneProviderCompPtr->GetScene();
	if (scenePtr != NULL){
		QGraphicsView* viewPtr = scenePtr->views().at(0);
		viewPtr->resetMatrix();
	}
}


void CSceneControllerGuiComp::OnRotateRight()
{
	I_ASSERT(m_sceneProviderCompPtr.IsValid());
	if (!m_sceneProviderCompPtr.IsValid()){
		return;
	}

	if (		m_sceneRestrictionsCompPtr.IsValid() && 
				m_sceneRestrictionsCompPtr->GetSceneRestrictionFlags() & i2d::ISceneController::SR_ROTATE){
			return;
	}

	QGraphicsScene* scenePtr = m_sceneProviderCompPtr->GetScene();
	if (scenePtr != NULL){
		QGraphicsView* viewPtr = scenePtr->views().at(0);

		QMatrix sceneMatrix = viewPtr->matrix();
		sceneMatrix.rotate(2);

		viewPtr->setMatrix(sceneMatrix);
	}
}


void CSceneControllerGuiComp::OnRotateLeft()
{
	I_ASSERT(m_sceneProviderCompPtr.IsValid());
	if (!m_sceneProviderCompPtr.IsValid()){
		return;
	}

	if (		m_sceneRestrictionsCompPtr.IsValid() && 
				m_sceneRestrictionsCompPtr->GetSceneRestrictionFlags() & i2d::ISceneController::SR_ROTATE){
			return;
	}

	QGraphicsScene* scenePtr = m_sceneProviderCompPtr->GetScene();
	if (scenePtr != NULL){
		QGraphicsView* viewPtr = scenePtr->views().at(0);

		QMatrix sceneMatrix = viewPtr->matrix();
		sceneMatrix.rotate(-2);

		viewPtr->setMatrix(sceneMatrix);
	}
}


} // namespace iqt2d


