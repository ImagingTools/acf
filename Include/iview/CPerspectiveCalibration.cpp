#include "iview/CPerspectiveCalibration.h"


// STL includes
#include <math.h>

// ACF includes
#include "imath/CVarMatrix.h"

#include "ibase/CSize.h"


namespace iview
{


CPerspectiveCalibration::CPerspectiveCalibration()
{
	Reset();
}


void CPerspectiveCalibration::Reset()
{
	ResetAxes();

	SetCameraParams(i2d::CVector2d(0, 0), i2d::CVector2d(0.001, 0.001));

	SetBounds(i2d::CRectangle(-1000, -1000, 3000, 3000));
}


void CPerspectiveCalibration::SetCameraParams(const i2d::CVector2d& viewOpticalCenter, const i2d::CVector2d& pixelAngleTangents)
{
	m_viewCenter = viewOpticalCenter;
	m_pixelAngleTangents = pixelAngleTangents;
}


void CPerspectiveCalibration::ResetAxes()
{
	m_logAxisX = i3d::CVector3d(1, 0, 0);
	m_logAxisY = i3d::CVector3d(0, 1, 0);
	m_logCenter = i3d::CVector3d(0, 0, 1000);
}


void CPerspectiveCalibration::ApplyLeft(const i2d::CAffine2d& transform)
{
	const i2d::CMatrix2d& deform = transform.GetDeformMatrix();

	i2d::CVector2d difference = transform.GetApply(m_viewCenter) - m_viewCenter;

	i2d::CVector2d logPos(m_logCenter.GetX(), m_logCenter.GetY());

	i2d::CVector2d translation(
					difference.GetX() * m_logCenter.GetZ() * m_pixelAngleTangents.GetX(),
					difference.GetY() * m_logCenter.GetZ() * m_pixelAngleTangents.GetY());

	logPos = translation + deform.GetMultiplied(logPos);

	m_logCenter.SetX(logPos.GetX());
	m_logCenter.SetY(logPos.GetY());

	logPos = i2d::CVector2d(m_logAxisX.GetX(), m_logAxisX.GetY());

	translation = i2d::CVector2d(
					difference.GetX() * m_logAxisX.GetZ() * m_pixelAngleTangents.GetX(),
					difference.GetY() * m_logAxisX.GetZ() * m_pixelAngleTangents.GetY());

	logPos = translation + deform.GetMultiplied(logPos);

	m_logAxisX.SetX(logPos.GetX());
	m_logAxisX.SetY(logPos.GetY());

	logPos = i2d::CVector2d(m_logAxisY.GetX(), m_logAxisY.GetY());

	translation = i2d::CVector2d(
					difference.GetX() * m_logAxisY.GetZ() * m_pixelAngleTangents.GetX(),
					difference.GetY() * m_logAxisY.GetZ() * m_pixelAngleTangents.GetY());

	logPos = translation + deform.GetMultiplied(logPos);

	m_logAxisY.SetX(logPos.GetX());
	m_logAxisY.SetY(logPos.GetY());
}


bool CPerspectiveCalibration::DoCalibration(const i2d::CVector2d* logicalPoints, const i2d::CVector2d* viewPoints, int pointsCount)
{
	if (pointsCount > 3){
		ResetAxes();

		i3d::CVector3d newAxisX;
		i3d::CVector3d newAxisY;
		i3d::CVector3d newCenter;

		if (!CalcAllAxes(logicalPoints, viewPoints, pointsCount, newAxisX, newAxisY, newCenter)){
			return false;
		}
		m_logAxisX = newAxisX;
		m_logAxisY = newAxisY;
		m_logCenter = newCenter;

		return true;
	}

	return false;
}


bool CPerspectiveCalibration::GetLogDerivative(
				const i2d::CVector2d& viewPosition,
				const i2d::CVector2d& viewDirection,
				double& result) const
{
	i2d::CAffine2d localTransform;

	if (GetLocalTransform(viewPosition, localTransform)){
		i2d::CMatrix2d transposed = localTransform.GetDeformMatrixRef().GetTransposed();

		result = transposed.GetMultiplied(viewDirection).GetLength();

		return true;
	}
	
	return false;
}


bool CPerspectiveCalibration::operator==(const CPerspectiveCalibration& calib) const
{
	return			(m_logAxisX == calib.m_logAxisX) &&
					(m_logAxisY == calib.m_logAxisY) &&
					(m_logCenter == calib.m_logCenter) &&
					(m_bounds == calib.m_bounds) &&
					(m_viewCenter == calib.m_viewCenter) &&
					(m_pixelAngleTangents == calib.m_pixelAngleTangents);
}


// reimplemented (i2d::ITransformation2d)

int CPerspectiveCalibration::GetTransformationFlags() const
{
	return TF_FORWARD | TF_INJECTIVE | TF_SURJECTIVE | TF_CONTINUES; 
}


bool CPerspectiveCalibration::GetDistance(
				const i2d::CVector2d& origPos1,
				const i2d::CVector2d& origPos2,
				double& result,
				ExactnessMode /*mode*/) const
{
	i2d::CVector2d log1;
	i2d::CVector2d log2;

	if (		GetPositionAt(origPos1, log1) &&
				GetPositionAt(origPos2, log2)){
		result = log1.GetDistance(log2);

		return true;
	}

	return false;
}


bool CPerspectiveCalibration::GetPositionAt(
			const i2d::CVector2d& origPosition,
			i2d::CVector2d& result,
			ExactnessMode /*mode*/) const
{
	i2d::CVector2d normPosition = GetNormPosition(origPosition);

	//	using determinant method to solve equation
	//		Ab = -C,
	//	where:
	//			[X_x Y_x -p_x]
	//		A =	[X_y Y_y -p_y],	b = [l_x, l_y, l_z]^T
	//			[X_z Y_z  -1 ]
	//
	//	X - m_logAxisX, Y - m_logAxisY, p - normPosition, C - m_logCenter
	double detM =	m_logAxisX.GetX() * normPosition.GetY() * m_logAxisY.GetZ() +
					m_logAxisY.GetX() * m_logAxisX.GetY() +
					normPosition.GetX() * m_logAxisY.GetY() * m_logAxisX.GetZ() -
					m_logAxisX.GetX() * m_logAxisY.GetY() -
					m_logAxisY.GetX() * normPosition.GetY() * m_logAxisX.GetZ() -
					normPosition.GetX() * m_logAxisX.GetY() * m_logAxisY.GetZ();

	double detX =	m_logAxisY.GetX() * normPosition.GetY() * m_logCenter.GetZ() +
						m_logAxisY.GetY() *					 m_logCenter.GetX() +
						m_logAxisY.GetZ() * normPosition.GetX() * m_logCenter.GetY() -
						m_logAxisY.GetX() *					 m_logCenter.GetY() -
						m_logAxisY.GetY() * normPosition.GetX() * m_logCenter.GetZ() -
						m_logAxisY.GetZ() * normPosition.GetY() * m_logCenter.GetX();
	
	double detY =	m_logAxisX.GetX() *					 m_logCenter.GetY() +
						m_logAxisX.GetY() * normPosition.GetX() * m_logCenter.GetZ() +
						m_logAxisX.GetZ() * normPosition.GetY() * m_logCenter.GetX() -
						m_logAxisX.GetX() * normPosition.GetY() * m_logCenter.GetZ() -
						m_logAxisX.GetY() *					 m_logCenter.GetX() -
						m_logAxisX.GetZ() * normPosition.GetX() * m_logCenter.GetY();


	result.SetX(detX / detM);
	result.SetY(detY / detM);

	if (m_bounds.Contains(result)){
		return true;
	}
	
	return false;
}


bool CPerspectiveCalibration::GetInvPositionAt(
			const i2d::CVector2d& transfPosition,
			i2d::CVector2d& result,
			ExactnessMode /*mode*/) const
{
	i3d::CVector3d pos3d = m_logAxisX * transfPosition.GetX() + m_logAxisY * transfPosition.GetY() + m_logCenter;

	if (pos3d.GetZ() > I_EPSILON){
		i2d::CVector2d normPosition(pos3d.GetX() / pos3d.GetZ(), pos3d.GetY() / pos3d.GetZ());

		result = GetViewPosition(normPosition);

		if (m_bounds.Contains(transfPosition)){
			return true;
		}
	}

	return false;
}


bool CPerspectiveCalibration::GetLocalTransform(
			const i2d::CVector2d& origPosition,
			i2d::CAffine2d& result,
			ExactnessMode /*mode*/) const
{
	i3d::CVector3d pos3d = m_logAxisX * origPosition.GetX() + m_logAxisY * origPosition.GetY() + m_logCenter;

	if (pos3d.GetZ() > I_EPSILON){
		i2d::CVector2d normPosition(pos3d.GetX() / pos3d.GetZ(), pos3d.GetY() / pos3d.GetZ());

		if (pos3d.GetZ() <= I_EPSILON){
			return false;
		}

		i2d::CMatrix2d resultMatrix;
		resultMatrix.SetAt(0,0, (m_logAxisX.GetX() - normPosition.GetX() * m_logAxisX.GetZ()) / (pos3d.GetZ() * m_pixelAngleTangents.GetX()));
		resultMatrix.SetAt(0,1, (m_logAxisY.GetX() - normPosition.GetX() * m_logAxisY.GetZ()) / (pos3d.GetZ() * m_pixelAngleTangents.GetX()));
		resultMatrix.SetAt(1,0, (m_logAxisX.GetY() - normPosition.GetY() * m_logAxisX.GetZ()) / (pos3d.GetZ() * m_pixelAngleTangents.GetY()));
		resultMatrix.SetAt(1,1, (m_logAxisY.GetY() - normPosition.GetY() * m_logAxisY.GetZ()) / (pos3d.GetZ() * m_pixelAngleTangents.GetY()));
		
		result.SetDeformMatrix(resultMatrix);
		result.SetTranslation(i2d::CVector2d(0, 0));

		if (m_bounds.Contains(origPosition)){
			return true;
		}
	}

	return false;
}


bool CPerspectiveCalibration::GetLocalInvTransform(
			const i2d::CVector2d& transfPosition,
			i2d::CAffine2d& result,
			ExactnessMode /*mode*/) const
{
	i2d::CVector2d logPosition;
	bool status = GetPositionAt(transfPosition, logPosition);
	if (status != false){
		i2d::CAffine2d localTransform;

		GetLocalTransform(logPosition, localTransform);
		
		result.SetDeformMatrix(localTransform.GetDeformMatrix().GetInverted());
		result.SetTranslation(i2d::CVector2d(0, 0));
	}

	return status;
}


const i2d::ITransformation2d* CPerspectiveCalibration::CreateCombinedTransformation(const i2d::ITransformation2d& /*transform*/) const
{
	return NULL;
}


// reimplemented (imath::TISurjectFunction)

bool CPerspectiveCalibration::GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetInvPositionAt(argument, result);
}


