#include <imath/CGeneralUnitInfo.h>


// ACF includes
#include <iser/CArchiveTag.h>
#include <iser/CPrimitiveTypesSerializer.h>
#include <imath/CDoubleManip.h>


namespace imath
{


// static attributes

static const iser::CArchiveTag s_typeTag("Type", "Type of this unit", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_nameTag("Name", "Name of this unit", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_multFactorTag("MultFactor", "Display multiplication factor", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_rangeTag("Range", "Range of values", iser::CArchiveTag::TT_GROUP);


CGeneralUnitInfo::CGeneralUnitInfo(
			int type,
			const QString& name,
			double displayMultFactor,
			const istd::CRange& range,
			const imath::IDoubleManip* valueManipPtr)
:	m_type(type),
	m_name(name),
	m_displayMultFactor(displayMultFactor),
	m_range(range),
	m_valueManipPtr(valueManipPtr)
{
}


void CGeneralUnitInfo::SetUnitType(int type)
{
	m_type = type;
}


void CGeneralUnitInfo::SetUnitName(const QString& name)
{
	m_name = name;
}


void CGeneralUnitInfo::SetDisplayMultiplicationFactor(double factor)
{
	m_displayMultFactor = factor;
}


void CGeneralUnitInfo::SetValueRange(const istd::CRange& range)
{
	m_range = range;
}


bool CGeneralUnitInfo::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && archive.BeginTag(s_typeTag);
	retVal = retVal && archive.Process(m_type);
	retVal = retVal && archive.EndTag(s_typeTag);

	retVal = retVal && archive.BeginTag(s_nameTag);
	retVal = retVal && archive.Process(m_name);
	retVal = retVal && archive.EndTag(s_nameTag);

	retVal = retVal && archive.BeginTag(s_multFactorTag);
	retVal = retVal && archive.Process(m_displayMultFactor);
	retVal = retVal && archive.EndTag(s_multFactorTag);

	retVal = retVal && archive.BeginTag(s_rangeTag);
	retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeRange(archive, m_range);
	retVal = retVal && archive.EndTag(s_rangeTag);

	return retVal;
}


//	reimplemented (imath::IUnitInfo)

int CGeneralUnitInfo::GetUnitType() const
{
	return m_type;
}


QString CGeneralUnitInfo::GetUnitName() const
{
	return m_name;
}


double CGeneralUnitInfo::GetDisplayMultiplicationFactor() const
{
	return m_displayMultFactor;
}


istd::CRange CGeneralUnitInfo::GetValueRange() const
{
	return m_range;
}


const imath::IDoubleManip& CGeneralUnitInfo::GetValueManip() const
{
	if (m_valueManipPtr != NULL){
		return *m_valueManipPtr;
	}

	static imath::CDoubleManip manip;

	return manip;
}


} // namespace imath


