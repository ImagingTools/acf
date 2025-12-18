#include <icmm/CCmyColorModel.h>
#include <icmm/CSubstractiveColorModel.h>


namespace icmm
{


// public static methods

IColorantList::ColorantId CCmyColorModel::GetCyan()
{
	return "Cyan";
}


IColorantList::ColorantId CCmyColorModel::GetMagenta()
{
	return "Magenta";
}


IColorantList::ColorantId CCmyColorModel::GetYellow()
{
	return "Yellow";
}


// public methods

// reimplemented (icmm::IColorantList)

IColorantList::ColorantIds CCmyColorModel::GetColorantIds() const
{
	return {GetCyan(), GetMagenta(), GetYellow()};
}


icmm::ColorantUsage CCmyColorModel::GetColorantUsage(const ColorantId& colorantId) const
{
	Q_ASSERT(GetColorantIds().contains(colorantId));

	if (colorantId == GetCyan()){
		return CU_CYAN;
	}
	else if (colorantId == GetMagenta()){
		return CU_MAGENTA;
	}
	else if (colorantId == GetYellow()){
		return CU_YELLOW;
	}

	return CU_NONE;
}

std::unique_ptr<ISubstractiveColorModel> CCmyColorModel::CreateSubspaceModel(const QStringList& colorantIds) const
{
	return CSubstractiveColorModel::CreateSubspaceModelFrom(*this, colorantIds);
}


} // namespace icmm