i2d::CVector2d CPerspectiveCalibration::GetInvValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d retVal;
	
	GetInvValueAt(argument, retVal);

	return retVal;
}


// reimplemented (imath::TIMathFunction)

bool CPerspectiveCalibration::GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetPositionAt(argument, result);
}


i2d::CVector2d CPerspectiveCalibration::GetValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d retVal;
	
	GetValueAt(argument, retVal);

	return retVal;
}
	
	
// reimplemented (iser::ISerializable)

bool CPerspectiveCalibration::Serialize(iser::IArchive& archive)
{
	bool result = m_logAxisX.Serialize(archive);
	result = result && m_logAxisY.Serialize(archive);
	result = result && m_logCenter.Serialize(archive);

	result = result && m_bounds.Serialize(archive);

	result = result && m_viewCenter.Serialize(archive);
	result = result && m_pixelAngleTangents.Serialize(archive);

	return result;
}


// protected methods

// TODO: remove calc methods, you dont use

bool CPerspectiveCalibration::CalcAxes2d(
				const i2d::CVector2d* logicalPoints,
				const i2d::CVector2d* viewPoints,
				int pointsCount,
				i3d::CVector3d& newAxisX,
				i3d::CVector3d& newAxisY,
				i3d::CVector3d& newCenter)
{
	imath::CVarMatrix matrixA(ibase::CSize(4, pointsCount * 2));
	imath::CVarMatrix matrixY(ibase::CSize(1, pointsCount * 2));

	for (int elementIndex = 0; elementIndex < pointsCount; ++elementIndex){
		const i2d::CVector2d& logPos = logicalPoints[elementIndex];
		i2d::CVector2d normPos = GetNormPosition(viewPoints[elementIndex]);

		double value;

		value =			logPos.GetX() * (normPos.GetX() * m_logAxisX.GetZ() - m_logAxisX.GetX()) +
						logPos.GetY() * (normPos.GetX() * m_logAxisY.GetZ() - m_logAxisY.GetX());
		matrixA.SetAt(istd::CIndex2d(0, elementIndex * 2), value);

		value =			logPos.GetX() * (normPos.GetX() * m_logAxisY.GetZ() - m_logAxisY.GetX()) -
						logPos.GetY() * (normPos.GetX() * m_logAxisX.GetZ() - m_logAxisX.GetX());
		matrixA.SetAt(istd::CIndex2d(1, elementIndex * 2), value);

		value = normPos.GetX() * m_logAxisX.GetZ() - m_logAxisX.GetX();
		matrixA.SetAt(istd::CIndex2d(2, elementIndex * 2), value);

		value = normPos.GetX() * m_logAxisY.GetZ() - m_logAxisY.GetX();
		matrixA.SetAt(istd::CIndex2d(3, elementIndex * 2), value);

		value = m_logCenter.GetX() - normPos.GetX() * m_logCenter.GetZ();
		matrixY.SetAt(istd::CIndex2d(0, elementIndex * 2), value);

		value =			logPos.GetX() * (normPos.GetY() * m_logAxisX.GetZ() - m_logAxisX.GetY()) +
						logPos.GetY() * (normPos.GetY() * m_logAxisY.GetZ() - m_logAxisY.GetY());
		matrixA.SetAt(istd::CIndex2d(0, elementIndex * 2 + 1), value);

		value =			logPos.GetX() * (normPos.GetY() * m_logAxisY.GetZ() - m_logAxisY.GetY()) -
						logPos.GetY() * (normPos.GetY() * m_logAxisX.GetZ() - m_logAxisX.GetY());
		matrixA.SetAt(istd::CIndex2d(1, elementIndex * 2 + 1), value);

		value = normPos.GetY() * m_logAxisX.GetZ() - m_logAxisX.GetY();
		matrixA.SetAt(istd::CIndex2d(2, elementIndex * 2 + 1), value);

		value = normPos.GetY() * m_logAxisY.GetZ() - m_logAxisY.GetY();
		matrixA.SetAt(istd::CIndex2d(3, elementIndex * 2 + 1), value);

		value = m_logCenter.GetY() - normPos.GetY() * m_logCenter.GetZ();
		matrixY.SetAt(istd::CIndex2d(0, elementIndex * 2 + 1), value);
	}

	imath::CVarMatrix coeff;
	if (matrixA.GetSolvedLSP(matrixY, coeff)){
		I_ASSERT(coeff.GetSizes().GetAt(0) == 4);

		i2d::CVector2d rotation(coeff[istd::CIndex2d(0, 0)], coeff[istd::CIndex2d(0, 1)]);
		i2d::CVector2d shift(coeff[istd::CIndex2d(0, 2)], coeff[istd::CIndex2d(0, 3)]);
		double normalization = rotation.GetLength() * ::sqrt(m_logAxisX.GetLength() * m_logAxisY.GetLength());
		rotation /= normalization;
		shift /= normalization;

		newAxisX = m_logAxisX * rotation.GetX() + m_logAxisY * rotation.GetY();
		newAxisY = m_logAxisY * rotation.GetX() - m_logAxisX * rotation.GetY();
		newCenter = m_logAxisX * shift.GetX() + m_logAxisY * shift.GetY() + m_logCenter / normalization;

		return true;
	}

	return false;
}


