#include "ilibav/CLibAvVideoDecoder.h"


// LIBAV includes
extern "C"{
#include "libavcodec/imgconvert.h"
}


#include "istd/TChangeNotifier.h"


namespace ilibav
{


// public methods

CLibAvVideoDecoder::CLibAvVideoDecoder()
:	m_streamId(-1),
	m_formatContextPtr(NULL),
	m_codecContextPtr(NULL),
	m_codecPtr(NULL),
	m_framePtr(NULL),
	m_frameRgbPtr(NULL),
	m_bytesRemaining(0),
	m_rawDataPtr(NULL)

{
	av_register_all();

	m_packet.data = NULL;
}


// reimplemented (icomp::IComponent)

void CLibAvVideoDecoder::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_framePtr = avcodec_alloc_frame();
	m_frameRgbPtr = avcodec_alloc_frame();
}


void CLibAvVideoDecoder::OnComponentDestroyed()
{
	CloseMedium();

	if (m_framePtr != NULL){
		av_free(m_framePtr);

		m_framePtr = NULL;
	}

	if (m_frameRgbPtr != NULL){
		av_free(m_frameRgbPtr);

		m_frameRgbPtr = NULL;
	}

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imm::IMediaController)

istd::CString CLibAvVideoDecoder::GetOpenedMediumUrl() const
{
	return m_currentUrl;
}


bool CLibAvVideoDecoder::OpenMediumUrl(const istd::CString& url, bool /*autoPlay*/)
{
	istd::CChangeNotifier notifier(this, CF_STATUS);

	CloseMedium();

	if (av_open_input_file(&m_formatContextPtr, url.ToString().c_str(), NULL, 0, NULL) != 0){
		return false;
	}

	if (m_formatContextPtr == NULL){
		return false;
	}

	if (av_find_stream_info(m_formatContextPtr) < 0){
		return false;
	}

	m_currentUrl = url;

	// Find the first video stream
	for (unsigned int i = 0; i < m_formatContextPtr->nb_streams; i++){
		AVCodecContext* codecContextPtr = m_formatContextPtr->streams[i]->codec;
		if ((codecContextPtr != NULL) && (codecContextPtr->codec_type == CODEC_TYPE_VIDEO)){
			m_streamId = i;
			m_codecContextPtr = codecContextPtr;

			// Determine required buffer size and allocate buffer
			int numBytes = avpicture_get_size(
						PIX_FMT_RGB32,
						m_codecContextPtr->width,
						m_codecContextPtr->height);
			m_imageBufferPtr.SetPtr(new I_BYTE[numBytes]);

			// Assign appropriate parts of buffer to image planes in pFrameRGB
			avpicture_fill(
						(AVPicture*)m_frameRgbPtr,
						m_imageBufferPtr.GetPtr(),
						PIX_FMT_RGB32,
						m_codecContextPtr->width,
						m_codecContextPtr->height);

			// Find the decoder for the video stream
			m_codecPtr = avcodec_find_decoder(m_codecContextPtr->codec_id);
			if (m_codecPtr==NULL){
				return false;
			}

			// Inform the codec that we can handle truncated bitstreams -- i.e.,
			// bitstreams where frame boundaries can fall in the middle of packets
			if (m_codecPtr->capabilities & CODEC_CAP_TRUNCATED){
				m_codecContextPtr->flags |= CODEC_FLAG_TRUNCATED;
			}

			// Open codec
			if (avcodec_open(m_codecContextPtr, m_codecPtr) < 0){
				return false;
			}

			return true;
		}
	}

	// Get a pointer to the codec context for the video stream

	return false;
}


void CLibAvVideoDecoder::CloseMedium()
{
	istd::CChangeNotifier notifier(this, CF_STATUS);

	if (m_formatContextPtr != NULL){
		av_close_input_file(m_formatContextPtr);

		m_formatContextPtr = NULL;
	}

	m_codecContextPtr = NULL;

	m_packet.data = NULL;
	m_bytesRemaining = 0;
	m_rawDataPtr = NULL;

	m_imageBufferPtr.Reset();
}


bool CLibAvVideoDecoder::IsPlaying() const
{
	return false;
}


