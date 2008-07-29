#include <QDir>

#include "icomp/export.h"

#include "QtCamPck.h"


namespace QtCamPck
{


I_EXPORT_PACKAGE("QtCam", "Qt camera package", "Qt Camera");
I_EXPORT_COMPONENT(FileAcquisition, "Acquise bitmap from file, load and save bitmaps in binary format as file loader", "Image Bitmap Acquisition Binary Loader Load Save JPG JPEG PNG BMP Qt");
I_EXPORT_COMPONENT(SnapImageGui, "Show snapped image and allow to change snap parameters", "View Image Bitmap Camera Snap Parameters GUI Qt");
I_EXPORT_COMPONENT(ExposureParamsGui, "Gui for exposure parameters", "Image Bitmap Camera Snap Acquisition Exposure Parameters GUI Qt");
I_EXPORT_COMPONENT(SnapBitmapSupplierGui, "Gui getting image from camera and connecting it to display console", "Image Bitmap Camera Snap Acquisition Supplier Parameters GUI Qt");


} // namespace QtCamPck
