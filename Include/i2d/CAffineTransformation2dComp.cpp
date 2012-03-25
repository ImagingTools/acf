#include "i2d/CAffineTransformation2dComp.h"


namespace i2d
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CAffineTransformation2dComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	CVector2d& translationRef = m_transformation.GetTranslationRef();
	CMatrix2d& matrixRef = m_transformation.GetDeformMatrixRef();

	if (m_m00AttrPtr.IsValid()){
		matrixRef.GetAtRef(0, 0) = *m_m00AttrPtr;
	}

	if (m_m10AttrPtr.IsValid()){
		matrixRef.GetAtRef(1, 0) = *m_m10AttrPtr;
	}

	if (m_m01AttrPtr.IsValid()){
		matrixRef.GetAtRef(0, 1) = *m_m01AttrPtr;
	}

	if (m_m11AttrPtr.IsValid()){
		matrixRef.GetAtRef(1, 1) = *m_m11AttrPtr;
	}

	if (m_translationXAttrPtr.IsValid()){
		translationRef.SetX(*m_translationXAttrPtr);
	}

	if (m_translationYAttrPtr.IsValid()){
		translationRef.SetY(*m_translationYAttrPtr);
	}
}


} // namespace i2d


