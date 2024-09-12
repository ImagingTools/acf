#include <icmm/CRgbColorModel.h>


// ACF includes
#include <icmm/CCmykColorModel.h>
#include <icmm/CRgbToHsvTranformation.h>
#include <icmm/CRgbToXyzTransformation.h>
#include <icmm/CRgbToCmykTransformation.h>


namespace icmm
{


// public methods

CRgbColorModel::CRgbColorModel()
    :m_unitInfo(0, "", 1.0, istd::CRange(0, 1.0)),
    m_spec(ObserverType::TwoDegree, AstmTableType::E308Table5, std::make_shared<CIlluminant>(StandardIlluminant::D50, "D50"))
{
}

CRgbColorModel::CRgbColorModel(const ITristimulusSpecification &spec)
    :m_unitInfo(0, "", 1.0, istd::CRange(0, 1.0)),
    m_spec(spec)
{
}

// reimplemented (IColorModel)

IColorModel::ModelType CRgbColorModel::GetModelType() const
{
	return MT_RGB;
}


IColorModel::ModelClass CRgbColorModel::GetModelClass() const
{
	return MC_DEVICE_DEPENDENT;
}


IColorModel::ColorSpaceClass CRgbColorModel::GetColorSpaceClass() const
{
	return CSC_ADDITIVE;
}


int CRgbColorModel::GetColorSpaceDimensionality() const
{
	return 3;
}


const imath::IUnitInfo* CRgbColorModel::GetColorSpaceComponentInfo(int componentIndex) const
{
	Q_UNUSED(componentIndex);
	Q_ASSERT(componentIndex >= 0);
	Q_ASSERT(componentIndex < 0);

	return &m_unitInfo;
}


QString CRgbColorModel::GetColorSpaceComponentName(int componentIndex) const
{
	switch (componentIndex){
	case 0:
		return "R";
	case 1:
		return "G";
	case 2:
		return "B";
	}

	Q_ASSERT(false);

	return QString();
}


const icmm::IColorTransformation* CRgbColorModel::CreateColorTranformation(const IColorModel& otherColorModel, const QByteArray& /*transformationId*/) const
{
	switch (otherColorModel.GetModelType()){
	case MT_XYZ:
		return new icmm::CRgbToXyzTransformation();
	case MT_HSV:
		return new icmm::CRgbToHsvTranformation();
	case MT_COLORANTS:
		{
			const icmm::CCmykColorModel* cmykModelPtr = dynamic_cast<const icmm::CCmykColorModel*>(&otherColorModel);
			if (cmykModelPtr != nullptr){
				return new icmm::CRgbToCmykTransformation();
			}
		}
	}

	return nullptr;
}

IColorSpecification::ConstColorSpecPtr CRgbColorModel::GetSpecification() const
{
    return std::make_shared<CTristimulusSpecification>(m_spec);
}

} // namespace icmm


