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

enum StillFrameType
{
	StillFramePNG,
	StillFrameSVG
};

class MovieWriter
{
	const unsigned int width, height;
	unsigned int iframe;
	int frameRate;

	SwsContext* swsCtx;
	AVOutputFormat* fmt;
	AVStream* stream;
	AVFormatContext* fc;
	AVCodecContext* ctx;
	AVPacket pkt;

	AVFrame *rgbpic, *yuvpic;

	std::vector<uint8_t> pixels;

	cairo_surface_t* cairo_surface;

public :

	MovieWriter(const std::string& filename, const unsigned int width, const unsigned int height, const int frameRate = 25);

	void addFrame(const std::string& filename, AVFrame** yuvout = nullptr);
	
	void addFrame(const std::vector<uint8_t>& data, StillFrameType type, AVFrame** yuvout = nullptr);

	void addFrame(const uint8_t* pixels, AVFrame** yuvout = nullptr);

	void addFrame(AVFrame* yuvframe);
	
	~MovieWriter();
};

class MovieReader
{
	const unsigned int width, height;

	SwsContext* swsCtx;
	AVOutputFormat* fmt;
	AVStream* stream;
	AVFormatContext* fc;
	AVCodecContext* c;
	AVFrame* pFrame;
	AVFrame* pFrameRGB;

	// The index of video stream.
	int ivstream;

public :

	MovieReader(const std::string& filename, const unsigned int width, const unsigned int height);

	bool getFrame(std::vector<uint8_t>& pixels);
	int getFrameRate() const;

	~MovieReader();	
};

#endif // MOVIE_H

