#ifndef CComponentView_included
#define CComponentView_included


// Qt includes
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QIcon>


#include "imod/TModelWrap.h"
#include "imod/TSingleModelObserverBase.h"

#include "icomp/IRegistry.h"

#include "icmpstr/IElementSelectionInfo.h"


namespace icmpstr
{


class CComponentConnector;
class CRegistryView;


class CComponentView:
			public QObject,
			public QGraphicsRectItem,
			protected imod::TSingleModelObserverBase<icomp::IRegistry>,
			virtual public imod::TModelWrap<IElementSelectionInfo>
{
	Q_OBJECT

public:
	typedef imod::TSingleModelObserverBase<icomp::IRegistry> BaseObserverClass;

	CComponentView(
				const CRegistryView* registryViewPtr,
				icomp::IRegistry* registryPtr,
				const icomp::IRegistry::ElementInfo* elementInfoPtr, 
				const std::string& componentName, 
				QGraphicsItem* parent = NULL);

	virtual ~CComponentView();

	/**
		Get registry element info associated with this object.
	*/
	const icomp::IRegistry::ElementInfo& GetElementInfo() const;
	/**
		Set registry element info associated with this object.
		\param	elementInfoPtr	pointer to element info object. It cannot be NULL.
	*/
	void SetElementInfo(const icomp::IRegistry::ElementInfo* elementInfoPtr);
	const std::string& GetComponentName() const;

	void AddConnector(CComponentConnector* connector);
	void RemoveConnector(const CComponentConnector* connector);
	void RemoveAllConnectors();

	QRectF GetInnerRect() const;

	// reimplemented (icmpstr::IElementSelectionInfo)
	virtual icomp::IRegistry* GetSelectedRegistry() const;
	virtual iser::ISerializable* GetSelectedElement() const;
	virtual const std::string& GetSelectedElementName() const;
	virtual QIcon GetSelectedElementIcon() const;
	virtual const icomp::CComponentAddress* GetSelectedElementAddress() const;

signals:
	void selectionChanged(CComponentView*, bool);
	void positionChanged(CComponentView*, const QPoint& point);

protected:
	QRect CalculateRect() const;

	void CalcExportedInteraces();

	// reimplemented (QGraphicsRectItem)
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	const CRegistryView& m_registryView;

	icomp::IRegistry& m_registry;
	std::string m_componentName;
	const icomp::IRegistry::ElementInfo* m_elementInfoPtr;

	mutable int m_componentLabelHeight;
	mutable int m_componentIdHeight;

	const int m_componentLabelFontSize;
	const int m_componentIdFontSize;

	QList<CComponentConnector*> m_connectors;

	QStringList m_exportedInterfacesList;

	QPixmap m_image;
};


} // namespace icmpstr


#endif // CComponentView_included

