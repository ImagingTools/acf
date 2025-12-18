#include <icmm/CSubstractiveColorModel.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <icmm/CCmykColorModel.h>
#include <iser/IArchive.h>
#include <iser/CPrimitiveTypesSerializer.h>


namespace icmm
{


// public methods

CSubstractiveColorModel::CSubstractiveColorModel()
	: m_bridge(this, imod::CModelUpdateBridge::UF_SOURCE)
{
	m_colorants.AttachObserver(&m_bridge);
}


CSubstractiveColorModel::CSubstractiveColorModel(const ColorantIds& colorantIds)
	: CSubstractiveColorModel()
{
	for (const ColorantId& colorantId : colorantIds){
		ColorantUsage usage = GetDefaultUsageFromColorantName(colorantId);

		m_colorants.push_back({colorantId, usage});
	}
}


CSubstractiveColorModel::CSubstractiveColorModel(const CSubstractiveColorModel& other)
	: CSubstractiveColorModel()
{
	CSubstractiveColorModelBase::operator=(other);
	m_colorants.SetBaseObject(other.m_colorants);
}


CSubstractiveColorModel::CSubstractiveColorModel(const ISubstractiveColorModel& other)
	: CSubstractiveColorModel()
{
	for (const ColorantId& colorantId : other.GetColorantIds()){
		ColorantUsage usage = other.GetColorantUsage(colorantId);

		m_colorants.push_back({ colorantId, usage });
		icmm::CCieLabColor cieLab(nullptr);
		if (other.GetColorantVisualInfo(colorantId, cieLab)){
			SetColorantPreview(colorantId, cieLab.GetLab());
		}
	}
}


CSubstractiveColorModel& CSubstractiveColorModel::operator=(const CSubstractiveColorModel& other)
{
	CSubstractiveColorModelBase::operator=(other);
	m_colorants.SetBaseObject(other.m_colorants);
	return *this;
}


bool CSubstractiveColorModel::operator==(const CSubstractiveColorModel& ref) const
{
	return m_colorants == ref.m_colorants;
}


bool CSubstractiveColorModel::operator!=(const CSubstractiveColorModel& ref) const
{
	return !operator==(ref);
}


bool CSubstractiveColorModel::ContainsColorant(const ColorantId& colorantId) const
{
	return FindColorantIndex(colorantId) >= 0;
}


bool CSubstractiveColorModel::InsertColorant(const ColorantId & colorantId, ColorantUsage usage, int index)
{
	int existingIndex = FindColorantIndex(colorantId);
	if (existingIndex >= 0){
		return false;
	}

	int insertPosition = index >= 0 ? index : m_colorants.size();

	m_colorants.insert(insertPosition, { colorantId, usage });

	return true;
}


bool CSubstractiveColorModel::RemoveColorant(const ColorantId & colorantId)
{
	for (qsizetype i = 0; i < m_colorants.size(); ++i){
		if (m_colorants[i].id == colorantId){
			m_colorants.remove(i);
			return true;
		}
	}

	return false;
}


bool CSubstractiveColorModel::SetColorantUsage(const ColorantId& colorantId, ColorantUsage usage)
{
	int colorantIndex = FindColorantIndex(colorantId);
	if (colorantIndex >= 0){
		if (m_colorants[colorantIndex].usage != usage){
			auto changes = ElementUpdatedChanges(colorantIndex);
			istd::CChangeNotifier changeNotifier(this, &changes);

			m_colorants[colorantIndex].usage = usage;
		}

		return true;
	}

	Q_ASSERT_X(false, "CSubstractiveColorModel::SetColorantUsage", "Trying to change a non-existing colorant");

	return false;
}


bool CSubstractiveColorModel::SetColorantId(const ColorantId& colorantId, const ColorantId& newColorantId)
{
	if (colorantId == newColorantId){
		return true;
	}

	int colorantIndex = FindColorantIndex(colorantId);
	if (colorantIndex >= 0){
		auto changes = ElementUpdatedChanges(colorantIndex);
		istd::CChangeNotifier changeNotifier(this, &changes);
		
		m_colorants[colorantIndex].id = newColorantId;

		return true;
	}

	Q_ASSERT_X(false, "CSubstractiveColorModel::SetColorantId", "Trying to change a non-existing colorant");

	return false;
}


bool CSubstractiveColorModel::AppendColorModel(const ISubstractiveColorModel& other)
{
	CSubstractiveColorModel temp(*this);
	
	ColorantIds otherColorantIds = other.GetColorantIds();
	for (const ColorantId& otherColorantId : otherColorantIds){
		ColorantUsage otherUsage = other.GetColorantUsage(otherColorantId);

		// Check if the current model contains the colorant from the other model:
		int existingIndex = temp.FindColorantIndex(otherColorantId);
		if (existingIndex >= 0){
			// If yes, check the corresponding colorant usage:
			ColorantUsage myUsage = temp.GetColorantUsage(otherColorantId);
			
			// If colorant usages are not the same, the color model cannot be merged:
			if (myUsage != otherUsage){
				return false;
			}
			else {
				continue;
			}
		}

		temp.m_colorants.push_back({ otherColorantId, otherUsage });

		icmm::CCieLabColor cieLab(nullptr);
		if (other.GetColorantVisualInfo(otherColorantId, cieLab)){
			temp.SetColorantPreview(otherColorantId, cieLab.GetLab());
		}
	}

	if (m_colorants != temp.m_colorants){
		m_colorants.SetBaseObject(temp.m_colorants);
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


// reimplemented ISubstractiveColorModel

std::unique_ptr<ISubstractiveColorModel> CSubstractiveColorModel::CreateSubspaceModel(const QStringList& colorantIds) const
{
	return CreateSubspaceModelFrom(*this, colorantIds);
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


// reimplemented (iser::ISerializable)

bool CSubstractiveColorModel::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	int colorantCount = m_colorants.size();

	const iser::CArchiveTag colorantListTag("Colorants", "List of colorants", iser::CArchiveTag::TT_MULTIPLE);
	const iser::CArchiveTag colorantInfoTag("ColorantInfo", "Single colorant description", iser::CArchiveTag::TT_GROUP, &colorantListTag);

	retVal = retVal && archive.BeginMultiTag(colorantListTag, colorantInfoTag, colorantCount);
	if (!retVal){
		return false;
	}

	istd::CChangeNotifier changeNotifier(archive.IsStoring() ? nullptr : this, &istd::IChangeable::GetAllChanges());

	if (!archive.IsStoring()){
		m_colorants.resize(colorantCount);
	}

	for (int i = 0; i < colorantCount; ++i){
		retVal = retVal && archive.BeginTag(colorantInfoTag);
		retVal = retVal && SerializeColorantInfo(archive, m_colorants[i], &colorantInfoTag);
		retVal = retVal && archive.EndTag(colorantInfoTag);
	}

	retVal = retVal && archive.EndTag(colorantListTag);

	const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();
	quint32 frameworkVersion = 0;
	if (!versionInfo.GetVersionNumber(iser::IVersionInfo::AcfVersionId, frameworkVersion) || (frameworkVersion >= 5436)){
		const iser::CArchiveTag previewSpecTag("PreviewSpec", "Colorant preview specification", iser::CArchiveTag::TT_GROUP);
		retVal = retVal && archive.BeginTag(previewSpecTag);
		retVal = retVal && m_previewSpec.Serialize(archive);
		retVal = retVal && archive.EndTag(previewSpecTag);

		auto SerializeKey = [](iser::IArchive& archive, ColorantId& key){
			return archive.Process(key);
		};

		const iser::CArchiveTag colorantPreviewMapTag("ColorantPreviews", "Previews of the model colorants", iser::CArchiveTag::TT_GROUP);
		retVal = retVal && archive.BeginTag(colorantPreviewMapTag);
		retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeAssociativeObjectContainer<ColorantPreviewMap, ColorantId>(archive, m_colorantPreviewMap, SerializeKey, "Colorants", "Colorant", "ColorantId", "Lab");
		retVal = retVal && archive.EndTag(colorantPreviewMapTag);
	}

	return retVal;
}


// reimplemented (istd::IChangeable)

int CSubstractiveColorModel::GetSupportedOperations() const
{
	return SO_CLONE | SO_COPY | SO_RESET;
}


bool CSubstractiveColorModel::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
{
	const CSubstractiveColorModel* sourceModelPtr = dynamic_cast<const CSubstractiveColorModel*>(&object);
	if (sourceModelPtr != nullptr){
		bool retVal = m_colorants.CopyFrom(sourceModelPtr->m_colorants);
		retVal = retVal && m_previewSpec.CopyFrom(sourceModelPtr->m_previewSpec);

		if (retVal){
			m_colorantPreviewMap = sourceModelPtr->m_colorantPreviewMap;
		}

		return retVal;
	}

	return false;
}


istd::IChangeableUniquePtr CSubstractiveColorModel::CloneMe(CompatibilityMode /*mode*/) const
{
	return new CSubstractiveColorModel(*this);
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
	for (int index = 0; index < m_colorants.size(); ++index){
		if (m_colorants[index].id == colorantId){
			return index;
		}
	}

	return -1;
}


bool CSubstractiveColorModel::SerializeColorantInfo(
			iser::IArchive& archive,
			ColorantInfo& colorantInfo,
			const iser::CArchiveTag* parentTagPtr) const
{
	bool retVal = true;

	const iser::CArchiveTag colorantIdTag("ColorantId", "ID of the colorant", iser::CArchiveTag::TT_LEAF, parentTagPtr);
	retVal = retVal && archive.BeginTag(colorantIdTag);
	retVal = retVal && archive.Process(colorantInfo.id);
	retVal = retVal && archive.EndTag(colorantIdTag);

	const iser::CArchiveTag usageTag("Usage", "Usage of the colorant", iser::CArchiveTag::TT_LEAF, parentTagPtr);
	retVal = retVal && archive.BeginTag(usageTag);
	retVal = retVal && I_SERIALIZE_ENUM(ColorantUsage, archive, colorantInfo.usage);
	retVal = retVal && archive.EndTag(usageTag);

	return retVal;
}

// static methods

std::unique_ptr<ISubstractiveColorModel> CSubstractiveColorModel::CreateSubspaceModelFrom(const CSubstractiveColorModelBase& model, const QStringList& colorantIds) 
{
	auto subModel = std::make_unique<imod::TModelWrap<CSubstractiveColorModel>>();
	subModel->SetPreviewSpec(model.GetPreviewSpec());

	auto modelIds = model.GetColorantIds();

	for (const auto& id : colorantIds){
		if (!modelIds.contains(id)){
			return nullptr;
		}
		if (!subModel->InsertColorant(id, model.GetColorantUsage(id))){
			return nullptr;
		}
		icmm::CCieLabColor cieLab(nullptr);
		if (model.GetColorantVisualInfo(id, cieLab)){
			subModel->SetColorantPreview(id, cieLab.GetLab());
		}
	}

	return subModel;
}


} // namespace icmm