bool CPerspectiveCalibration::CalcRotationOY(
				const i2d::CVector2d* logicalPoints,
				const i2d::CVector2d* viewPoints,
				int pointsCount,
				i3d::CVector3d& newAxisX,
				i3d::CVector3d& newAxisY,
				i3d::CVector3d& newCenter)
{
	imath::CVarMatrix matrixA(ibase::CSize(4, pointsCount * 2));
	imath::CVarMatrix matrixY(ibase::CSize(1, pointsCount * 2));

	for (int elementIndex = 0; elementIndex < pointsCount; ++elementIndex){
		const i2d::CVector2d& logPos = logicalPoints[elementIndex];
		i2d::CVector2d normPos = GetNormPosition(viewPoints[elementIndex]);

		double value;

		value =			normPos.GetX() * (logPos.GetX() * m_logAxisX.GetZ() + logPos.GetY() * m_logAxisY.GetZ() + m_logCenter.GetZ()) -
						logPos.GetX() * m_logAxisX.GetX() - logPos.GetY() * m_logAxisY.GetX() - m_logCenter.GetX();
		matrixA.SetAt(istd::CIndex2d(0, elementIndex * 2), value);

		value =			normPos.GetX() * (logPos.GetX() * m_logAxisX.GetX() + logPos.GetY() * m_logAxisY.GetX() + m_logCenter.GetX()) +
						logPos.GetX() * m_logAxisX.GetZ() + logPos.GetY() * m_logAxisY.GetZ() + m_logCenter.GetZ();
		matrixA.SetAt(istd::CIndex2d(1, elementIndex * 2), value);

		matrixA.SetAt(istd::CIndex2d(2, elementIndex * 2), -1);

		value = normPos.GetX();
		matrixA.SetAt(istd::CIndex2d(3, elementIndex * 2), value);

		matrixY.SetAt(istd::CIndex2d(0, elementIndex * 2), 0);

		value = normPos.GetY() * (logPos.GetX() * m_logAxisX.GetZ() + logPos.GetY() * m_logAxisY.GetZ() + m_logCenter.GetZ());
		matrixA.SetAt(istd::CIndex2d(0, elementIndex * 2 + 1), value);

		value = normPos.GetY() * (logPos.GetX() * m_logAxisX.GetX() + logPos.GetY() * m_logAxisY.GetX() + m_logCenter.GetX());
		matrixA.SetAt(istd::CIndex2d(1, elementIndex * 2 + 1), value);

		matrixA.SetAt(istd::CIndex2d(2, elementIndex * 2 + 1), 0);

		value = normPos.GetY();
		matrixA.SetAt(istd::CIndex2d(3, elementIndex * 2 + 1), value);

		value = logPos.GetX() * m_logAxisX.GetY() + logPos.GetY() * m_logAxisY.GetY() + m_logCenter.GetY();
		matrixY.SetAt(istd::CIndex2d(0, elementIndex * 2 + 1), value);
	}

	imath::CVarMatrix coeff;
	if (matrixA.GetSolvedLSP(matrixY, coeff)){
		I_ASSERT(coeff.GetSizes().GetAt(0) == 4);

		i2d::CVector2d rotation(coeff[istd::CIndex2d(0, 0)], coeff[istd::CIndex2d(0, 1)]);
		i2d::CVector2d shift(coeff[istd::CIndex2d(0, 2)], coeff[istd::CIndex2d(0, 3)]);
		double normalization = rotation.GetLength() * ::sqrt(m_logAxisX.GetLength() * m_logAxisY.GetLength());
		rotation /= normalization;
		shift /= normalization;

		newAxisX.SetX(		m_logAxisX.GetX() * rotation.GetX() -
						m_logAxisX.GetZ() * rotation.GetY());
		newAxisX.SetZ(		m_logAxisX.GetX() * rotation.GetY() +
						m_logAxisX.GetZ() * rotation.GetX());
		newAxisY.SetX(		m_logAxisY.GetX() * rotation.GetX() -
						m_logAxisY.GetZ() * rotation.GetY());
		newAxisY.SetZ(		m_logAxisY.GetX() * rotation.GetY() +
						m_logAxisY.GetZ() * rotation.GetX());
		newCenter.SetX(	m_logCenter.GetX() * rotation.GetX() -
						m_logCenter.GetZ() * rotation.GetY() +
						shift.GetX());
		newCenter.SetZ(	m_logCenter.GetX() * rotation.GetY() +
						m_logCenter.GetZ() * rotation.GetX() +
						shift.GetY());

		return true;
	}

	return false;
}


