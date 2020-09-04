#include <i2d/CParallelogram.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <istd/CChangeNotifier.h>


namespace i2d
{


// public methods

CParallelogram::CParallelogram()
{
}


double CParallelogram::GetRotation() const
{
	const i2d::CMatrix2d& parallDeform = m_transform.GetDeformMatrix();
	return parallDeform.GetApproxAngle();
}


void CParallelogram::SetRotation(double angle)
{
	double curr = GetRotation();
	if (curr != angle){
		istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);
		Q_UNUSED(changeNotifier);
		const double cos = std::cos(angle);
		const double sin = std::sin(angle);
		const double width = GetWidth();
		const double height = GetHeight();
		const i2d::CVector2d center = GetCenter();
		const i2d::CMatrix2d scale(width, 0, 0, height);
		const i2d::CMatrix2d rotation(cos, sin, -sin, cos);
		m_transform.SetDeformMatrix(rotation.GetMultiplied(scale));
		const i2d::CVector2d newCenter = GetCenter();

		m_transform.SetTranslation(GetTranslation() - (newCenter - center));
	}
}


i2d::CVector2d CParallelogram::GetTranslation() const
{
	return m_transform.GetTranslation();
}


void CParallelogram::SetTranslation(const i2d::CVector2d& translation)
{
	if (m_transform.GetTranslation() != translation){
		istd::CChangeNotifier changeNotifier(this, &s_objectMovedChange);
		Q_UNUSED(changeNotifier);

		m_transform.SetTranslation(translation);
	}
}


double CParallelogram::GetWidth() const
{
	const i2d::CMatrix2d& parallDeform = m_transform.GetDeformMatrix();
	const i2d::CVector2d& axisX = parallDeform.GetAxisX();
	return axisX.GetLength();
}


void CParallelogram::SetWidth(double w)
{
	if (GetWidth() != w){
		istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);
		Q_UNUSED(changeNotifier);

		const double angle = GetRotation();
		const double cos = std::cos(angle);
		const double sin = std::sin(angle);
		const double height = GetHeight();
		const i2d::CMatrix2d scale(w, 0, 0, height);
		const i2d::CMatrix2d rotation(cos, sin, -sin, cos);
		m_transform.SetDeformMatrix(rotation.GetMultiplied(scale));
	}
}


double CParallelogram::GetHeight() const
{
	const i2d::CMatrix2d& parallDeform = m_transform.GetDeformMatrix();
	const i2d::CVector2d& axisY = parallDeform.GetAxisY();
	return axisY.GetLength();
}


void CParallelogram::SetHeight(double h)
{
	if (GetHeight() != h){
		istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);
		Q_UNUSED(changeNotifier);

		const double angle = GetRotation();
		const double cos = std::cos(angle);
		const double sin = std::sin(angle);
		const double width = GetWidth();
		const i2d::CMatrix2d scale(width, 0, 0, h);
		const i2d::CMatrix2d rotation(cos, sin, -sin, cos);
		m_transform.SetDeformMatrix(rotation.GetMultiplied(scale));
	}
}


QVector<i2d::CVector2d> CParallelogram::GetNodes() const
{
	const i2d::CMatrix2d deformMatrix = GetDeformMatrix();
	const i2d::CVector2d& axisX = deformMatrix.GetAxisX();
	const i2d::CVector2d& axisY = deformMatrix.GetAxisY();

	const i2d::CVector2d& node1 = m_transform.GetTranslation();
	const i2d::CVector2d& node2 = node1 + axisX;
	const i2d::CVector2d& node3 = node2 + axisY;
	const i2d::CVector2d& node4 = node1 + axisY;

	QVector<i2d::CVector2d> nodes(4);
	nodes[0] = node1;
	nodes[1] = node2;
	nodes[2] = node3;
	nodes[3] = node4;

	return nodes;
}

i2d::CVector2d CParallelogram::GetCenter(const i2d::CMatrix2d& deformMatrix) const
{
	const i2d::CVector2d& axisX = deformMatrix.GetAxisX();
	const i2d::CVector2d& axisY = deformMatrix.GetAxisY();

	const i2d::CVector2d& node1 = m_transform.GetTranslation();
	const i2d::CVector2d& node2 = node1 + axisX;
	const i2d::CVector2d& node3 = node2 + axisY;
	const i2d::CVector2d& center = (node1 + node3) * 0.5;

	return center;
}


