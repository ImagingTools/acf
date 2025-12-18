#include <icmm/CCmykColorModel.h>
#include <icmm/CSubstractiveColorModel.h>
#include <imod/TModelWrap.h>


namespace icmm
{


// static public methods

IColorantList::ColorantId CCmykColorModel::GetBlack()
{
	return "Black";
}


// public methods

// reimplemented (icmm::IColorantList)

IColorantList::ColorantIds CCmykColorModel::GetColorantIds() const
{
	ColorantIds retVal = BaseClass::GetColorantIds();

	retVal += GetBlack();

	return retVal;
}


icmm::ColorantUsage CCmykColorModel::GetColorantUsage(const ColorantId& colorantId) const
{
	Q_ASSERT(GetColorantIds().contains(colorantId));

	if (colorantId == GetBlack()){
		return CU_BLACK;
	}

	return BaseClass::GetColorantUsage(colorantId);
}


std::unique_ptr<ISubstractiveColorModel> CCmykColorModel::CreateSubspaceModel(const QStringList& colorantIds) const
{
	auto subModel = std::make_unique<imod::TModelWrap<CCmyColorModel>>();

	if (colorantIds == subModel->GetColorantIds()) {
		subModel->SetPreviewSpec(GetPreviewSpec());
		for (const auto& id: subModel->GetColorantIds()) {
			icmm::CCieLabColor cieLab(nullptr);
			if (GetColorantVisualInfo(id, cieLab)) {
				subModel->SetColorantPreview(id, cieLab.GetLab());
			}
		}
		return subModel;
	}

	return CSubstractiveColorModel::CreateSubspaceModelFrom(*this, colorantIds);
}


} // namespace icmm


