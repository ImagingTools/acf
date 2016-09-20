#include <CAbComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


// static constants
static const istd::IChangeable::ChangeSet s_changeAChangeSet(IAb::CF_A_CHANGED, QObject::tr("Change A"));
static const istd::IChangeable::ChangeSet s_changeBChangeSet(IAb::CF_B_CHANGED, QObject::tr("Change B"));
static const iser::CArchiveTag s_aTag("A", "Value of A", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_bTag("B", "Value of B", iser::CArchiveTag::TT_LEAF);


// public methods

CAbComp::CAbComp()
:	m_a(0),
	m_aRange(0, 100) // Constraints [0, 100]
{
}


// reimplemented (iprm::IAbConstraints)

const IAConstraints& CAbComp::GetAConstraints() const
{
	return *this;
}


int CAbComp::GetA() const
{
	return m_a;
}


bool CAbComp::SetA(int a)
{
	// If A out of range, return false:
	if (!m_aRange.Contains(a)){
		return false;
	}

	// If new value of A differs from the old,
	// set the new value and fire notification about this change:
	if (m_a != a){
		istd::CChangeNotifier notifier(this, &s_changeAChangeSet);
		Q_UNUSED(notifier);

		m_a = a;
	}

	return true;
}


QByteArray CAbComp::GetB() const
{
	return m_b;
}


void CAbComp::SetB(const QByteArray& b)
{
	// If new value of B differs from the old,
	// set the new value and fire notification about this change:
	if (m_b != b){
		istd::CChangeNotifier notifier(this, &s_changeBChangeSet);
		Q_UNUSED(notifier);

		m_b = b;
	}
}


// reimplemented (IAConstraints)

const istd::CRange& CAbComp::GetARange() const
{
	return m_aRange;
}


// reimplemented (iser::ISerializable)

bool CAbComp::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	// Copy current A value to a  temporary variable:
	int a = m_a;

	// Process value copy of A
	bool retVal = archive.BeginTag(s_aTag);
	retVal = retVal && archive.Process(a);
	retVal = retVal && archive.EndTag(s_aTag);

	// Process value of B
	retVal = retVal && archive.BeginTag(s_bTag);
	retVal = retVal && archive.Process(m_b);
	retVal = retVal && archive.EndTag(s_bTag);

	// If archive is loading, we set A via the method SetA() to guarantee, that the contraints of A were perceived:
	if (retVal && !archive.IsStoring()){
		retVal = retVal && SetA(a);
	}

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CAbComp::OnComponentCreated()
{
	// Call the parent class implementation:
	BaseClass::OnComponentCreated();

	// If the default value attribute for A is valid, then set the value from attribute to data model:
	if (m_defaultAValueAttrPtr.IsValid()){
		m_a = *m_defaultAValueAttrPtr;
	}

	// If the default value attribute for B is valid, then set the value from attribute to data model:
	if (m_defaultBValueAttrPtr.IsValid()){
		m_b = *m_defaultBValueAttrPtr;
	}
}


