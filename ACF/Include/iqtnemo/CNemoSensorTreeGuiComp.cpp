#include <QHeaderView>


#include <QItemDelegate>
#include <QLineEdit>


#include "QtAcf/QtAcf.h"

#include "inemo/INemoSensor.h"

#include "iqtnemo/CNemoSensorTreeGuiComp.h"


namespace iqtnemo
{


class SensorItemDelegate: public QItemDelegate
{
public:
	enum{
		ItemData = Qt::UserRole + 1
	};

	SensorItemDelegate(CNemoSensorTreeGuiComp& parent):
		m_parent(parent)
	{
	
	}

	virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		QSize size = QItemDelegate::sizeHint(option, index);
		size.setHeight(25);

		return size;
	}

protected:
	acf::PolymorphicInterface* extractItemData(const QModelIndex& index) const
	{
		QTreeWidgetItem* itemPtr = reinterpret_cast<QTreeWidgetItem*>(index.internalPointer());
		QVariant userData = itemPtr->data(0, ItemData);

		return reinterpret_cast<acf::PolymorphicInterface*>(userData.toInt());
	}


private:
	CNemoSensorTreeGuiComp& m_parent;
};


CNemoSensorTreeGuiComp::CNemoSensorTreeGuiComp()
	:BaseClass(),
	m_stateIconsProviderIfPtr(this, "StateIcons")
{
	m_selectedSensorPtr = NULL;
}


// reimplemented (acf::ObserverInterface)

bool CNemoSensorTreeGuiComp::onAttached(acf::ModelInterface* modelPtr)
{
	if (BaseClass::onAttached(modelPtr)){
		return true;
	}

	return false;
}


void CNemoSensorTreeGuiComp::onDetached(acf::ModelInterface* modelPtr)
{
	ResetView();

	BaseClass::onDetached(modelPtr);
}


void CNemoSensorTreeGuiComp::update(acf::ModelInterface* modelPtr)
{
	ResetView();

	UpdateView();
}


// reimplemented (acf::ModelEditorInterface)

void CNemoSensorTreeGuiComp::updateEditor()
{

}


void CNemoSensorTreeGuiComp::updateModel()
{

}


// protected methods

// reimplemented (acf::QtAbstractGuiComponent)

void CNemoSensorTreeGuiComp::initializeGui()
{
	if (m_widget != NULL){
		SensorList->setColumnCount(3);

		QStringList labels;

		labels << tr("Sensors") << tr("Location (SAP)") << tr("State");
		SensorList->setHeaderLabels(labels);

		SensorList->setItemDelegate(new SensorItemDelegate(*this));

		SensorList->setRootIsDecorated(false);

		SensorList->header()->setResizeMode(QHeaderView::ResizeToContents);
		SensorList->setIconSize(QSize(16, 16));
	}
}


// private methods

void CNemoSensorTreeGuiComp::UpdateView()
{
	inemo::INemoSystemModel* nemoModelPtr = objectPtr();
	assert(nemoModelPtr != NULL);

	int sensorCount = nemoModelPtr->GetSensorCount();

	for (int sensorIndex = 0; sensorIndex < sensorCount; sensorIndex++){
		inemo::INemoSensor& sensor = nemoModelPtr->GetSensor(sensorIndex);

		QTreeWidgetItem* sensorItem = new QTreeWidgetItem();
		sensorItem->setText(0, acf::qtString(sensor.name()));

		if (m_stateIconsProviderIfPtr.isValid()){
			QIcon icon = m_stateIconsProviderIfPtr->getIcon(sensor.GetState());
			

			sensorItem->setIcon(0, icon);
		}

		SensorList->addTopLevelItem(sensorItem);
	}
}


void CNemoSensorTreeGuiComp::ResetView()
{
	while (SensorList->topLevelItemCount()){
		SensorList->takeTopLevelItem(0);
	}

}




} // namespace iqtnemo