bool CLibAvVideoDecoder::SetPlaying(bool /*state*/)
{
	return false;
}


double CLibAvVideoDecoder::GetMediumLength() const
{
	return 0;
}


double CLibAvVideoDecoder::GetCurrentPosition() const
{
	return 0;
}


bool CLibAvVideoDecoder::SetCurrentPosition(double /*position*/)
{
	return false;
}


int	CLibAvVideoDecoder::GetSupportedFeatures() const
{
	return SF_GRAB_CURRENT;
}


// reimplemented (imm::IVideoInfo)

int CLibAvVideoDecoder::GetFramesCount() const
{
	return 0;
}


double CLibAvVideoDecoder::GetFrameIntervall() const
{
	if (m_codecContextPtr != NULL){
		AVRational timeBase = m_codecContextPtr->time_base;
		if (timeBase.den != 0){
			return double(timeBase.num) / timeBase.den;
		}
	}

	return -1;
}


istd::CIndex2d CLibAvVideoDecoder::GetFrameSize() const
{
	if (m_codecContextPtr != NULL){
		return istd::CIndex2d(m_codecContextPtr->width, m_codecContextPtr->height);
	}

	return istd::CIndex2d::GetInvalid();
}


double CLibAvVideoDecoder::GetPixelAspectRatio() const
{
	if ((m_codecContextPtr != NULL) && (m_codecContextPtr->sample_aspect_ratio.num != 0)){
		AVRational ratio = m_codecContextPtr->sample_aspect_ratio;
		if (ratio.den != 0){
			return double(ratio.num) / ratio.den;
		}
	}

	return -1;
}


// reimplemented (imm::IVideoController)

int CLibAvVideoDecoder::GetCurrentFrame() const
{
	return 0;
}


bool CLibAvVideoDecoder::SetCurrentFrame(int /*frameIndex*/)
{
	return false;
}


bool CLibAvVideoDecoder::GrabFrame(iimg::IBitmap& result, int /*frameIndex*/) const
{
	if (!m_imageBufferPtr.IsValid() || !const_cast<CLibAvVideoDecoder*>(this)->ReadNextFrame()){
		return false;
	}

	istd::CIndex2d bitmapSize = GetFrameSize();
	if (result.CreateBitmap(bitmapSize, 32, 4)){
		for (int y = 0; y < bitmapSize[1]; ++y){
			::memcpy(result.GetLinePtr(y), m_frameRgbPtr->data[0], result.GetLineBytesCount());
		}

		return true;
	}

	return false;
}


// protected methods

bool CLibAvVideoDecoder::ReadNextFrame()
{
	if (		(m_formatContextPtr != NULL) ||
				(m_codecContextPtr == NULL) ||
				(m_framePtr == NULL) ||
				(m_frameRgbPtr == NULL)){
		return false;
	}

	// Decode packets until we have decoded a complete frame
	while (true){
		// Work on the current packet until we have decoded all of it
		while (m_bytesRemaining > 0){
			// Decode the next chunk of data
			int frameFinished = 0;
			int bytesDecoded = avcodec_decode_video(m_codecContextPtr, m_framePtr, &frameFinished, m_rawDataPtr, m_bytesRemaining);

			// Was there an error?
			if (bytesDecoded < 0){
				return false;
			}

			m_bytesRemaining -= bytesDecoded;
			m_rawDataPtr += bytesDecoded;

			// Did we finish the current frame? Then we can return
			if (frameFinished != 0){
				img_convert((AVPicture*)m_frameRgbPtr,
							PIX_FMT_RGB32,
							(AVPicture*)m_framePtr,
							m_codecContextPtr->pix_fmt,
							m_codecContextPtr->width,
							m_codecContextPtr->height);

				return true;
			}
		}

		// Read the next packet, skipping all packets that aren't for this
		// stream
		do{
			// Free old packet
			if (m_packet.data != NULL){
				av_free_packet(&m_packet);
			}

			// Read new packet
			if (av_read_packet(m_formatContextPtr, &m_packet) != 0){
				return false;
			}
		} while (m_packet.stream_index != m_streamId);

		m_bytesRemaining = m_packet.size;
		m_rawDataPtr = m_packet.data;
	}
}


} // namespace ilibav


