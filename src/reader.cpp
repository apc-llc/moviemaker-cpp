#include "movie.h"

#include <cstdlib>

using namespace std;

MovieReader::MovieReader(const string& filename, unsigned int width_, unsigned int height_) :

fc(NULL), width(width_), height(height_)

{
	int ret;

	const string::size_type p(filename.find_last_of('.'));
	string ext = "";
	if (p != -1) ext = filename.substr(p);

	fmt = av_guess_format(ext.c_str(), NULL, NULL);

	// Get input file format context
	if ((ret = avformat_open_input(&fc, filename.c_str(), 0, 0)) < 0)
	{
		fprintf(stderr, "Could not open input file \"%s\"", filename.c_str());
		exit(-1);
	}
	
	// Extract streams description
	if ((ret = avformat_find_stream_info(fc, 0)) < 0)
	{
		fprintf(stderr, "Failed to retrieve input stream information");
		exit(-1);
	}
	
	// Print detailed information about the input or output format,
	// such as duration, bitrate, streams, container,
	// programs, metadata, side data, codec and time base.
	av_dump_format(fc, 0, filename.c_str(), 0);

	ivstream = -1;
	for (int i = 0; i < fc->nb_streams; i++)
	{
		if (fc->streams[i]->codec->coder_type == AVMEDIA_TYPE_VIDEO)
		{
			ivstream = i;
			c = fc->streams[i]->codec;
			AVCodec* codec = avcodec_find_decoder(c->codec_id);
			if (!codec)
			{
				fprintf(stderr, "Input codec not found\n");
				exit(1);
			}

			// Open input codec
			avcodec_open2(c, codec, NULL);

			break;
		}
	}

	pFrame = av_frame_alloc();
	pFrameRGB = av_frame_alloc();

	// Size of movie frame in pixels.
	int szframe = avpicture_get_size(AV_PIX_FMT_BGR24, width, height);

	// Allocate frame buffer. 
	uint8_t* buffer = (uint8_t*)av_malloc(szframe * sizeof(uint8_t));

	// Create frame structure.
	avpicture_fill((AVPicture*)pFrameRGB, buffer, AV_PIX_FMT_BGR24, width, height);
}

bool MovieReader::getFrame(vector<uint8_t>& pixels)
{
	while (1)
	{
		AVPacket pkt;
		int ret = av_read_frame(fc, &pkt);
		if (ret < 0) return false;

		if (pkt.stream_index != ivstream)
			continue;

		int frameFinished;
		avcodec_decode_video2(c, pFrame, &frameFinished, &pkt);
		if (!frameFinished) continue;

		SwsContext* swsCtx;
		swsCtx = sws_getCachedContext(NULL,
			c->width, c->height, c->pix_fmt,
			width, height, AV_PIX_FMT_BGR24,
			SWS_BICUBIC, NULL, NULL, NULL);
		sws_scale(swsCtx, ((AVPicture*)pFrame)->data, ((AVPicture*)pFrame)->linesize,
			0, c->height, ((AVPicture *)pFrameRGB)->data, ((AVPicture *)pFrameRGB)->linesize);
		sws_freeContext(swsCtx);

		pixels.resize(4 * width * height);
		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
		    	// pFrameRGB->linesize[0] is equal to width.
				pixels[y * 4 * width + 4 * x + 0] = pFrameRGB->data[0][y * pFrameRGB->linesize[0] + 3 * x + 0];
				pixels[y * 4 * width + 4 * x + 1] = pFrameRGB->data[0][y * pFrameRGB->linesize[0] + 3 * x + 1];
				pixels[y * 4 * width + 4 * x + 2] = pFrameRGB->data[0][y * pFrameRGB->linesize[0] + 3 * x + 2];
			}
		}

		break;
	}

	return true;
}

int MovieReader::getFrameRate() const { return c->framerate.num; }

MovieReader::~MovieReader()
{
	// Closing the file.
	if (!(fmt->flags & AVFMT_NOFILE))
	    avio_closep(&fc->pb);

	avformat_close_input(&fc);
	avcodec_close(c);
	av_free(pFrame);
	av_free(pFrameRGB);
}

