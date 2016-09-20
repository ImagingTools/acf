#include <iimg/CPixelFormatList.h>


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <iimg/IBitmap.h>


namespace iimg
{


CPixelFormatList::CPixelFormatList()
{
	m_activeFormats << iimg::IBitmap::PF_UNKNOWN;
	m_activeFormats << iimg::IBitmap::PF_MONO;
	m_activeFormats << iimg::IBitmap::PF_GRAY;
	m_activeFormats << iimg::IBitmap::PF_RGB;
	m_activeFormats << iimg::IBitmap::PF_RGBA;
	m_activeFormats << iimg::IBitmap::PF_GRAY16;
	m_activeFormats << iimg::IBitmap::PF_GRAY32;
	m_activeFormats << iimg::IBitmap::PF_FLOAT32;
	m_activeFormats << iimg::IBitmap::PF_FLOAT64;
}


CPixelFormatList::ActiveFormats CPixelFormatList::GetActiveFormats() const
{
	return m_activeFormats;
}


void CPixelFormatList::SetActiveFormats(const ActiveFormats& formats)
{
	m_activeFormats = formats;
}


// reimplemented (iprm::IOptionsList)

int CPixelFormatList::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int CPixelFormatList::GetOptionsCount() const
{
	return IBitmap::PF_FLOAT64 + 1;
}


QString CPixelFormatList::GetOptionName(int index) const
{
	switch (index){
	case iimg::IBitmap::PF_UNKNOWN:
		return QObject::tr("Automatic");

	case iimg::IBitmap::PF_MONO:
		return QObject::tr("Monochrome");

	case iimg::IBitmap::PF_GRAY:
		return QObject::tr("Grayscale");

	case iimg::IBitmap::PF_RGB:
		return QObject::tr("RGB");

	case iimg::IBitmap::PF_RGBA:
		return QObject::tr("RGBA");

	case iimg::IBitmap::PF_GRAY16:
		return QObject::tr("Grayscale 16");

	case iimg::IBitmap::PF_GRAY32:
		return QObject::tr("Grayscale 32");

	case iimg::IBitmap::PF_FLOAT32:
		return QObject::tr("Float");

	case iimg::IBitmap::PF_FLOAT64:
		return QObject::tr("Float 64");

	default:
		return QObject::tr("Unknown");
	}
}


QString CPixelFormatList::GetOptionDescription(int /*index*/) const
{
	return "";
}


QByteArray CPixelFormatList::GetOptionId(int index) const
{
	switch (index){
	case iimg::IBitmap::PF_UNKNOWN:
		return "Auto";

	case iimg::IBitmap::PF_MONO:
		return "Mono";

	case iimg::IBitmap::PF_GRAY:
		return "Gray";

	case iimg::IBitmap::PF_RGB:
		return "RGB";

	case iimg::IBitmap::PF_RGBA:
		return "RGBA";

	case iimg::IBitmap::PF_GRAY16:
		return "Gray16";

	case iimg::IBitmap::PF_GRAY32:
		return "Gray32";

	case iimg::IBitmap::PF_FLOAT32:
		return "Float32";

	case iimg::IBitmap::PF_FLOAT64:
		return "Float64";

	default:
		return "Unknown";
	}
}


bool CPixelFormatList::IsOptionEnabled(int index) const
{
	return m_activeFormats.contains(index);
}


// static methods

const CPixelFormatList& CPixelFormatList::GetInstance()
{
	return s_instance;
}


// static attributes

CPixelFormatList CPixelFormatList::s_instance;


} // namespace iimg