bool CPerspectiveCalibration::CalcRotationOX(
				const i2d::CVector2d* logicalPoints,
				const i2d::CVector2d* viewPoints,
				int pointsCount,
				i3d::CVector3d& newAxisX,
				i3d::CVector3d& newAxisY,
				i3d::CVector3d& newCenter)
{
	imath::CVarMatrix matrixA(ibase::CSize(4, pointsCount * 2));
	imath::CVarMatrix matrixY(ibase::CSize(1, pointsCount * 2));

	for (int elementIndex = 0; elementIndex < pointsCount; ++elementIndex){
		const i2d::CVector2d& logPos = logicalPoints[elementIndex];
		i2d::CVector2d normPos = GetNormPosition(viewPoints[elementIndex]);

		double value;

		value =			normPos.GetY() * (logPos.GetX() * m_logAxisX.GetZ() + logPos.GetY() * m_logAxisY.GetZ() + m_logCenter.GetZ()) -
						logPos.GetX() * m_logAxisX.GetY() - logPos.GetY() * m_logAxisY.GetY() - m_logCenter.GetY();
		matrixA.SetAt(istd::CIndex2d(0, elementIndex * 2), value);

		value =			normPos.GetY() * (logPos.GetX() * m_logAxisX.GetY() + logPos.GetY() * m_logAxisY.GetY() + m_logCenter.GetY()) +
						logPos.GetX() * m_logAxisX.GetZ() + logPos.GetY() * m_logAxisY.GetZ() + m_logCenter.GetZ();
		matrixA.SetAt(istd::CIndex2d(1, elementIndex * 2), value);

		matrixA.SetAt(istd::CIndex2d(2, elementIndex * 2), -1);

		value = normPos.GetY();
		matrixA.SetAt(istd::CIndex2d(3, elementIndex * 2), value);

		matrixY.SetAt(istd::CIndex2d(0, elementIndex * 2), 0);

		value = normPos.GetX() * (logPos.GetX() * m_logAxisX.GetZ() + logPos.GetY() * m_logAxisY.GetZ() + m_logCenter.GetZ());
		matrixA.SetAt(istd::CIndex2d(0, elementIndex * 2 + 1), value);

		value = normPos.GetX() * (logPos.GetX() * m_logAxisX.GetY() + logPos.GetY() * m_logAxisY.GetY() + m_logCenter.GetY());
		matrixA.SetAt(istd::CIndex2d(1, elementIndex * 2 + 1), value);

		matrixA.SetAt(istd::CIndex2d(2, elementIndex * 2 + 1), 0);

		value = normPos.GetX();
		matrixA.SetAt(istd::CIndex2d(3, elementIndex * 2 + 1), value);

		value = logPos.GetX() * m_logAxisX.GetX() + logPos.GetY() * m_logAxisY.GetX() + m_logCenter.GetX();
		matrixY.SetAt(istd::CIndex2d(0, elementIndex * 2 + 1), value);
	}

	imath::CVarMatrix coeff;
	if (matrixA.GetSolvedLSP(matrixY, coeff)){
		I_ASSERT(coeff.GetSizes().GetAt(0) == 4);

		i2d::CVector2d rotation(coeff[istd::CIndex2d(0, 0)], coeff[istd::CIndex2d(0, 1)]);
		i2d::CVector2d shift(coeff[istd::CIndex2d(0, 2)], coeff[istd::CIndex2d(0, 3)]);
		double normalization = rotation.GetLength() * ::sqrt(m_logAxisX.GetLength() * m_logAxisY.GetLength());
		rotation /= normalization;
		shift /= normalization;

		newAxisX.SetY(		m_logAxisX.GetY() * rotation.GetX() -
						m_logAxisX.GetZ() * rotation.GetY());
		newAxisX.SetZ(		m_logAxisX.GetY() * rotation.GetY() +
						m_logAxisX.GetZ() * rotation.GetX());
		newAxisY.SetY(		m_logAxisY.GetY() * rotation.GetX() -
						m_logAxisY.GetZ() * rotation.GetY());
		newAxisY.SetZ(		m_logAxisY.GetY() * rotation.GetY() +
						m_logAxisY.GetZ() * rotation.GetX());
		newCenter.SetY(	m_logCenter.GetY() * rotation.GetX() -
						m_logCenter.GetZ() * rotation.GetY() +
						shift.GetX());
		newCenter.SetZ(	m_logCenter.GetY() * rotation.GetY() +
						m_logCenter.GetZ() * rotation.GetX() +
						shift.GetY());

		return true;
	}

	return false;
}