// reimplemented (i2d::IObject2d)

i2d::CVector2d CParallelogram::GetCenter() const
{
	return GetCenter(GetDeformMatrix());
}


void CParallelogram::MoveCenterTo(const i2d::CVector2d& position)
{
	const i2d::CVector2d delta = position - GetCenter();
	SetTranslation(GetTranslation() + delta);
}


i2d::CRectangle CParallelogram::GetBoundingBox() const
{
	QVector<i2d::CVector2d> nodes = GetNodes();
	double minX = std::min(std::min(nodes[0].GetX(), nodes[1].GetX()), std::min(nodes[2].GetX(), nodes[3].GetX()));
	double minY = std::min(std::min(nodes[0].GetY(), nodes[1].GetY()), std::min(nodes[2].GetY(), nodes[3].GetY()));
	double maxX = std::max(std::max(nodes[0].GetX(), nodes[1].GetX()), std::max(nodes[2].GetX(), nodes[3].GetX()));
	double maxY = std::max(std::max(nodes[0].GetY(), nodes[1].GetY()), std::max(nodes[2].GetY(), nodes[3].GetY()));

	return i2d::CRectangle(i2d::CVector2d(minX, minY), i2d::CVector2d(maxX, maxY));
}


void CParallelogram::SetTransform(const i2d::CAffine2d& transform)
{
	if (m_transform != transform){
		istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);
		Q_UNUSED(changeNotifier);

		m_transform = transform;
	}
}


bool CParallelogram::Transform(
			const i2d::ITransformation2d& transformation,
			i2d::ITransformation2d::ExactnessMode mode,
			double* /*errorFactorPtr*/)
{
	i2d::CAffine2d localTransform;
	if (transformation.GetLocalTransform(GetCenter(), localTransform, mode)){
		istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);
		Q_UNUSED(changeNotifier);

		m_transform.Apply(localTransform);

		return true;
	}

	return true;
}


bool CParallelogram::InvTransform(
			const i2d::ITransformation2d& transformation,
			i2d::ITransformation2d::ExactnessMode mode,
			double* /*errorFactorPtr*/)
{
	i2d::CAffine2d localTransform;
	if (transformation.GetLocalInvTransform(GetCenter(), localTransform, mode)){
		istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);
		Q_UNUSED(changeNotifier);

		m_transform.Apply(localTransform);

		return true;
	}

	return true;
}


bool CParallelogram::GetTransformed(
			const i2d::ITransformation2d& transformation,
			i2d::IObject2d& result,
			i2d::ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CParallelogram* parallelogramPtr = dynamic_cast<CParallelogram*>(&result);
	if (parallelogramPtr != NULL){
		istd::CChangeNotifier changeNotifier(parallelogramPtr, &s_objectModifiedChange);
		Q_UNUSED(changeNotifier);

		return parallelogramPtr->Transform(transformation, mode, errorFactorPtr);
	}

	return false;
}


bool CParallelogram::GetInvTransformed(
			const i2d::ITransformation2d& transformation,
			i2d::IObject2d& result,
			i2d::ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CParallelogram* parallelogramPtr = dynamic_cast<CParallelogram*>(&result);
	if (parallelogramPtr != NULL){
		istd::CChangeNotifier changeNotifier(parallelogramPtr, &s_objectModifiedChange);
		Q_UNUSED(changeNotifier);

		return parallelogramPtr->InvTransform(transformation, mode, errorFactorPtr);
	}

	return false;
}


// reimplemented (istd::IChangeable)

int CParallelogram::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CParallelogram::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CParallelogram* parallelogramPtr = dynamic_cast<const CParallelogram*>(&object);

	if (parallelogramPtr != NULL){
		istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);
		Q_UNUSED(changeNotifier);
		
		SetTransform(parallelogramPtr->GetTransform());

		CObject2dBase::CopyFrom(object, mode);

		return true;
	}

	return false;
}


istd::IChangeable* CParallelogram::CloneMe(CompatibilityMode mode) const 
{
	istd::TDelPtr<CParallelogram> clonePtr(new CParallelogram);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CParallelogram::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag transformTag("Transform", "Transformation used in parallelogram", iser::CArchiveTag::TT_GROUP);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(transformTag);
	retVal = retVal && m_transform.Serialize(archive);
	retVal = retVal && archive.EndTag(transformTag);

	return retVal;
}


} // namespace i2d


