#include <icmm/CSubstractiveColorModelBase.h>


// ACF includes
#include <imath/CGeneralUnitInfo.h>
#include <iser/CPrimitiveTypesSerializer.h>


namespace icmm
{


// public methods

CSubstractiveColorModelBase::CSubstractiveColorModelBase()
	:m_previewSpec(ObserverType::TwoDegree, AstmTableType::Unknown, std::make_shared<CIlluminant>())
{
}


const icmm::CTristimulusSpecification& CSubstractiveColorModelBase::GetPreviewSpec() const
{
	return m_previewSpec;
}


void CSubstractiveColorModelBase::SetPreviewSpec(const ITristimulusSpecification& previewSpec)
{
	istd::CChangeNotifier changeNotifier(this);

	m_previewSpec = icmm::CTristimulusSpecification(previewSpec);
}


void CSubstractiveColorModelBase::SetColorantPreview(const ColorantId& colorantId, const icmm::CLab& preview)
{
	istd::CChangeNotifier changeNotifier(this);

	m_colorantPreviewMap[colorantId] = preview;
}


// reimplemented (icmm::IColorModel)

IColorModel::ModelType CSubstractiveColorModelBase::GetModelType() const
{
	return MT_COLORANTS;
}


IColorModel::ModelClass CSubstractiveColorModelBase::GetModelClass() const
{
	return MC_DEVICE_DEPENDENT;
}


IColorModel::ColorSpaceClass CSubstractiveColorModelBase::GetColorSpaceClass() const
{
	return CSC_SUBSTRACTIVE;
}


int CSubstractiveColorModelBase::GetColorSpaceDimensionality() const
{
	return GetColorantIds().count();
}


const imath::IUnitInfo* CSubstractiveColorModelBase::GetColorSpaceComponentInfo(int /*componentIndex*/) const
{
	static const imath::CGeneralUnitInfo unitInfoBase(imath::IUnitInfo::UnitType::UT_RELATIVE, "", 100.0, istd::CRange(0, 1.0));

	return &unitInfoBase;
}


QString CSubstractiveColorModelBase::GetColorSpaceComponentName(int componentIndex) const
{
	ColorantIds colorantIds = GetColorantIds();

	Q_ASSERT(componentIndex < colorantIds.count());
	Q_ASSERT(componentIndex >= 0);

	return colorantIds[componentIndex];
}


const icmm::IColorTransformation* CSubstractiveColorModelBase::CreateColorTranformation(
	const IColorModel& /*otherColorModel*/,
	const QByteArray& /*transformationId*/) const
{
	return nullptr;
}


bool CSubstractiveColorModelBase::GetColorantVisualInfo(const ColorantId& colorantId, icmm::ICieLabColor& preview) const
{
	if (m_colorantPreviewMap.contains(colorantId)){
		return preview.Initialize(m_colorantPreviewMap[colorantId], m_previewSpec);
	}

	return false;
}


} // namespace icmm


