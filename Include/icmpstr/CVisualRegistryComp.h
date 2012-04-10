#ifndef icmpstr_CVisualRegistryComp_included
#define icmpstr_CVisualRegistryComp_included


// Qt includes
#include <QtCore/QObject>


// ACF includes
#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/CRegistry.h"

#include "ibase/TLoggerCompWrap.h"

#include "icmpstr/IComponentNoteController.h"
#include "icmpstr/CVisualRegistryElement.h"


namespace icmpstr
{


class CVisualRegistryComp:
			public QObject,
			public ibase::CLoggerComponentBase,
			public icomp::CRegistry,
			virtual public IComponentNoteController
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef icomp::CRegistry BaseClass2;

	I_BEGIN_COMPONENT(CVisualRegistryComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(icomp::IRegistry);
		I_REGISTER_INTERFACE(IComponentNoteController);
		I_ASSIGN(m_envManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
	I_END_COMPONENT;

	enum MessageId
	{
		MI_CANNOT_CREATE_ELEMENT = 650
	};

	virtual bool SerializeComponentsLayout(iser::IArchive& archive);
	virtual bool SerializeRegistry(iser::IArchive& archive);
	virtual bool SerializeUserData(iser::IArchive& archive);

	// reimplemented (IComponentNoteController)
	virtual QString GetComponentNote(const QByteArray& componentName);
	virtual void SetComponentNote(const QByteArray& componentName, const QString& componentNote);

	// reimplemented (icomp::IRegistry)
	virtual ElementInfo* InsertElementInfo(
				const QByteArray& elementId,
				const icomp::CComponentAddress& address,
				bool ensureElementCreated = true);
	virtual bool RenameElement(const QByteArray& oldElementId, const QByteArray& newElementId);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	typedef imod::TModelWrap<istd::TChangeDelegator<CVisualRegistryElement> > Element;
	typedef istd::TDelPtr<QIcon> IconPtr;
	typedef QMap<icomp::CComponentAddress, IconPtr> IconMap;

	bool SerializeComponentInfo(
				iser::IArchive& archive,
				QByteArray& componentName,
				i2d::CVector2d& position,
				QString& note);

	// reimplemented (icomp::CRegistry)
	virtual icomp::IRegistryElement* CreateRegistryElement(
				const QByteArray& elementId,
				const icomp::CComponentAddress& address) const;

private:
	I_REF(icomp::IComponentEnvironmentManager, m_envManagerCompPtr);
};


} // namespace icmpstr


#endif // !icmpstr_CVisualRegistryComp_included


