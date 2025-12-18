#ifndef iimg_CPixelFormatList_included
#define iimg_CPixelFormatList_included

#include <QtCore/QSet>

// ACF includes
#include <iprm/IOptionsList.h>


namespace iimg
{


class CPixelFormatList: virtual public iprm::IOptionsList
{
public:
	typedef QSet<int> ActiveFormats;

	CPixelFormatList();

	ActiveFormats GetActiveFormats() const;
	void SetActiveFormats(const ActiveFormats& formats);

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const override;
	virtual int GetOptionsCount() const override;
	virtual QString GetOptionName(int index) const override;
	virtual QString GetOptionDescription(int index) const override;
	virtual QByteArray GetOptionId(int index) const override;
	virtual bool IsOptionEnabled(int index) const override;

	// static methods
	static const CPixelFormatList& GetInstance();

private:
	ActiveFormats m_activeFormats;

	static CPixelFormatList s_instance;
};


} // namespace iimg


#endif // !iimg_CPixelFormatList_included


