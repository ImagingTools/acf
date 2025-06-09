#pragma once


// Qt includes
#include <QtCore/QIODevice>

// ACF includes
#include <istd/IChangeable.h>


namespace ibase
{
	class IProgressManager;
}


namespace ifile
{


/**
	Interface providing reading and writing data objects via I/O-devices.

	\ingroup Persistence
*/
class IDeviceBasedPersistence: virtual public istd::IPolymorphic
{
public:
	/**
		Result of device-related operation.
	*/
	enum OperationState
	{
		/**
			Operation was successful.
		*/
		Successful,

		/**
			Operation was aborted by user.
		*/
		Canceled,

		/**
			Operation failed.
		*/
		Failed
	};

	enum DeviceOperation
	{
		ReadOperation = 1,
		WriteOperation = 2
	};

	/**
		Possible operation errors.
	*/
	enum ErrorType
	{
		/**
			No write access granted.
		*/
		NoWritePermissions,

		/**
			No read access granted.
		*/
		NoReadPermissions,

		/**
			Device could not be opened
		*/
		NoDeviceAccess,

		/**
			Reading the data was failed.
		*/
		ReadOperationFailed,

		/**
			Writing the data was failed.
		*/
		WriteOperationFailed,

		/**
			The archive has a wrong/unsupported version.
		*/
		UnsupportedArchiveVersion
	};

	/**
		Returns \c true if object \c dataObject can be processed via defined I/O-device.
		\param	dataObject		data object should be red/written.
	*/
	virtual bool IsDeviceOperationSupported(const istd::IChangeable& dataObject, const QIODevice& device, int deviceOperation) const = 0;

	/**
		This function loads data \c data from the given I/O-device \c device
		\returns State of the reading operation. \sa OperationState
	*/
	virtual int ReadFromDevice(
				istd::IChangeable& data,
				QIODevice& device,
				ibase::IProgressManager* progressManagerPtr = nullptr) const = 0;

	/**
		This function writes data \c data to the given I/O-device \c device
		\returns State of the writing operation. \sa OperationState
	*/
	virtual int WriteToDevice(
				const istd::IChangeable& data,
				QIODevice& device,
				ibase::IProgressManager* progressManagerPtr = nullptr) const = 0;
};


} // namespace ifile


