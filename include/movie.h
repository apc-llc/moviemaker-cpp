// Adapted from https://stackoverflow.com/questions/34511312/how-to-encode-a-video-from-several-images-generated-in-a-c-program-without-wri

#ifndef MOVIE_H
#define MOVIE_H

#include <cairo/cairo.h>
#include <stdint.h>
#include <string>
#include <vector>

extern "C"
{
	#include <x264.h>
	#include <libswscale/swscale.h>
	#include <libavcodec/avcodec.h>
	#include <libavutil/mathematics.h>
	#include <libavformat/avformat.h>
	#include <libavutil/opt.h>
}

class Movie
{
	const unsigned int width, height;
	unsigned int iframe;

	SwsContext* convertCtx;
	AVOutputFormat* fmt;
	AVStream* stream;
	AVFormatContext* oc;
	AVCodecContext* c;
	AVPacket pkt;

	AVFrame *rgbpic, *yuvpic;

	std::vector<uint8_t> pixels;

	cairo_surface_t* cairo_surface;

public :

	Movie(const std::string& filename, const unsigned int width, const unsigned int height);

	void addFrame(const std::string& filename);

	void addFrame(const uint8_t* pixels);
	
	~Movie();
};

#endif // MOVIE_H

