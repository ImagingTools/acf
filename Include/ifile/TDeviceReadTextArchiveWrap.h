#pragma once


// Qt includes
#include <QtCore/QIODevice>

// ACF includes
#include <iser/IVersionInfo.h>
#include <iser/CArchiveTag.h>


namespace ifile
{


/**
	Template wrapper for the text-based writing archives using an i/o-device given from outside.
*/
template <class Base, class Device = QIODevice>
class TDeviceReadTextArchiveWrap: public Base
{
public:
	typedef Base BaseClass;

	TDeviceReadTextArchiveWrap(
			QIODevice& device,
			bool serializeHeader = true,
			const iser::CArchiveTag& rootTag = iser::CArchiveBase::s_acfRootTag);
	~TDeviceReadTextArchiveWrap();

protected:
	QIODevice& GetDevice() const;

private:
	Device& m_device;
};


// public methods

template <class Base, class Device>
TDeviceReadTextArchiveWrap<Base, Device>::TDeviceReadTextArchiveWrap(
			QIODevice& device,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
	:BaseClass(serializeHeader, rootTag),
	m_device(device)
{
	if (!m_device.isOpen()){
		if (!m_device.open(QIODevice::ReadOnly | QIODevice::Text)){
			Q_ASSERT(false);

			return;
		}
	}

	if (m_device.isOpen()){
		Q_ASSERT(m_device.isReadable());
		Q_ASSERT(m_device.isTextModeEnabled());

		if (!BaseClass::SetContent(&m_device)){
			Q_ASSERT(false);

			m_device.close();
		}
	}
}


template <class Base, class Device>
TDeviceReadTextArchiveWrap<Base, Device>::~TDeviceReadTextArchiveWrap()
{
	m_device.close();
}


// protected methods

template <class Base, class Device>
QIODevice& TDeviceReadTextArchiveWrap<Base, Device>::GetDevice() const
{
	return m_device;
}


} // namespace ifile


