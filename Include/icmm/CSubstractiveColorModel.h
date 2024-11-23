#pragma once


// ACF includes
#include <iser/ISerializable.h>
#include <iser/CArchiveTag.h>
#include <icmm/CSubstractiveColorModelBase.h>


namespace icmm
{


/**
	Common implementation of the general device-based, substractive color model based on the list of colorants.
	All kinds of colorants (Process, ECG and Spot) can be combined in this color model.
*/
class CSubstractiveColorModel:
			public CSubstractiveColorModelBase,
			virtual public iser::ISerializable
{
public:
	CSubstractiveColorModel();
	CSubstractiveColorModel(const ColorantIds& colorantIds);
	CSubstractiveColorModel(const CSubstractiveColorModel& other);
	CSubstractiveColorModel(const ISubstractiveColorModel& other);

	bool operator==(const CSubstractiveColorModel& ref) const;
	bool operator!=(const CSubstractiveColorModel& ref) const;

	/**
		Returns \c true if the colorant with the given ID exists in the color model.
	*/
	virtual bool ContainsColorant(const ColorantId& colorantId) const;

	/**
		Insert a new colorant at the given position.
		If a colorant with the same colorant-ID already exists, the method will fail.
		\param index	Position, where the new colorant will be inserted.
		A negative value means the new colorant will be inserted at the end of the colorant list.
	*/
	virtual bool InsertColorant(const ColorantId& colorantId, ColorantUsage usage, int index = -1);

	/**
		Remove a colorant with the given index.
		\return Returns \c true, if the operation was succesful or \c false otherwise.
	*/
	virtual bool RemoveColorant(const ColorantId& colorantId);

	/**
		Append colorants from from another model to this one.
		\return Returns \c true, if the operation was succesful or \c false otherwise.
	*/
	virtual bool AppendColorModel(const ISubstractiveColorModel& other);

	/**
		Check if the color model contains process colorants.
	*/
	virtual bool HasProcessColorants() const;

	/**
		Check if the color model contains ECG colorants.
	*/
	virtual bool HasEcg() const;

	/**
		Check if the color model contains any spot colors.
	*/
	virtual bool HasSpot() const;

	/**
		Get the list of process colors in this color model.
	*/
	virtual ColorantIds GetProcessColorants() const;

	/**
		Get the list of ECG colorants in this color model.
	*/
	virtual ColorantIds GetEcgColorants() const;

	/**
		Get the list of spot colors in this color model.
	*/
	virtual ColorantIds GetSpotColorants() const;

	// reimplemented (icmm::IColorantList)
	virtual ColorantIds GetColorantIds() const override;
	virtual ColorantUsage GetColorantUsage(const ColorantId& colorantId) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	struct ColorantInfo
	{
		ColorantId id;
		ColorantUsage usage = CU_NONE;

		inline bool operator==(const ColorantInfo& ref) const
		{
			return (ref.id == id) && (ref.usage == usage);
		}

		inline bool operator!=(const ColorantInfo& ref)  const
		{
			return !operator==(ref);
		}
	};

	typedef QVector<ColorantInfo> ColorantInfoList;

protected:
	static icmm::ColorantUsage GetDefaultUsageFromColorantName(const ColorantId& colorantId);
	static ColorantId GetEcgGreen();
	static ColorantId GetEcgOrange();
	static ColorantId GetEcgViolet();
	static ColorantId GetEcgRed();
	static ColorantId GetEcgBlue();

	int FindColorantIndex(const ColorantId& colorantId) const;
	bool SerializeColorantInfo(
				iser::IArchive& archive,
				ColorantInfo& colorantInfo,
				const iser::CArchiveTag* parentTagPtr) const;

private:
	ColorantInfoList m_colorants;
};


} // namespace icmm


