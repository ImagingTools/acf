#include "BitmapPck.h"


#include "icomp/export.h"


namespace BitmapPck
{


I_EXPORT_PACKAGE(
			"Acf/Bitmap",
			"Bitmap package",
			IM_PROJECT("ACF") IM_TAG("Bitmap") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			Bitmap,
			"QImage based bitmap implementation",
			"QImage" IM_TAG("Image Model"));

I_EXPORT_COMPONENT(
			GeneralBitmap,
			"Buffer based bitmap implementation",
			"Bitmap" IM_TAG("Image Model"));

I_EXPORT_COMPONENT(
			BitmapSurface,
			"QImage based bitmap implementation with surface extension",
			"Sampled Function QImage Surface" IM_TAG("Image Model 3D"));

I_EXPORT_COMPONENT(
			BitmapLoader,
			"Loader loading bitmaps using Qt standard implementations",
			IM_TAG("Image File Loader"));

I_EXPORT_COMPONENT(
			MultiPageBitmap,
			"Multi-page bitmap document",
			IM_TAG("Image Bitmap Document Pages Page"));

I_EXPORT_COMPONENT(
			ComposedBitmapProvider,
			"Provider of the composed bitmap list",
			IM_TAG("Image Bitmap List Multi"));

I_EXPORT_COMPONENT(
			MultiPageBitmapSequence,
			"Sequence of multi-page bitmaps",
			"Bitmap Multi Sequence" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("Bitmap"));



} // namespace BitmapPck


