// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/CIlluminant.h>


// ACF includes
#include <icmm/CVarColor.h>
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/IVersionInfo.h>
#include <iser/CArchiveTag.h>
#include <iser/CPrimitiveTypesSerializer.h>

// Qt includes
#include <QtCore/QDebug>


namespace icmm
{


// public static methods

QByteArray CIlluminant::GetTypeId()
{
	return QByteArrayLiteral("icmm::CIlluminant");
}


// public methods

CIlluminant::CIlluminant(StandardIlluminant illuminantType)
	:m_illuminantType(illuminantType)
{
	InitFromStandardIlluminant(illuminantType);
}


CIlluminant::CIlluminant(const QString& illuminantName, const icmm::CSpectrum& spectralPowerDistribution)
	:m_illuminantType(icmm::StandardIlluminant::Custom),
	m_illuminantName(illuminantName),
	m_spectralPowerDistribution(spectralPowerDistribution)
{
}


CIlluminant::CIlluminant(const CIlluminant & other)
	:m_illuminantName(other.m_illuminantName),
	m_spectralPowerDistribution(other.m_spectralPowerDistribution),
	m_illuminantType(other.m_illuminantType)
{
}


// reimplemented (IIlluminant)

const icmm::CSpectrum& CIlluminant::GetSpectralPowerDistribution() const
{
	return m_spectralPowerDistribution;
}


void CIlluminant::SetSpectralPowerDistribution(const icmm::CSpectrum& spectrum)
{
	if (!m_spectralPowerDistribution.IsEqual(spectrum)){
		istd::CChangeNotifier changeNotifier(this);

		m_spectralPowerDistribution.CopyFrom(spectrum);
	}
}


QString CIlluminant::GetIlluminantName() const
{
	return m_illuminantName;
}


void CIlluminant::SetIlluminantName(const QString& illuminantName)
{
	if (m_illuminantName != illuminantName){
		istd::CChangeNotifier changeNotifier(this);

		m_illuminantName = illuminantName;
	}
}


StandardIlluminant CIlluminant::GetIlluminantType() const
{
	return m_illuminantType;
}


void CIlluminant::SetIlluminantType(const StandardIlluminant& illuminantType)
{
	if (m_illuminantType != illuminantType){
		istd::CChangeNotifier changeNotifier(this);

		m_illuminantType = illuminantType;
	}
}


// reimplemented (iser::IObject)

QByteArray CIlluminant::GetFactoryId() const
{
	return GetTypeId();
}


// reimplemented (iser::ISerializable)

bool CIlluminant::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring() ? nullptr : this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	iser::CArchiveTag illuminantTypeTag("IlluminantType", "Illuminant", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(illuminantTypeTag);
	retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeQEnum(archive, m_illuminantType);
	retVal = retVal && archive.EndTag(illuminantTypeTag);

	iser::CArchiveTag illuminantNameTag("IlluminantName", "Name of the illuminant", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(illuminantNameTag);
	retVal = retVal && archive.Process(m_illuminantName);
	retVal = retVal && archive.EndTag(illuminantNameTag);

	const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();
	quint32 frameworkVersion = 0;
	bool hasSpectralPowerDistribution =
		!versionInfo.GetVersionNumber(iser::IVersionInfo::AcfVersionId, frameworkVersion) || (frameworkVersion >= 6526);

	if (hasSpectralPowerDistribution){
		iser::CArchiveTag spectrumTag(
					"SpectralPowerDistribution", "Spectral power distribution of the illuminant", iser::CArchiveTag::TT_GROUP);
		retVal = retVal && archive.BeginTag(spectrumTag);
		retVal = retVal && m_spectralPowerDistribution.Serialize(archive);
		retVal = retVal && archive.EndTag(spectrumTag);
	}
	else if (!archive.IsStoring()){
		// Pre-1f0b426b3 data stored an illuminant white point, it had no real use case.
		icmm::CVarColor whitePoint;
		iser::CArchiveTag whitePointTag("WhitePoint", "White point of the illuminant", iser::CArchiveTag::TT_GROUP);
		retVal = retVal && archive.BeginTag(whitePointTag);
		retVal = retVal && whitePoint.Serialize(archive);
		retVal = retVal && archive.EndTag(whitePointTag);
		qWarning() << "Legacy Illuminant WhitePoint data discarded";
	}

	return retVal;
}


// reimplemented (istd::IChangeable)

int CIlluminant::GetSupportedOperations() const
{
	return SO_CLONE | SO_COMPARE | SO_COPY;
}


bool CIlluminant::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const CIlluminant* objectPtr = dynamic_cast<const CIlluminant*>(&object);
	if (objectPtr != nullptr){
		istd::CChangeNotifier notifier(this);

		m_illuminantType = objectPtr->m_illuminantType;
		m_illuminantName = objectPtr->m_illuminantName;
		m_spectralPowerDistribution.CopyFrom(objectPtr->m_spectralPowerDistribution);

		return true;
	}

	return false;
}


bool CIlluminant::IsEqual(const IChangeable& other) const
{
	const IIlluminant* objectPtr = dynamic_cast<const IIlluminant*>(&other);

	if (objectPtr == nullptr){
		return false;
	}

	return
				m_illuminantType == objectPtr->GetIlluminantType() &&
				m_illuminantName == objectPtr->GetIlluminantName() &&
				m_spectralPowerDistribution.IsEqual(objectPtr->GetSpectralPowerDistribution());
}


istd::IChangeableUniquePtr CIlluminant::CloneMe(CompatibilityMode mode) const
{
	istd::IChangeableUniquePtr clonePtr(new CIlluminant());
	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr;
	}

	return istd::IChangeableUniquePtr();
}


// protected methods

void CIlluminant::InitFromStandardIlluminant(StandardIlluminant illuminantType)
{
	/// \todo Implement spectral power distribution for each standard illuminant types.

	switch (illuminantType){
	case icmm::StandardIlluminant::A:
		m_illuminantName = "A";
		break;
	case icmm::StandardIlluminant::B:
		m_illuminantName = "B";
		break;
	case icmm::StandardIlluminant::C:
		m_illuminantName = "C";
		break;
	case icmm::StandardIlluminant::D50:
		m_illuminantName = "D50";
		break;
	case icmm::StandardIlluminant::D55:
		m_illuminantName = "D55";
		break;
	case icmm::StandardIlluminant::D60:
		m_illuminantName = "D60";
		break;
	case icmm::StandardIlluminant::D65:
		m_illuminantName = "D65";
		break;
	case icmm::StandardIlluminant::D75:
		m_illuminantName = "D75";
		break;
	case icmm::StandardIlluminant::E:
		m_illuminantName = "E";
		break;
	default:
		Q_ASSERT(false);
		break;
	}
}


} // namespace icmm

