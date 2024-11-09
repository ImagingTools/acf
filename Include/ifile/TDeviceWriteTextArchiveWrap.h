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
class TDeviceWriteTextArchiveWrap: public Base
{
public:
	typedef Base BaseClass;

	TDeviceWriteTextArchiveWrap(
			QIODevice& device,
			const iser::IVersionInfo* versionInfoPtr = NULL,
			bool serializeHeader = true,
			const iser::CArchiveTag& rootTag = iser::CArchiveBase::s_acfRootTag);
	~TDeviceWriteTextArchiveWrap();

	bool Flush();

protected:
	QIODevice& GetDevice() const;

	// pseudo-reimplemented (iser::CTextWriteArchiveBase)
	virtual bool WriteTextNode(const QByteArray& text) override;

private:
	Device& m_device;
};


// public methods

template <class Base, class Device>
TDeviceWriteTextArchiveWrap<Base, Device>::TDeviceWriteTextArchiveWrap(
			QIODevice& device,
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
	:BaseClass(versionInfoPtr, serializeHeader, rootTag),
	m_device(device)
{
	if (!m_device.isOpen()){
		if (m_device.open(QIODevice::WriteOnly | QIODevice::Text)){
			BaseClass::InitArchive(&m_device);
		}
		else{
			Q_ASSERT(false);
		}
	}
	else{
		Q_ASSERT(m_device.isWritable());
		Q_ASSERT(m_device.isTextModeEnabled());

		BaseClass::InitArchive(&m_device);
	}
}


template <class Base, class Device>
TDeviceWriteTextArchiveWrap<Base, Device>::~TDeviceWriteTextArchiveWrap()
{
	Flush();
}


template <class Base, class Device>
bool TDeviceWriteTextArchiveWrap<Base, Device>::Flush()
{
	BaseClass::Flush();

	if (m_device.isOpen()){
		m_device.close();

		return true;
	}

	return false;
}


// protected methods

template <class Base, class Device>
QIODevice& TDeviceWriteTextArchiveWrap<Base, Device>::GetDevice() const
{
	return m_device;
}


// pseudo-reimplemented (iser::CTextWriteArchiveBase)

template <class Base, class Device>
bool TDeviceWriteTextArchiveWrap<Base, Device>::WriteTextNode(const QByteArray& text)
{
	if (!m_device.isOpen()){
		return false;
	}

	return BaseClass::WriteTextNode(text);
}


} // namespace ifile