bool CPerspectiveCalibration::CalcAllAxes(
				const i2d::CVector2d* logicalPoints,
				const i2d::CVector2d* viewPoints,
				int pointsCount,
				i3d::CVector3d& newAxisX,
				i3d::CVector3d& newAxisY,
				i3d::CVector3d& newCenter)
{
	imath::CVarMatrix matrixA(ibase::CSize(8, pointsCount * 2));
	imath::CVarMatrix matrixY(ibase::CSize(1, pointsCount * 2));

	for (int elementIndex = 0; elementIndex < pointsCount; ++elementIndex){
		const i2d::CVector2d& logPos = logicalPoints[elementIndex];
		i2d::CVector2d normPos = GetNormPosition(viewPoints[elementIndex]);

		matrixA.SetAt(istd::CIndex2d(0, elementIndex * 2), logPos.GetX());
		matrixA.SetAt(istd::CIndex2d(1, elementIndex * 2), 0);
		matrixA.SetAt(istd::CIndex2d(2, elementIndex * 2), -normPos.GetX() * logPos.GetX());
		matrixA.SetAt(istd::CIndex2d(3, elementIndex * 2), logPos.GetY());
		matrixA.SetAt(istd::CIndex2d(4, elementIndex * 2), 0);
		matrixA.SetAt(istd::CIndex2d(5, elementIndex * 2), -normPos.GetX() * logPos.GetY());
		matrixA.SetAt(istd::CIndex2d(6, elementIndex * 2), 1);
		matrixA.SetAt(istd::CIndex2d(7, elementIndex * 2), 0);
		matrixY.SetAt(istd::CIndex2d(0, elementIndex * 2), normPos.GetX());

		matrixA.SetAt(istd::CIndex2d(0, elementIndex * 2 + 1), 0);
		matrixA.SetAt(istd::CIndex2d(1, elementIndex * 2 + 1), logPos.GetX());
		matrixA.SetAt(istd::CIndex2d(2, elementIndex * 2 + 1), -normPos.GetY() * logPos.GetX());
		matrixA.SetAt(istd::CIndex2d(3, elementIndex * 2 + 1), 0);
		matrixA.SetAt(istd::CIndex2d(4, elementIndex * 2 + 1), logPos.GetY());
		matrixA.SetAt(istd::CIndex2d(5, elementIndex * 2 + 1), -normPos.GetY() * logPos.GetY());
		matrixA.SetAt(istd::CIndex2d(6, elementIndex * 2 + 1), 0);
		matrixA.SetAt(istd::CIndex2d(7, elementIndex * 2 + 1), 1);
		matrixY.SetAt(istd::CIndex2d(0, elementIndex * 2 + 1), normPos.GetY());
	}

	imath::CVarMatrix coeff;
	if (matrixA.GetSolvedLSP(matrixY, coeff)){
		I_ASSERT(coeff.GetSizes().GetAt(0) == 8);

		newAxisX.SetX(coeff.GetAt(istd::CIndex2d(0, 0)));
		newAxisX.SetY(coeff.GetAt(istd::CIndex2d(0, 1)));
		newAxisX.SetZ(coeff.GetAt(istd::CIndex2d(0, 2)));

		newAxisY.SetX(coeff.GetAt(istd::CIndex2d(0, 3)));
		newAxisY.SetY(coeff.GetAt(istd::CIndex2d(0, 4)));
		newAxisY.SetZ(coeff.GetAt(istd::CIndex2d(0, 5)));

		newCenter.SetX(coeff.GetAt(istd::CIndex2d(0, 6)));
		newCenter.SetY(coeff.GetAt(istd::CIndex2d(0, 7)));
		newCenter.SetZ(1);

		return true;
	}

	return false;
}


} // namespace iview


