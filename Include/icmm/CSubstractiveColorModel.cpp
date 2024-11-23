#include <icmm/CSubstractiveColorModel.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <icmm/CCmykColorModel.h>


namespace icmm
{


// public methods

CSubstractiveColorModel::CSubstractiveColorModel()
{
}


CSubstractiveColorModel::CSubstractiveColorModel(const ColorantIds& colorantIds)
{
	for (const ColorantId& colorantId : colorantIds){
		ColorantUsage usage = GetDefaultUsageFromColorantName(colorantId);

		m_colorants.push_back({colorantId, usage});
	}
}


CSubstractiveColorModel::CSubstractiveColorModel(const CSubstractiveColorModel& other)
{
	m_colorants = other.m_colorants;
}


CSubstractiveColorModel::CSubstractiveColorModel(const ISubstractiveColorModel& other)
{
	for (const ColorantId& colorantId : other.GetColorantIds()){
		ColorantUsage usage = other.GetColorantUsage(colorantId);

		m_colorants.push_back({ colorantId, usage });
	}
}


bool CSubstractiveColorModel::InsertColorant(const ColorantId & colorantId, ColorantUsage usage, int index)
{
	int existingIndex = FindColorantIndex(colorantId);
	if (existingIndex >= 0) {
		return false;
	}

	istd::CChangeNotifier changeNotifier(this);

	int insertPosition = index >= 0 ? index : m_colorants.count();

	m_colorants.insert(insertPosition, { colorantId, usage });

	return true;
}


bool CSubstractiveColorModel::RemoveColorant(const ColorantId & colorantId)
{
	QMutableListIterator<ColorantInfo> colorantIterator(m_colorants);
	while (colorantIterator.hasNext()){
		ColorantInfo& colorantInfo = colorantIterator.next();
		if (colorantInfo.id == colorantId){
			istd::CChangeNotifier changeNotifier(this);

			colorantIterator.remove();

			return true;
		}
	}

	return false;
}


bool CSubstractiveColorModel::AppendColorModel(const ISubstractiveColorModel& other)
{
	ColorantIds otherColorantIds = other.GetColorantIds();
	for (const ColorantId& otherColorantId : otherColorantIds){
		ColorantUsage otherUsage = other.GetColorantUsage(otherColorantId);

		// Check if the current model contains the colorant from the other model:
		int existingIndex = FindColorantIndex(otherColorantId);
		if (existingIndex >= 0) {
			// If yes, check the corresponding colorant usage:
			ColorantUsage myUsage = GetColorantUsage(otherColorantId);
			
			// If colorant usages are not the same, the color model cannot be merged:
			if (myUsage != otherUsage){
				return false;
			}
			else {
				continue;
			}
		}

		m_colorants.push_back({ otherColorantId, otherUsage });
	}

	return true;
}


bool CSubstractiveColorModel::HasProcessColorants() const
{
	for (const ColorantInfo& colorant : m_colorants){
		if ((colorant.usage != icmm::CU_ECG) && (colorant.usage != icmm::CU_SPOT)){
			return true;
		}
	}

	return false;
}


bool CSubstractiveColorModel::HasEcg() const
{
	for (const ColorantInfo& colorant : m_colorants){
		if (colorant.usage == icmm::CU_ECG){
			return true;
		}
	}

	return false;
}


bool CSubstractiveColorModel::HasSpot() const
{
	for (const ColorantInfo& colorant : m_colorants){
		if (colorant.usage == icmm::CU_SPOT){
			return true;
		}
	}

	return false;
}


IColorantList::ColorantIds CSubstractiveColorModel::GetProcessColorants() const
{
	ColorantIds processColorants;

	for (const ColorantInfo& colorant : m_colorants){
		if ((colorant.usage != icmm::CU_ECG) && (colorant.usage != icmm::CU_SPOT)){
			processColorants.push_back(colorant.id);
		}
	}

	return processColorants;
}


IColorantList::ColorantIds CSubstractiveColorModel::GetEcgColorants() const
{
	ColorantIds ecgColorants;

	for (const ColorantInfo& colorant : m_colorants){
		if (colorant.usage == icmm::CU_ECG){
			ecgColorants.push_back(colorant.id);
		}
	}

	return ecgColorants;
}


IColorantList::ColorantIds CSubstractiveColorModel::GetSpotColorants() const
{
	ColorantIds spotColorants;

	for (const ColorantInfo& colorant : m_colorants){
		if (colorant.usage == icmm::CU_SPOT){
			spotColorants.push_back(colorant.id);
		}
	}

	return spotColorants;
}


// reimplemented (icmm::IColorantList)

IColorantList::ColorantIds CSubstractiveColorModel::GetColorantIds() const
{
	ColorantIds retVal;

	for (const ColorantInfo& colorant : m_colorants){
		retVal.push_back(colorant.id);
	}

	return retVal;
}


icmm::ColorantUsage CSubstractiveColorModel::GetColorantUsage(const ColorantId& colorantId) const
{
	for (const ColorantInfo& colorant : m_colorants){
		if (colorantId == colorant.id){
			return colorant.usage;
		}
	}

	return CU_NONE;
}


// protected static methods

icmm::ColorantUsage CSubstractiveColorModel::GetDefaultUsageFromColorantName(const ColorantId& colorantId)
{
	if (colorantId == icmm::CCmykColorModel::GetCyan()){
		return icmm::CU_CYAN;
	}
	else if (colorantId == icmm::CCmykColorModel::GetMagenta()){
		return icmm::CU_MAGENTA;
	}
	else if (colorantId == icmm::CCmykColorModel::GetYellow()){
		return icmm::CU_YELLOW;
	}
	else if (colorantId == icmm::CCmykColorModel::GetBlack()){
		return icmm::CU_BLACK;
	}
	else if (colorantId == GetEcgOrange()){
		return icmm::CU_ECG;
	}
	else if (colorantId == GetEcgGreen()){
		return icmm::CU_ECG;
	}
	else if (colorantId == GetEcgViolet()){
		return icmm::CU_ECG;
	}
	else if (colorantId == GetEcgRed()){
		return icmm::CU_ECG;
	}
	else if (colorantId == GetEcgBlue()){
		return icmm::CU_ECG;
	}

	return CU_SPOT;
}


icmm::IColorantList::ColorantId CSubstractiveColorModel::GetEcgGreen()
{
	return "Green";
}


icmm::IColorantList::ColorantId CSubstractiveColorModel::GetEcgOrange()
{
	return "Orange";
}


icmm::IColorantList::ColorantId CSubstractiveColorModel::GetEcgViolet()
{
	return "Violet";
}


icmm::IColorantList::ColorantId CSubstractiveColorModel::GetEcgRed()
{
	return "Red";
}


icmm::IColorantList::ColorantId CSubstractiveColorModel::GetEcgBlue()
{
	return "Blue";
}


int CSubstractiveColorModel::FindColorantIndex(const ColorantId& colorantId) const
{
	for (int index = 0; index < m_colorants.count(); ++index){
		if (m_colorants[index].id == colorantId){
			return index;
		}
	}

	return -1;
}


} // namespace icmm


